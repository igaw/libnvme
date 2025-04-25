// SPDX-License-Identifier: LGPL-2.1-or-later
/*
 * This file is part of libnvme.
 * Copyright (c) 2025 SUSE.
 *
 * Authors: Daniel Wagner <dwagner@suse.de>
 */

#ifdef CONFIG_LIBURING
#include <liburing.h>
#endif

#include "nvme/types.h"
#include "nvme/types2.h"
#include "nvme/ioctl.h"
#include "nvme/ioctl2.h"

#undef CONFIG_LIBURING

__attribute__((weak))
int nvme2_submit_passthru(int fd, unsigned long ioctl_cmd,
			  struct nvme_passthru_cmd *cmd, __u32 *result)
{
	int err = ioctl(fd, ioctl_cmd, cmd);

	if (err >= 0 && result)
		*result = cmd->result;
	return err;
}

int nvme2_submit_admin_passthru(int fd, struct nvme_passthru_cmd *cmd, __u32 *result)
{
	return nvme2_submit_passthru(fd, NVME_IOCTL_ADMIN_CMD, cmd, result);
}

int nvme2_get_log(struct nvme2_handle *hdl, struct nvme2_common_command *cmd,
		  void *log, __u64 len, __u64 start)
{
	__u32 numd = (len >> 2) - 1;
	__u16 numdu = numd >> 16, numdl = numd & 0xffff;

	cmd->cmd.opcode		= nvme_admin_get_log_page;
	cmd->cmd.addr		= (__u64)(uintptr_t)log;
	cmd->cmd.data_len	= len;

	NVME_SET_FIELD(cmd, LOG_NUMDL, numdl);
	NVME_SET_FIELD(cmd, LOG_NUMDU, numdu);
	NVME_SET_FIELD(cmd, LOG_LPO_LOW, start);
	NVME_SET_FIELD(cmd, LOG_LPO_HIGH, start);

	return nvme_submit_admin_passthru(hdl->fd, &cmd->cmd, &cmd->cmd.result);
}

#ifdef CONFIG_LIBURING
enum {
	IO_URING_NOT_AVAILABLE,
	IO_URING_AVAILABLE,
} io_uring_kernel_support = IO_URING_NOT_AVAILABLE;

/*
 * gcc specific attribute, call automatically on the library loading.
 * if IORING_OP_URING_CMD is not supported, fallback to ioctl interface.
 */
__attribute__((constructor))
static void nvme2_uring_cmd_probe()
{
	struct io_uring_probe *probe = io_uring_get_probe();

	if (!probe)
		return;

	if (!io_uring_opcode_supported(probe, IORING_OP_URING_CMD))
		return;

	io_uring_kernel_support = IO_URING_AVAILABLE;
}

static int nvme2_uring_cmd_setup(struct io_uring *ring)
{
	return io_uring_queue_init(NVME_URING_ENTRIES, ring,
				   IORING_SETUP_SQE128 | IORING_SETUP_CQE32);
}

static void nvme2_uring_cmd_exit(struct io_uring *ring)
{
	io_uring_queue_exit(ring);
}

