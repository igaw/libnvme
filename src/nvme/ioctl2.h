#ifndef NVME2_IOCTL_H
#define NVME2_IOCTL_H

#include <nvme/types.h>
#include <nvme/types2.h>

/**
 * DOC: ioctl.h
 *
 * Linux NVMe ioctl interface functions
 */

/* '0' is interpreted by the kernel to mean 'apply the default timeout' */
#define NVME2_DEFAULT_IOCTL_TIMEOUT 0

/*
 * 4k is the smallest possible transfer unit, so restricting to 4k
 * avoids having to check the MDTS value of the controller.
 */
#define NVME2_LOG_PAGE_PDU_SIZE 4096

/*
 * should not exceed CAP.MQES, 16 is rational for most ssd
 */
#define NVME2_URING_ENTRIES 16

struct nvme2_handle {
	int fd;
};

/**
 * nvme_get_log_page() - Get log page data
 * @fd:		File descriptor of nvme device
 * @xfer_len:	Max log transfer size per request to split the total.
 * @args:	&struct nvme_get_log_args argument structure
 *
 * Return: The nvme command status if a response was received (see
 * &enum nvme_status_field) or -1 with errno set otherwise.
 */
int nvme2_get_log_page(struct nvme2_handle *hdl,
		       struct nvme2_common_command *cmd,
		       void *log, __u64 len, __u64 start,
		       __u32 xfer_len);

static inline int nvme2_get_nsid_log(struct nvme2_handle *hdl, bool rae,
				     enum nvme_cmd_get_log_lid lid,
				     __u32 nsid, __u32 len, void *log)
{
	struct nvme2_common_command cmd = { 0, };

	NVME_SET_FIELD(&cmd, LOG_NSID, nsid);
	NVME_SET_FIELD(&cmd, LOG_LID, lid);
	NVME_SET_FIELD(&cmd, LOG_LSP, NVME_LOG_LSP_NONE);
	NVME_SET_FIELD(&cmd, LOG_RAE, rae);
	NVME_SET_FIELD(&cmd, LOG_LSI, NVME_LOG_LSI_NONE);
	NVME_SET_FIELD(&cmd, LOG_UUID, NVME_UUID_NONE);
	NVME_SET_FIELD(&cmd, LOG_OT, false);
	NVME_SET_FIELD(&cmd, LOG_CSI, NVME_CSI_NVM);

	return nvme2_get_log_page(hdl, &cmd, log, len, 0, NVME2_LOG_PAGE_PDU_SIZE);
}

#endif // NVME2_IOCTL_H
