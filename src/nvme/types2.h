#ifndef NVME_TYPES2_H_
#define NVME_TYPES2_H_

#include "nvme/ioctl.h"
#include "nvme/tree.h"
#include <endian.h>

#include <linux/types.h>

enum nvme2_get_features_def {
	// CDW0 - Command Identification and Opcode
	NVME_FEAT_OPCODE_WORD = 0,
	NVME_FEAT_OPCODE_SHIFT = 0,
	NVME_FEAT_OPCODE_MASK = 0xff,

	NVME_FEAT_FUSE_WORD = 0,
	NVME_FEAT_FUSE_SHIFT = 8,
	NVME_FEAT_FUSE_MASK = 0x3,

	NVME_FEAT_CID_WORD = 0,
	NVME_FEAT_CID_SHIFT = 16,
	NVME_FEAT_CID_MASK = 0xffff,

	// CDW1 - Namespace Identifier
	NVME_FEAT_NSID_WORD = 1,
	NVME_FEAT_NSID_SHIFT = 0,
	NVME_FEAT_NSID_MASK = 0xffffffff,

	// CDW2 - Reserved
	NVME_FEAT_RESERVED2_WORD = 2,
	NVME_FEAT_RESERVED2_SHIFT = 0,
	NVME_FEAT_RESERVED2_MASK = 0xffffffff,

	// CDW3 - Reserved
	NVME_FEAT_RESERVED3_WORD = 3,
	NVME_FEAT_RESERVED3_SHIFT = 0,
	NVME_FEAT_RESERVED3_MASK = 0xffffffff,

	// CDW4-5 - Metadata Pointer (MPTR)
	NVME_FEAT_MPTR_LO_WORD = 4,
	NVME_FEAT_MPTR_LO_SHIFT = 0,
	NVME_FEAT_MPTR_LO_MASK = 0xffffffff,

	NVME_FEAT_MPTR_HI_WORD = 5,
	NVME_FEAT_MPTR_HI_SHIFT = 0,
	NVME_FEAT_MPTR_HI_MASK = 0xffffffff,

	// CDW6-7 - PRP1 (Physical Region Page 1)
	NVME_FEAT_PRP1_LO_WORD = 6,
	NVME_FEAT_PRP1_LO_SHIFT = 0,
	NVME_FEAT_PRP1_LO_MASK = 0xffffffff,

	NVME_FEAT_PRP1_HI_WORD = 7,
	NVME_FEAT_PRP1_HI_SHIFT = 0,
	NVME_FEAT_PRP1_HI_MASK = 0xffffffff,

	// CDW8-9 - PRP2 (Physical Region Page 2)
	NVME_FEAT_PRP2_LO_WORD = 8,
	NVME_FEAT_PRP2_LO_SHIFT = 0,
	NVME_FEAT_PRP2_LO_MASK = 0xffffffff,

	NVME_FEAT_PRP2_HI_WORD = 9,
	NVME_FEAT_PRP2_HI_SHIFT = 0,
	NVME_FEAT_PRP2_HI_MASK = 0xffffffff,

	// CDW10 - Feature Identifier (FID) and Select (SEL)
	NVME_FEAT_FID_WORD = 10,
	NVME_FEAT_FID_SHIFT = 0,
	NVME_FEAT_FID_MASK = 0xff,

	NVME_FEAT_SEL_WORD = 10,
	NVME_FEAT_SEL_SHIFT = 8,
	NVME_FEAT_SEL_MASK = 0x7,

	// CDW11 - Feature-Specific Parameter
	NVME_FEAT_CDW11_WORD = 11,
	NVME_FEAT_CDW11_SHIFT = 0,
	NVME_FEAT_CDW11_MASK = 0xffffffff,

	// CDW12 - Reserved
	NVME_FEAT_RESERVED12_WORD = 12,
	NVME_FEAT_RESERVED12_SHIFT = 0,
	NVME_FEAT_RESERVED12_MASK = 0xffffffff,