static int nvme2_uring_cmd_admin_passthru_async(struct io_uring *ring, struct nvme2_cmd_buf *buf)
{
	struct io_uring_sqe *sqe;
	struct nvme_uring_cmd *cmd;
	int ret;

	__u32 numd = (args->len >> 2) - 1;
	__u16 numdu = numd >> 16, numdl = numd & 0xffff;

	__u32 cdw10 = NVME_SET(args->lid, LOG_CDW10_LID) |
			NVME_SET(args->lsp, LOG_CDW10_LSP) |
			NVME_SET(!!args->rae, LOG_CDW10_RAE) |
			NVME_SET(numdl, LOG_CDW10_NUMDL);
	__u32 cdw11 = NVME_SET(numdu, LOG_CDW11_NUMDU) |
			NVME_SET(args->lsi, LOG_CDW11_LSI);
	__u32 cdw12 = args->lpo & 0xffffffff;
	__u32 cdw13 = args->lpo >> 32;
	__u32 cdw14 = NVME_SET(args->uuidx, LOG_CDW14_UUID) |
			NVME_SET(!!args->ot, LOG_CDW14_OT) |
			NVME_SET(args->csi, LOG_CDW14_CSI);

	if (args->args_size < sizeof(struct nvme_get_log_args)) {
		errno = EINVAL;
		return -1;
	}

	sqe = io_uring_get_sqe(ring);
	if (!sqe)
		return -1;

	cmd = (void *)&sqe->cmd;
	cmd->opcode        = nvme_admin_get_log_page,
	cmd->nsid          = args->nsid,
	cmd->addr          = (__u64)(uintptr_t)args->log,
	cmd->data_len      = args->len,
	cmd->cdw10         = cdw10,
	cmd->cdw11         = cdw11,
	cmd->cdw12         = cdw12,
	cmd->cdw13         = cdw13,
	cmd->cdw14         = cdw14,
	cmd->timeout_ms    = args->timeout,

	sqe->fd = args->fd;
	sqe->opcode = IORING_OP_URING_CMD;
	sqe->cmd_op = NVME_URING_CMD_ADMIN;
	sqe->user_data = (__u64)(uintptr_t)args;

	ret = io_uring_submit(ring);
	if (ret < 0) {
		errno = -ret;
		return -1;
	}

	return 0;
}

static int nvme_uring_cmd_wait_complete(struct io_uring *ring, int n)
{
	struct nvme_get_log_args *args;
	struct io_uring_cqe *cqe;
	int i, ret = 0;

	for (i = 0; i < n; i++) {
		ret = io_uring_wait_cqe(ring, &cqe);
		if (ret) {
			errno = -ret;
			return -1;
		}

		if (cqe->res) {
			args = (struct nvme_get_log_args *)cqe->user_data;
			if (args->result)
				*args->result = cqe->res;
			ret = cqe->res;
			break;
		}

		io_uring_cqe_seen(ring, cqe);
	}

	return ret;
}
#endif

int nvme2_get_log_page(struct nvme2_handle *hdl,
		       struct nvme2_common_command *cmd,
		       void *log, __u64 len, __u64 start,
		       __u32 xfer_len)
{
	bool retain = NVME_GET_FIELD(cmd, LOG_RAE);
	__u64 offset = 0, xfer;
	int ret;

#ifdef CONFIG_LIBURING
	int n = 0;
	struct io_uring ring;

	if (io_uring_kernel_support == IO_URING_AVAILABLE) {
		if (nvme_uring_cmd_setup(&ring))
			return -1;
	}
#endif
	/*
	 * 4k is the smallest possible transfer unit, so restricting to 4k
	 * avoids having to check the MDTS value of the controller.
	 */
	do {
		xfer = len - offset;
		if (xfer > xfer_len)
			xfer  = xfer_len;

		/*
		 * Always retain regardless of the RAE parameter until the very
		 * last portion of this log page so the data remains latched
		 * during the fetch sequence.
		 */
		NVME_SET_FIELD(cmd, LOG_RAE, offset + xfer < len || retain);
#ifdef CONFIG_LIBURING
		if (io_uring_kernel_support == IO_URING_AVAILABLE) {
			if (n >= NVME_URING_ENTRIES) {
				ret = nvme_uring_cmd_wait_complete(&ring, n);
				n = 0;
			}
			n += 1;
			ret = nvme_uring_cmd_admin_passthru_async(&ring, args);

			if (ret)
				nvme_uring_cmd_exit(&ring);
		} else
#endif
		ret = nvme2_get_log(hdl, cmd, log, xfer, start + offset);
		if (ret)
			return ret;

		offset += xfer;
		log += xfer;
	} while (offset < len);

#ifdef CONFIG_LIBURING
	if (io_uring_kernel_support == IO_URING_AVAILABLE) {
		ret = nvme_uring_cmd_wait_complete(&ring, n);
		nvme_uring_cmd_exit(&ring);
		if (ret)
			return ret;
	}
#endif
	return 0;
}
