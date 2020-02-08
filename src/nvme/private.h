#ifndef _LIBNVME_PRIVATE_H
#define _LIBNVME_PRIVATE_H

#include <dirent.h>
#include <stdint.h>
#include <stdlib.h>

#include <ccan/list/list.h>

#include "tree.h"

struct nvme_path {
	struct list_node entry;
	struct list_node nentry;

	struct nvme_ctrl *c;
	struct nvme_ns *n;

	char *name;
	char *sysfs_dir;
	char *ana_state;
	int grpid;
};

struct nvme_ns {
	struct list_node entry;
	struct list_head paths;

	struct nvme_subsystem *s;
	struct nvme_ctrl *c;

	int fd;
	char *name;
	char *sysfs_dir;
	int nsid;

	int lba_size;
	int meta_size;
	uint64_t lba_count;
	uint64_t lba_util;
};

struct nvme_ctrl {
	struct list_node entry;
	struct list_head paths;
	struct list_head namespaces;

	struct nvme_subsystem *s;

	int fd;
	char *name;
	char *sysfs_dir;
	char *address;
	char *firmware;
	char *model;
	char *state;
	char *numa_node;
	char *queue_count;
	char *serial;
	char *sqsize;
	char *transport;
	char *subsysnqn;
};

struct nvme_subsystem {
	struct list_node entry;
	struct list_head ctrls;
	struct list_head namespaces;
	struct nvme_root *r;

	char *name;
	char *sysfs_dir;
	char *subsysnqn;
};

struct nvme_root {
	struct list_head subsystems;
};

void nvme_free_ctrl(struct nvme_ctrl *c);
void nvme_ctrl_free_ns(struct nvme_ns *n);
void nvme_subsystem_free_ns(struct nvme_ns *n);
void nvme_free_path(struct nvme_path *p);
void nvme_free_subsystem(struct nvme_subsystem *s);

int nvme_scan_subsystem(struct nvme_root *t, char *name, nvme_scan_filter_t f);
int nvme_subsystem_scan_namespace(struct nvme_subsystem *s, char *name);
int nvme_subsystem_scan_ctrls(struct nvme_subsystem *s);
int nvme_subsystem_scan_ctrl(struct nvme_subsystem *s, char *name);

int nvme_ctrl_scan_namespace(struct nvme_ctrl *c, char *name);
int nvme_ctrl_scan_path(struct nvme_ctrl *c, char *name);

static inline void nvme_free_dirents(struct dirent **d, int i)
{
	while (i-- > 0)
		free(d[i]);
	free(d);
}

#endif /* _LIBNVME_PRIVATE_H */