	// CDW13 - Reserved
	NVME_FEAT_RESERVED13_WORD = 13,
	NVME_FEAT_RESERVED13_SHIFT = 0,
	NVME_FEAT_RESERVED13_MASK = 0xffffffff,

	// CDW14 - Reserved
	NVME_FEAT_RESERVED14_WORD = 14,
	NVME_FEAT_RESERVED14_SHIFT = 0,
	NVME_FEAT_RESERVED14_MASK = 0xffffffff,

	// CDW15 - Reserved
	NVME_FEAT_RESERVED15_WORD = 15,
	NVME_FEAT_RESERVED15_SHIFT = 0,
	NVME_FEAT_RESERVED15_MASK = 0xffffffff,
};

enum nvme_get_log_def {
	// CDW0
	NVME_LOG_OPCODE_WORD        = 0,
	NVME_LOG_OPCODE_SHIFT       = 0,
	NVME_LOG_OPCODE_MASK        = 0xff,

	NVME_LOG_FLAGS_WORD         = 0,
	NVME_LOG_FLAGS_SHIFT        = 8,
	NVME_LOG_FLAGS_MASK         = 0xff,

	NVME_LOG_CID_WORD           = 0,
	NVME_LOG_CID_SHIFT          = 16,
	NVME_LOG_CID_MASK           = 0xffff,

	// CDW1
	NVME_LOG_NSID_WORD          = 1,
	NVME_LOG_NSID_SHIFT         = 0,
	NVME_LOG_NSID_MASK          = 0xffffffff,

	// CDW2
	NVME_LOG_RESERVED2_WORD     = 2,
	NVME_LOG_RESERVED2_SHIFT    = 0,
	NVME_LOG_RESERVED2_MASK     = 0xffffffff,

	// CDW3
	NVME_LOG_RESERVED3_WORD     = 3,
	NVME_LOG_RESERVED3_SHIFT    = 0,
	NVME_LOG_RESERVED3_MASK     = 0xffffffff,

	// CDW4–5: MPTR (metadata pointer, 64-bit)
	NVME_LOG_MPTR_LO_WORD       = 4,
	NVME_LOG_MPTR_LO_SHIFT      = 0,
	NVME_LOG_MPTR_LO_MASK       = 0xffffffff,

	NVME_LOG_MPTR_HI_WORD       = 5,
	NVME_LOG_MPTR_HI_SHIFT      = 0,
	NVME_LOG_MPTR_HI_MASK       = 0xffffffff,

	// CDW6–7: PRP1 (physical region page 1, 64-bit)
	NVME_LOG_PRP1_LO_WORD       = 6,
	NVME_LOG_PRP1_LO_SHIFT      = 0,
	NVME_LOG_PRP1_LO_MASK       = 0xffffffff,

	NVME_LOG_PRP1_HI_WORD       = 7,
	NVME_LOG_PRP1_HI_SHIFT      = 0,
	NVME_LOG_PRP1_HI_MASK       = 0xffffffff,

	// CDW8–9: PRP2 (physical region page 2, 64-bit)
	NVME_LOG_PRP2_LO_WORD       = 8,
	NVME_LOG_PRP2_LO_SHIFT      = 0,
	NVME_LOG_PRP2_LO_MASK       = 0xffffffff,

	NVME_LOG_PRP2_HI_WORD       = 9,
	NVME_LOG_PRP2_HI_SHIFT      = 0,
	NVME_LOG_PRP2_HI_MASK       = 0xffffffff,

	// CDW10: Get Log Parameters
	NVME_LOG_LID_WORD           = 10,
	NVME_LOG_LID_SHIFT          = 0,
	NVME_LOG_LID_MASK           = 0xff,

	NVME_LOG_LSP_WORD           = 10,
	NVME_LOG_LSP_SHIFT          = 8,
	NVME_LOG_LSP_MASK           = 0x7f,

	NVME_LOG_RAE_WORD           = 10,
	NVME_LOG_RAE_SHIFT          = 15,
	NVME_LOG_RAE_MASK           = 0x1,

	NVME_LOG_NUMDL_WORD         = 10,
	NVME_LOG_NUMDL_SHIFT        = 16,
	NVME_LOG_NUMDL_MASK         = 0xffff,

	// CDW11
	NVME_LOG_NUMDU_WORD         = 11,
	NVME_LOG_NUMDU_SHIFT        = 0,
	NVME_LOG_NUMDU_MASK         = 0xffff,

	NVME_LOG_LSI_WORD           = 11,
	NVME_LOG_LSI_SHIFT          = 16,
	NVME_LOG_LSI_MASK           = 0xffff,

	// CDW12
	NVME_LOG_LPO_HIGH_WORD      = 12,
	NVME_LOG_LPO_HIGH_SHIFT     = 0,
	NVME_LOG_LPO_HIGH_MASK      = 0xffffffff,

	// CDW13
	NVME_LOG_LPO_LOW_WORD       = 13,
	NVME_LOG_LPO_LOW_SHIFT      = 0,
	NVME_LOG_LPO_LOW_MASK       = 0xffffffff,

	// CDW14
	NVME_LOG_UUID_WORD          = 14,
	NVME_LOG_UUID_SHIFT         = 0,
	NVME_LOG_UUID_MASK          = 0x7f,

	NVME_LOG_OT_WORD            = 14,
	NVME_LOG_OT_SHIFT           = 23,
	NVME_LOG_OT_MASK            = 0x1,

	NVME_LOG_CSI_WORD           = 14,
	NVME_LOG_CSI_SHIFT          = 24,
	NVME_LOG_CSI_MASK           = 0xff,

	// CDW15
	NVME_LOG_RESERVED15_WORD    = 15,
	NVME_LOG_RESERVED15_SHIFT   = 0,
	NVME_LOG_RESERVED15_MASK    = 0xffffffff,
};

enum nvme_identify_def {
	// CDW0 - Command Identification and Opcode
	NVME_IDENTIFY_OPCODE_WORD        = 0,
	NVME_IDENTIFY_OPCODE_SHIFT       = 0,
	NVME_IDENTIFY_OPCODE_MASK        = 0xff,

	NVME_IDENTIFY_FUSE_WORD          = 0,
	NVME_IDENTIFY_FUSE_SHIFT         = 8,
	NVME_IDENTIFY_FUSE_MASK          = 0x3,

	NVME_IDENTIFY_CID_WORD           = 0,
	NVME_IDENTIFY_CID_SHIFT          = 16,
	NVME_IDENTIFY_CID_MASK           = 0xffff,

	// CDW1 - Namespace Identifier
	NVME_IDENTIFY_NSID_WORD          = 1,
	NVME_IDENTIFY_NSID_SHIFT         = 0,
	NVME_IDENTIFY_NSID_MASK          = 0xffffffff,

	// CDW2 - Reserved
	NVME_IDENTIFY_RESERVED2_WORD     = 2,
	NVME_IDENTIFY_RESERVED2_SHIFT    = 0,
	NVME_IDENTIFY_RESERVED2_MASK     = 0xffffffff,

	// CDW3 - Reserved
	NVME_IDENTIFY_RESERVED3_WORD     = 3,
	NVME_IDENTIFY_RESERVED3_SHIFT    = 0,
	NVME_IDENTIFY_RESERVED3_MASK     = 0xffffffff,

	// CDW4-5 - Metadata Pointer (MPTR)
	NVME_IDENTIFY_MPTR_LO_WORD       = 4,
	NVME_IDENTIFY_MPTR_LO_SHIFT      = 0,
	NVME_IDENTIFY_MPTR_LO_MASK       = 0xffffffff,

	NVME_IDENTIFY_MPTR_HI_WORD       = 5,
	NVME_IDENTIFY_MPTR_HI_SHIFT      = 0,
	NVME_IDENTIFY_MPTR_HI_MASK       = 0xffffffff,

	// CDW6-7 - PRP1 (Physical Region Page 1)
	NVME_IDENTIFY_PRP1_LO_WORD       = 6,
	NVME_IDENTIFY_PRP1_LO_SHIFT      = 0,
	NVME_IDENTIFY_PRP1_LO_MASK       = 0xffffffff,

	NVME_IDENTIFY_PRP1_HI_WORD       = 7,
	NVME_IDENTIFY_PRP1_HI_SHIFT      = 0,
	NVME_IDENTIFY_PRP1_HI_MASK       = 0xffffffff,

	// CDW8-9 - PRP2 (Physical Region Page 2)
	NVME_IDENTIFY_PRP2_LO_WORD       = 8,
	NVME_IDENTIFY_PRP2_LO_SHIFT      = 0,
	NVME_IDENTIFY_PRP2_LO_MASK       = 0xffffffff,

	NVME_IDENTIFY_PRP2_HI_WORD       = 9,
	NVME_IDENTIFY_PRP2_HI_SHIFT      = 0,
	NVME_IDENTIFY_PRP2_HI_MASK       = 0xffffffff,

	// CDW10 - Command-specific: CNS (Controller or Namespace Structure)
	NVME_IDENTIFY_CNS_WORD           = 10,
	NVME_IDENTIFY_CNS_SHIFT          = 0,
	NVME_IDENTIFY_CNS_MASK           = 0xff,

	// CDW10 - Command-specific: CNTID (Controller Identifier)
	NVME_IDENTIFY_CNTID_WORD         = 10,
	NVME_IDENTIFY_CNTID_SHIFT        = 16,
	NVME_IDENTIFY_CNTID_MASK         = 0xffff,

	// CDW11 - Command-specific: CNS-specific parameters
	NVME_IDENTIFY_CDW11_WORD         = 11,
	NVME_IDENTIFY_CDW11_SHIFT        = 0,
	NVME_IDENTIFY_CDW11_MASK         = 0xffffffff,

	// CDW12 - Reserved
	NVME_IDENTIFY_RESERVED12_WORD    = 12,
	NVME_IDENTIFY_RESERVED12_SHIFT   = 0,
	NVME_IDENTIFY_RESERVED12_MASK    = 0xffffffff,

	// CDW13 - Reserved
	NVME_IDENTIFY_RESERVED13_WORD    = 13,
	NVME_IDENTIFY_RESERVED13_SHIFT   = 0,
	NVME_IDENTIFY_RESERVED13_MASK    = 0xffffffff,

	// CDW14 - Reserved
	NVME_IDENTIFY_RESERVED14_WORD    = 14,
	NVME_IDENTIFY_RESERVED14_SHIFT   = 0,
	NVME_IDENTIFY_RESERVED14_MASK    = 0xffffffff,

	// CDW15 - Reserved
	NVME_IDENTIFY_RESERVED15_WORD    = 15,
	NVME_IDENTIFY_RESERVED15_SHIFT   = 0,
	NVME_IDENTIFY_RESERVED15_MASK    = 0xffffffff,
};

struct nvme2_common_command {
	union {
		__le32 words[16];
		struct nvme_passthru_cmd cmd;
		struct nvme_passthru_cmd64 cmd64;
	};
};

#define NVME_SET_FIELD(cmd, name, value)					\
	(cmd)->words[NVME_##name##_WORD] =					\
		htole32(							\
                        (le32toh((cmd)->words[NVME_##name##_WORD]) &		\
                         ~(NVME_##name##_MASK << NVME_##name##_SHIFT)) |	\
                        (((value) & NVME_##name##_MASK) << NVME_##name##_SHIFT))

#define NVME_GET_FIELD(cmd, name)						\
	((le32toh((cmd)->words[NVME_##name##_WORD]) >> NVME_##name##_SHIFT) & NVME_##name##_MASK)

#endif // NVME_TYPES2_H_
