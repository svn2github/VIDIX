/*
   rhd_vid - VIDIX based video driver for RadeonHD chips.
   Copyrights 2008 Nickols_K.
   Licence: GPL
*/

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <inttypes.h>
#include <sys/mman.h>
#include "config.h"
#include "bswap.h"
#include "libdha/pci_ids.h"
#include "libdha/pci_names.h"
#include "vidix.h"
#include "fourcc.h"
#include "libdha/libdha.h"
#include "radeonhd.h"

#define RADEON_MSG "RadeonHD_vid:"
#define RADEON_ASSERT(msg) printf(RADEON_MSG"################# FATAL:"msg);

#define VIDIX_STATIC r600_

//#undef RADEON_ENABLE_BM /* unfinished stuff. May corrupt your filesystem ever */
#undef RADEON_ENABLE_BM

#ifdef RADEON_ENABLE_BM
static void * radeon_dma_desc_base = 0;
static unsigned long bus_addr_dma_desc = 0;
static unsigned long *dma_phys_addrs = 0;
#pragma pack(1)
typedef struct
{
	uint32_t framebuf_offset;
	uint32_t sys_addr;
	uint32_t command;
	uint32_t reserved;
} bm_list_descriptor;
#pragma pack()
#endif

#define VERBOSE_LEVEL 0
static int __verbose = 0;

typedef struct ovl_registers_s {
uint32_t CRTC_H_TOTAL;
uint32_t CRTC_V_TOTAL;
uint32_t GRPH_ENABLE;
uint32_t GRPH_CONTOL;
uint32_t GRPH_PRIMARY_SURFACE_ADDRESS;
uint32_t GRPH_SECONDARY_SURFACE_ADDRESS;
uint32_t OVL_ENABLE;
uint32_t OVL_CONTROL1;
uint32_t OVL_CONTROL2;
uint32_t OVL_SWAP_CNTL;
uint32_t OVL_SURFACE_ADDRESS;
uint32_t OVL_PITCH;
uint32_t OVL_SURFACE_OFFSET_X;
uint32_t OVL_SURFACE_OFFSET_Y;
uint32_t OVL_START;
uint32_t OVL_END;
uint32_t OVL_UPDATE;
uint32_t OVL_SURFACE_ADDRESS_INUSE;
uint32_t OVL_DFQ_CONTROL;
uint32_t OVL_DFQ_STATUS;
uint32_t OVL_COLOR_MATIX_TANSFORM_CNTL;
uint32_t OVL_COLOR_MATIX_TANSFORM_EN;
uint32_t OVL_MATRIX_COEFF_1_1;
uint32_t OVL_MATRIX_COEFF_1_2;
uint32_t OVL_MATRIX_COEFF_1_3;
uint32_t OVL_MATRIX_COEFF_1_4;
uint32_t OVL_MATRIX_COEFF_2_1;
uint32_t OVL_MATRIX_COEFF_2_2;
uint32_t OVL_MATRIX_COEFF_2_3;
uint32_t OVL_MATRIX_COEFF_2_4;
uint32_t OVL_MATRIX_COEFF_3_1;
uint32_t OVL_MATRIX_COEFF_3_2;
uint32_t OVL_MATRIX_COEFF_3_3;
uint32_t OVL_MATRIX_COEFF_3_4;
uint32_t OVL_PWL_TRANSFORM_EN;
uint32_t OVL_0TOF;
uint32_t OVL_10TO1F;
uint32_t OVL_20TO3F;
uint32_t OVL_40TO7F;
uint32_t OVL_80TOBF;
uint32_t OVL_C0TOFF;
uint32_t OVL_100TO13F;
uint32_t OVL_140TO17F;
uint32_t OVL_180TO1BF;
uint32_t OVL_1C0TO1FF;
uint32_t OVL_200TO23F;
uint32_t OVL_240TO27F;
uint32_t OVL_280TO2BF;
uint32_t OVL_2C0TO2FF;
uint32_t OVL_300TO3FF;
uint32_t OVL_340TO37F;
uint32_t OVL_380TO3BF;
uint32_t OVL_3C0TO3FF;
uint32_t OVL_KEY_CONTOL;
uint32_t GRPH_ALPHA;
uint32_t OVL_ALPHA;
uint32_t OVL_ALPHA_CONTROL;
uint32_t GRPH_KEY_RANGE_RED;
uint32_t GRPH_KEY_RANGE_GREEN;
uint32_t GRPH_KEY_RANGE_BLUE;
uint32_t GRPH_KEY_RANGE_ALPHA;
uint32_t OVL_KEY_RANGE_RED_CR;
uint32_t OVL_KEY_RANGE_GREEN_Y;
uint32_t OVL_KEY_RANGE_BLUE_CB;
uint32_t OVL_KEY_RANGE_ALPHA;
}ovl_registers_t;
static ovl_registers_t ovl;

static void route_secondary(void) {
    ovl.CRTC_H_TOTAL=RHD_D1CRTC_H_TOTAL;
    ovl.CRTC_V_TOTAL=RHD_D1CRTC_V_TOTAL;
    ovl.GRPH_ENABLE=RHD_D1GRPH_ENABLE;
    ovl.GRPH_CONTOL=RHD_D1GRPH_CONTOL;
    ovl.GRPH_PRIMARY_SURFACE_ADDRESS=RHD_D1GRPH_PRIMARY_SURFACE_ADDRESS;
    ovl.GRPH_SECONDARY_SURFACE_ADDRESS=RHD_D1GRPH_SECONDARY_SURFACE_ADDRESS;
    ovl.OVL_ENABLE=RHD_D1OVL_ENABLE;
    ovl.OVL_CONTROL1=RHD_D1OVL_CONTROL1;
    ovl.OVL_CONTROL2=RHD_D1OVL_CONTROL2;
    ovl.OVL_SWAP_CNTL=RHD_D1OVL_SWAP_CNTL;
    ovl.OVL_SURFACE_ADDRESS=RHD_D1OVL_SURFACE_ADDRESS;
    ovl.OVL_PITCH=RHD_D1OVL_PITCH;
    ovl.OVL_SURFACE_OFFSET_X=RHD_D1OVL_SURFACE_OFFSET_X;
    ovl.OVL_SURFACE_OFFSET_Y=RHD_D1OVL_SURFACE_OFFSET_Y;
    ovl.OVL_START=RHD_D1OVL_START;
    ovl.OVL_END=RHD_D1OVL_END;
    ovl.OVL_UPDATE=RHD_D1OVL_UPDATE;
    ovl.OVL_SURFACE_ADDRESS_INUSE=RHD_D1OVL_SURFACE_ADDRESS_INUSE;
    ovl.OVL_DFQ_CONTROL=RHD_D1OVL_DFQ_CONTROL;
    ovl.OVL_DFQ_STATUS=RHD_D1OVL_DFQ_STATUS;
    ovl.OVL_COLOR_MATIX_TANSFORM_CNTL=RHD_D1OVL_COLOR_MATIX_TANSFORM_CNTL;
    ovl.OVL_COLOR_MATIX_TANSFORM_EN=RHD_D1OVL_COLOR_MATIX_TANSFORM_EN;
    ovl.OVL_MATRIX_COEFF_1_1=RHD_D1OVL_MATRIX_COEFF_1_1;
    ovl.OVL_MATRIX_COEFF_1_2=RHD_D1OVL_MATRIX_COEFF_1_2;
    ovl.OVL_MATRIX_COEFF_1_3=RHD_D1OVL_MATRIX_COEFF_1_3;
    ovl.OVL_MATRIX_COEFF_1_4=RHD_D1OVL_MATRIX_COEFF_1_4;
    ovl.OVL_MATRIX_COEFF_2_1=RHD_D1OVL_MATRIX_COEFF_2_1;
    ovl.OVL_MATRIX_COEFF_2_2=RHD_D1OVL_MATRIX_COEFF_2_2;
    ovl.OVL_MATRIX_COEFF_2_3=RHD_D1OVL_MATRIX_COEFF_2_3;
    ovl.OVL_MATRIX_COEFF_2_4=RHD_D1OVL_MATRIX_COEFF_2_4;
    ovl.OVL_MATRIX_COEFF_3_1=RHD_D1OVL_MATRIX_COEFF_3_1;
    ovl.OVL_MATRIX_COEFF_3_2=RHD_D1OVL_MATRIX_COEFF_3_2;
    ovl.OVL_MATRIX_COEFF_3_3=RHD_D1OVL_MATRIX_COEFF_3_3;
    ovl.OVL_MATRIX_COEFF_3_4=RHD_D1OVL_MATRIX_COEFF_3_4;
    ovl.OVL_PWL_TRANSFORM_EN=RHD_D1OVL_PWL_TRANSFORM_EN;
    ovl.OVL_0TOF=RHD_D1OVL_0TOF;
    ovl.OVL_10TO1F=RHD_D1OVL_10TO1F;
    ovl.OVL_20TO3F=RHD_D1OVL_20TO3F;
    ovl.OVL_40TO7F=RHD_D1OVL_40TO7F;
    ovl.OVL_80TOBF=RHD_D1OVL_80TOBF;
    ovl.OVL_C0TOFF=RHD_D1OVL_C0TOFF;
    ovl.OVL_100TO13F=RHD_D1OVL_100TO13F;
    ovl.OVL_140TO17F=RHD_D1OVL_140TO17F;
    ovl.OVL_180TO1BF=RHD_D1OVL_180TO1BF;
    ovl.OVL_1C0TO1FF=RHD_D1OVL_1C0TO1FF;
    ovl.OVL_200TO23F=RHD_D1OVL_200TO23F;
    ovl.OVL_240TO27F=RHD_D1OVL_240TO27F;
    ovl.OVL_280TO2BF=RHD_D1OVL_280TO2BF;
    ovl.OVL_2C0TO2FF=RHD_D1OVL_2C0TO2FF;
    ovl.OVL_300TO3FF=RHD_D1OVL_300TO3FF;
    ovl.OVL_340TO37F=RHD_D1OVL_340TO37F;
    ovl.OVL_380TO3BF=RHD_D1OVL_380TO3BF;
    ovl.OVL_3C0TO3FF=RHD_D1OVL_3C0TO3FF;
    ovl.OVL_KEY_CONTOL=RHD_D1OVL_KEY_CONTOL;
    ovl.GRPH_ALPHA=RHD_D1GRPH_ALPHA;
    ovl.OVL_ALPHA=RHD_D1OVL_ALPHA;
    ovl.OVL_ALPHA_CONTROL=RHD_D1OVL_ALPHA_CONTROL;
    ovl.GRPH_KEY_RANGE_RED=RHD_D1GRPH_KEY_RANGE_RED;
    ovl.GRPH_KEY_RANGE_GREEN=RHD_D1GRPH_KEY_RANGE_GREEN;
    ovl.GRPH_KEY_RANGE_BLUE=RHD_D1GRPH_KEY_RANGE_BLUE;
    ovl.GRPH_KEY_RANGE_ALPHA=RHD_D1GRPH_KEY_RANGE_ALPHA;
    ovl.OVL_KEY_RANGE_RED_CR=RHD_D1OVL_KEY_RANGE_RED_CR;
    ovl.OVL_KEY_RANGE_GREEN_Y=RHD_D1OVL_KEY_RANGE_GREEN_Y;
    ovl.OVL_KEY_RANGE_BLUE_CB=RHD_D1OVL_KEY_RANGE_BLUE_CB;
    ovl.OVL_KEY_RANGE_ALPHA=RHD_D1OVL_KEY_RANGE_ALPHA;
}

static void route_primary(void) {
    ovl.CRTC_H_TOTAL=RHD_D2CRTC_H_TOTAL;
    ovl.CRTC_V_TOTAL=RHD_D2CRTC_V_TOTAL;
    ovl.GRPH_ENABLE=RHD_D2GRPH_ENABLE;
    ovl.GRPH_CONTOL=RHD_D2GRPH_CONTOL;
    ovl.GRPH_PRIMARY_SURFACE_ADDRESS=RHD_D2GRPH_PRIMARY_SURFACE_ADDRESS;
    ovl.GRPH_SECONDARY_SURFACE_ADDRESS=RHD_D2GRPH_SECONDARY_SURFACE_ADDRESS;
    ovl.OVL_ENABLE=RHD_D2OVL_ENABLE;
    ovl.OVL_CONTROL1=RHD_D2OVL_CONTROL1;
    ovl.OVL_CONTROL2=RHD_D2OVL_CONTROL2;
    ovl.OVL_SWAP_CNTL=RHD_D2OVL_SWAP_CNTL;
    ovl.OVL_SURFACE_ADDRESS=RHD_D2OVL_SURFACE_ADDRESS;
    ovl.OVL_PITCH=RHD_D2OVL_PITCH;
    ovl.OVL_SURFACE_OFFSET_X=RHD_D2OVL_SURFACE_OFFSET_X;
    ovl.OVL_SURFACE_OFFSET_Y=RHD_D2OVL_SURFACE_OFFSET_Y;
    ovl.OVL_START=RHD_D2OVL_START;
    ovl.OVL_END=RHD_D2OVL_END;
    ovl.OVL_UPDATE=RHD_D2OVL_UPDATE;
    ovl.OVL_SURFACE_ADDRESS_INUSE=RHD_D2OVL_SURFACE_ADDRESS_INUSE;
    ovl.OVL_DFQ_CONTROL=RHD_D2OVL_DFQ_CONTROL;
    ovl.OVL_DFQ_STATUS=RHD_D2OVL_DFQ_STATUS;
    ovl.OVL_COLOR_MATIX_TANSFORM_CNTL=RHD_D2OVL_COLOR_MATIX_TANSFORM_CNTL;
    ovl.OVL_COLOR_MATIX_TANSFORM_EN=RHD_D2OVL_COLOR_MATIX_TANSFORM_EN;
    ovl.OVL_MATRIX_COEFF_1_1=RHD_D2OVL_MATRIX_COEFF_1_1;
    ovl.OVL_MATRIX_COEFF_1_2=RHD_D2OVL_MATRIX_COEFF_1_2;
    ovl.OVL_MATRIX_COEFF_1_3=RHD_D2OVL_MATRIX_COEFF_1_3;
    ovl.OVL_MATRIX_COEFF_1_4=RHD_D2OVL_MATRIX_COEFF_1_4;
    ovl.OVL_MATRIX_COEFF_2_1=RHD_D2OVL_MATRIX_COEFF_2_1;
    ovl.OVL_MATRIX_COEFF_2_2=RHD_D2OVL_MATRIX_COEFF_2_2;
    ovl.OVL_MATRIX_COEFF_2_3=RHD_D2OVL_MATRIX_COEFF_2_3;
    ovl.OVL_MATRIX_COEFF_2_4=RHD_D2OVL_MATRIX_COEFF_2_4;
    ovl.OVL_MATRIX_COEFF_3_1=RHD_D2OVL_MATRIX_COEFF_3_1;
    ovl.OVL_MATRIX_COEFF_3_2=RHD_D2OVL_MATRIX_COEFF_3_2;
    ovl.OVL_MATRIX_COEFF_3_3=RHD_D2OVL_MATRIX_COEFF_3_3;
    ovl.OVL_MATRIX_COEFF_3_4=RHD_D2OVL_MATRIX_COEFF_3_4;
    ovl.OVL_PWL_TRANSFORM_EN=RHD_D2OVL_PWL_TRANSFORM_EN;
    ovl.OVL_0TOF=RHD_D2OVL_0TOF;
    ovl.OVL_10TO1F=RHD_D2OVL_10TO1F;
    ovl.OVL_20TO3F=RHD_D2OVL_20TO3F;
    ovl.OVL_40TO7F=RHD_D2OVL_40TO7F;
    ovl.OVL_80TOBF=RHD_D2OVL_80TOBF;
    ovl.OVL_C0TOFF=RHD_D2OVL_C0TOFF;
    ovl.OVL_100TO13F=RHD_D2OVL_100TO13F;
    ovl.OVL_140TO17F=RHD_D2OVL_140TO17F;
    ovl.OVL_180TO1BF=RHD_D2OVL_180TO1BF;
    ovl.OVL_1C0TO1FF=RHD_D2OVL_1C0TO1FF;
    ovl.OVL_200TO23F=RHD_D2OVL_200TO23F;
    ovl.OVL_240TO27F=RHD_D2OVL_240TO27F;
    ovl.OVL_280TO2BF=RHD_D2OVL_280TO2BF;
    ovl.OVL_2C0TO2FF=RHD_D2OVL_2C0TO2FF;
    ovl.OVL_300TO3FF=RHD_D2OVL_300TO3FF;
    ovl.OVL_340TO37F=RHD_D2OVL_340TO37F;
    ovl.OVL_380TO3BF=RHD_D2OVL_380TO3BF;
    ovl.OVL_3C0TO3FF=RHD_D2OVL_3C0TO3FF;
    ovl.OVL_KEY_CONTOL=RHD_D2OVL_KEY_CONTOL;
    ovl.GRPH_ALPHA=RHD_D2GRPH_ALPHA;
    ovl.OVL_ALPHA=RHD_D2OVL_ALPHA;
    ovl.OVL_ALPHA_CONTROL=RHD_D2OVL_ALPHA_CONTROL;
    ovl.GRPH_KEY_RANGE_RED=RHD_D2GRPH_KEY_RANGE_RED;
    ovl.GRPH_KEY_RANGE_GREEN=RHD_D2GRPH_KEY_RANGE_GREEN;
    ovl.GRPH_KEY_RANGE_BLUE=RHD_D2GRPH_KEY_RANGE_BLUE;
    ovl.GRPH_KEY_RANGE_ALPHA=RHD_D2GRPH_KEY_RANGE_ALPHA;
    ovl.OVL_KEY_RANGE_RED_CR=RHD_D2OVL_KEY_RANGE_RED_CR;
    ovl.OVL_KEY_RANGE_GREEN_Y=RHD_D2OVL_KEY_RANGE_GREEN_Y;
    ovl.OVL_KEY_RANGE_BLUE_CB=RHD_D2OVL_KEY_RANGE_BLUE_CB;
    ovl.OVL_KEY_RANGE_ALPHA=RHD_D2OVL_KEY_RANGE_ALPHA;
}

typedef struct bes_registers_s
{
  /* base address of yuv framebuffer */
  uint32_t fourcc;
  uint32_t control1;
  uint32_t control2;
  uint32_t swap_cntl;
  uint32_t dest_bpp;
  /* YUV BES registers */
  uint32_t y_x_start;
  uint32_t y_x_end;
  uint32_t vid_buf_pitch0_value;
  uint32_t vid_buf_pitch1_value;
  uint32_t base_addr;
  uint32_t vid_buf_base_adrs_y[VID_PLAY_MAXFRAMES];
  uint32_t vid_buf_base_adrs_u[VID_PLAY_MAXFRAMES];
  uint32_t vid_buf_base_adrs_v[VID_PLAY_MAXFRAMES];
  uint32_t vid_nbufs;

  uint32_t key_cntl;
  uint32_t test;
  /* Configurable stuff */
  int double_buff;
  
  int brightness;
  int saturation;
  
  int ckey_on;
  uint32_t graphics_key_clr;
  uint32_t graphics_key_msk;
  uint32_t ckey_cntl;
  uint32_t merge_cntl;
  
  int deinterlace_on;
  uint32_t deinterlace_pattern;
  unsigned chip_flags;
} bes_registers_t;

typedef struct video_registers_s
{
  const char * sname;
  uint32_t name;
  uint32_t value;
}video_registers_t;

static bes_registers_t besr;
#define DECLARE_VREG(name) { #name, name, 0 }
static video_registers_t vregs[] = 
{
};

typedef struct ati_card_ids_s
{
    unsigned short id;
    unsigned flags;
}ati_card_ids_t;

#define R_5X0		0x00000010
#define R_6X0		0x00000020
#define R_7X0		0x00000040
#define R_PCIE		0x04000000

static const ati_card_ids_t ati_card_ids[] =
{
 /* R500 */
 { DEVICE_ATI_RV505_RADEON_X1550,	R_5X0|R_PCIE  },
 { DEVICE_ATI_RV505_RADEON_X15502,	R_5X0|R_PCIE  },
 { DEVICE_ATI_RV505_CE_RADEON,		R_5X0|R_PCIE  },
 { DEVICE_ATI_RV505_RADEON_X15503,	R_5X0|R_PCIE  },
 { DEVICE_ATI_RV515_RADEON_X1300,	R_5X0|R_PCIE  },
 { DEVICE_ATI_RV515_RADEON_X13002,	R_5X0|R_PCIE  },
 { DEVICE_ATI_RV515LE_RADEON_X13002,	R_5X0|R_PCIE  },
 { DEVICE_ATI_RV515_RADEON_X13004,	R_5X0|R_PCIE  },
 { DEVICE_ATI_RV515LE_RADEON_X1300,	R_5X0|R_PCIE  },
 { DEVICE_ATI_RV515GL_FIREGL_V3300,	R_5X0|R_PCIE  },
 { DEVICE_ATI_RV515GL_FIREGL_V33002,	R_5X0|R_PCIE  },
 { DEVICE_ATI_RV515GL_FIREGL_V3350,	R_5X0|R_PCIE  },
 { DEVICE_ATI_RV515GL_FIREGL_V33502,	R_5X0|R_PCIE  },
 { DEVICE_ATI_RV515_RADEON_X13003,	R_5X0|R_PCIE  },
 { DEVICE_ATI_RV515_RADEON_X1600,	R_5X0|R_PCIE  },
 { DEVICE_ATI_RV515_PRO_RADEON,		R_5X0|R_PCIE  },
 { DEVICE_ATI_RV515_PRO_RADEON2,	R_5X0|R_PCIE  },
 { DEVICE_ATI_RADEON_MOBILITY_X1400,	R_5X0|R_PCIE  },
 { DEVICE_ATI_RV515_RADEON_MOBILITY,	R_5X0|R_PCIE  },
 { DEVICE_ATI_RV515_RADEON_X1300,	R_5X0|R_PCIE  },
 { DEVICE_ATI_M52_MOBILITY_RADEON,	R_5X0|R_PCIE  },
 { DEVICE_ATI_M52_MOBILITY_RADEON2,	R_5X0|R_PCIE  },
 { DEVICE_ATI_M52_MOBILITY_RADEON3,	R_5X0|R_PCIE  },
 { DEVICE_ATI_M52_MOBILITY_RADEON4,	R_5X0|R_PCIE  },
 { DEVICE_ATI_RV516_RADEON_X1300_X1550,	R_5X0|R_PCIE  },
 { DEVICE_ATI_RV516_RADEON_X1300_X15502,	R_5X0|R_PCIE  },
 { DEVICE_ATI_RV516_RADEON_X1300_X15503,	R_5X0|R_PCIE  },
 { DEVICE_ATI_RV516_RADEON_X1300_X15504,	R_5X0|R_PCIE  },
 { DEVICE_ATI_RV516_XT_RADEON,		R_5X0|R_PCIE  },
 { DEVICE_ATI_RV516_RADEON_X1550,	R_5X0|R_PCIE  },
 { DEVICE_ATI_RV516_MOBILITY_RADEON,	R_5X0|R_PCIE  },
 { DEVICE_ATI_RV516LE_RADEON_X1550,	R_5X0|R_PCIE  },
 { DEVICE_ATI_RV516_XT_RADEON2,		R_5X0|R_PCIE  },
 { DEVICE_ATI_RV516_RADEON_X1300,	R_5X0|R_PCIE  },
 { DEVICE_ATI_RV516_RADEON_X1300_X15505,	R_5X0|R_PCIE  },
 { DEVICE_ATI_R520GL_FIREGL_V7200,	R_5X0  },
 { DEVICE_ATI_R520_FIREGL,		R_5X0  },
 { DEVICE_ATI_FIREMV_2250,		R_5X0  },
 { DEVICE_ATI_FIREMV_2250_SECONDARY,	R_5X0  },
 { DEVICE_ATI_M56P_RADEON_MOBILITY,	R_5X0  },
 { DEVICE_ATI_M56GL_MOBILITY_FIREGL,	R_5X0  },
 { DEVICE_ATI_M56GL_MOBILITY_FIREGL2,	R_5X0  },
 { DEVICE_ATI_M58_MOBILITY_FIREGL2,	R_5X0  },
 { DEVICE_ATI_M58_RADEON_MOBILITY2,	R_5X0  },
 { DEVICE_ATI_M64_S_MOBILITY_RADEON,	R_5X0  },
 { DEVICE_ATI_MOBILITY_RADEON_X2300,	R_5X0  },
 { DEVICE_ATI_M62CSP64_MOBILITY_RADEON,	R_5X0  },
 { DEVICE_ATI_M64CSP128_MOBILITY_RADEON,	R_5X0  },
 { DEVICE_ATI_M66_P_MOBILITY_RADEON,	R_5X0  },
 { DEVICE_ATI_M66_XT_MOBILITY_RADEON,	R_5X0  },
 { DEVICE_ATI_M71_MOBILITY_RADEON,	R_5X0  },
 { DEVICE_ATI_M71_MOBILITY_RADEON2,	R_5X0  },
 { DEVICE_ATI_R520_RADEON_X18002,	R_5X0  },
 { DEVICE_ATI_R520_RADEON_X18003,	R_5X0  },
 { DEVICE_ATI_R520_RADEON_X18004,	R_5X0  },
 { DEVICE_ATI_R520_RADEON_X18005,	R_5X0  },
 { DEVICE_ATI_R520_RADEON_X18006,	R_5X0  },
 { DEVICE_ATI_R520_RADEON_X18007,	R_5X0  },
 { DEVICE_ATI_R520GL_FIREGL_V72002,	R_5X0  },
 { DEVICE_ATI_RV530_RADEON_X1600,	R_5X0  },
 { DEVICE_ATI_RV530_RADEON_X16002,	R_5X0  },
 { DEVICE_ATI_RV530_RADEON_X16003,	R_5X0  },
 { DEVICE_ATI_RV530_RADEON_X16004,	R_5X0  },
 { DEVICE_ATI_RADEON_X1650_PRO,		R_5X0  },
 { DEVICE_ATI_RV530LE_RADEON_X1600_X1650,	R_5X0  },
 { DEVICE_ATI_RV535_RADEON_X1650,	R_5X0  },
 { DEVICE_ATI_RV530LE_RADEON_X1600,	R_5X0  },
 { DEVICE_ATI_RV530LE_RADEON_X16002,	R_5X0  },
 { DEVICE_ATI_RADEON_X1650_PRO2,	R_5X0  },
 { DEVICE_ATI_RV530LE_RADEON_X1650,	R_5X0  },
 { DEVICE_ATI_RV535_RADEON_X16502,	R_5X0  },
 { DEVICE_ATI_RV570_RADEON_X19502,	R_5X0  },
 { DEVICE_ATI_R580_RADEON_X1900,	R_5X0  },
 { DEVICE_ATI_R580_RADEON_X19002,	R_5X0  },
 { DEVICE_ATI_R580_RADEON_X19003,	R_5X0  },
 { DEVICE_ATI_R580_RADEON_X19004,	R_5X0  },
 { DEVICE_ATI_R580_RADEON_X19005,	R_5X0  },
 { DEVICE_ATI_R580_RADEON_X19006,	R_5X0  },
 { DEVICE_ATI_R580_RADEON_X19007,	R_5X0  },
 { DEVICE_ATI_R580_RADEON_X19008,	R_5X0  },
 { DEVICE_ATI_R580_RADEON_X19009,	R_5X0  },
 { DEVICE_ATI_R580_RADEON_X190010,	R_5X0  },
 { DEVICE_ATI_R580_RADEON_X190011,	R_5X0  },
 { DEVICE_ATI_R580_RADEON_X190012,	R_5X0  },
 { DEVICE_ATI_R580_RADEON_X190013,	R_5X0  },
 { DEVICE_ATI_R580_RADEON_X190014,	R_5X0  },
 { DEVICE_ATI_R580_RADEON_X190015,	R_5X0  },
 { DEVICE_ATI_R580_RADEON_X190016,	R_5X0  },
 { DEVICE_ATI_R580_AMD_STREAM,		R_5X0  },
 { DEVICE_ATI_R580_AMD_STREAM2,		R_5X0  },
 { DEVICE_ATI_RADEON_X1950_GT,		R_5X0  },
 { DEVICE_ATI_RADEON_X1650_XT,		R_5X0|R_PCIE  },
 { DEVICE_ATI_RADEON_X1650_SERIES,	R_5X0  },
 { DEVICE_ATI_RADEON_X1950_GT2,		R_5X0  },
 { DEVICE_ATI_RADEON_X1650_XT2,		R_5X0|R_PCIE  },
 { DEVICE_ATI_RADEON_X1650_SERIES2,	R_5X0  },
 { DEVICE_ATI_RADEON_9100_PRO,		R_5X0  },
 { DEVICE_ATI_RADEON_MOBILITY_9200,	R_5X0  },
/* R600 */
 { DEVICE_ATI_R600_RADEON_HD,		R_6X0|R_PCIE  },
 { DEVICE_ATI_R600_RADEON_HD2,		R_6X0|R_PCIE  },
 { DEVICE_ATI_R600GL_FIRE_GL,		R_6X0|R_PCIE  },
 { DEVICE_ATI_R600GL_FIRE_GL2,		R_6X0|R_PCIE  },
 { DEVICE_ATI_R600_FIREGL_V7600,	R_6X0|R_PCIE  },
 { DEVICE_ATI_RV610_RADEON_HD,		R_6X0|R_PCIE  },
 { DEVICE_ATI_RV610_VIDEO_DEVICE,	R_6X0|R_PCIE  },
 { DEVICE_ATI_RV610_LE_AGP,		R_6X0 },
 { DEVICE_ATI_RV_610LE_PCI,		R_6X0 },
 { DEVICE_ATI_RADEON_HD_2400,		R_6X0|R_PCIE  },
 { DEVICE_ATI_RADEON_E2400,		R_6X0|R_PCIE  },
 { DEVICE_ATI_RADEON_HD_3870,		R_6X0|R_PCIE  },
 { DEVICE_ATI_RV620_LE_AGP,		R_6X0  },
 { DEVICE_ATI_RV620_ATI_FIREGL,		R_6X0|R_PCIE  },
 { DEVICE_ATI_RV620_FIREPRO_2260,	R_6X0|R_PCIE  },
 { DEVICE_ATI_RV620_FIREPRO_22602,	R_6X0|R_PCIE  },
 { DEVICE_ATI_RADEON_HD_3200,		R_6X0|R_PCIE  },
 { DEVICE_ATI_RADEON_3100_GRAPHICS,	R_6X0|R_PCIE  },
 { DEVICE_ATI_RS780M_RS780MN_RADEON_HD,	R_6X0|R_PCIE  },
 { DEVICE_ATI_RS780MC_RADEON_HD,	R_6X0|R_PCIE  },
 { DEVICE_ATI_RADEON_HD_3300,		R_6X0|R_PCIE  },
 { DEVICE_ATI_RV_630_XT,		R_6X0|R_PCIE  },
 { DEVICE_ATI_RV630_PRO_AGP,		R_6X0  },
 { DEVICE_ATI_RV630_RADEON_HD,		R_6X0|R_PCIE  },
 { DEVICE_ATI_RV630_RADEON_HD2,		R_6X0|R_PCIE  },
 { DEVICE_ATI_RV630GL_FIREGL_V5600,	R_6X0|R_PCIE  },
 { DEVICE_ATI_RV630_FIREGL_V3600,	R_6X0|R_PCIE  },
 { DEVICE_ATI_RV635_PRO_AGP,		R_6X0  },
 { DEVICE_ATI_MOBILITY_RADEON_HD,	R_6X0|R_PCIE  },
 { DEVICE_ATI_MOBILITY_RADEON_HD2,	R_6X0|R_PCIE  },
 { DEVICE_ATI_MOBILITY_RADEON_HD3,	R_6X0|R_PCIE  },
 { DEVICE_ATI_MOBILITY_RADEON_HD4,	R_6X0|R_PCIE  },
 { DEVICE_ATI_MOBILITY_RADEON_HD5,	R_6X0|R_PCIE  },
 { DEVICE_ATI_MOBILITY_RADEON_HD6,	R_6X0|R_PCIE  },
 { DEVICE_ATI_MOBILITY_RADEON_HD7,	R_6X0|R_PCIE  },
 { DEVICE_ATI_MOBILITY_RADEON_HD8,	R_6X0|R_PCIE  },
 { DEVICE_ATI_M76_RADEON_MOBILITY,	R_6X0|R_PCIE  },
 { DEVICE_ATI_M76XT_MOBILITY_RADEON,	R_6X0|R_PCIE  },
 { DEVICE_ATI_RV670PRO_RADEON_HD,	R_6X0|R_PCIE  },
 { DEVICE_ATI_RV670_RADEON_HD,		R_6X0|R_PCIE  },
 { DEVICE_ATI_RV670_FIREGL_7700,	R_6X0|R_PCIE  },
 { DEVICE_ATI_RV670_AGP_RADEON,		R_6X0  },
 { DEVICE_ATI_RV670_FIRESTREAM_9170,	R_6X0|R_PCIE  },
 { DEVICE_ATI_M86GL_MOBILITY_FIREGL,	R_6X0|R_PCIE  },
 { DEVICE_ATI_M88_XT_MOBILITY,		R_6X0|R_PCIE  },
 { DEVICE_ATI_R680_RADEON_HD,		R_6X0|R_PCIE  },
 /* R700 */
 { DEVICE_ATI_R700_RADEON_HD,		R_7X0|R_PCIE  },
 { DEVICE_ATI_R700_RADEON_HD2,		R_7X0|R_PCIE  },
 { DEVICE_ATI_RV710_RADEON_HD,		R_7X0|R_PCIE  },
 { DEVICE_ATI_RV710_RADEON_HD2,		R_7X0|R_PCIE  },
 { DEVICE_ATI_RV730XT_RADEON_HD,	R_7X0|R_PCIE  },
 { DEVICE_ATI_RV730_PRO_RADEON,		R_7X0|R_PCIE  },
 { DEVICE_ATI_RV770_RADEON_HD,		R_7X0|R_PCIE  },
 { DEVICE_ATI_RV770_RADEON_HD2,		R_7X0|R_PCIE  },
 { DEVICE_ATI_M96_MOBILITY_RADEON,	R_7X0|R_PCIE  },
 { DEVICE_ATI_M98L_MOBILITY_RADEON,	R_7X0|R_PCIE  },
 { DEVICE_ATI_M98_XT_MOBILITY,		R_7X0|R_PCIE  },
 { DEVICE_ATI_RV770_LE_RADEON,		R_7X0|R_PCIE  },
};

static void * radeon_mmio_base = 0;
static void * radeon_mem_base = 0; 
static int32_t radeon_overlay_off = 0;
static uint32_t radeon_ram_size = 0;

#define GETREG(TYPE,PTR,OFFZ)		(*((volatile TYPE*)((PTR)+(OFFZ))))
#define SETREG(TYPE,PTR,OFFZ,VAL)	(*((volatile TYPE*)((PTR)+(OFFZ))))=VAL

#define INREG8(addr)		GETREG(uint8_t,(uint8_t *)(radeon_mmio_base),addr)
#define OUTREG8(addr,val)	SETREG(uint8_t,(uint8_t *)(radeon_mmio_base),addr,val)
static inline uint32_t INREG (uint32_t addr) {
    uint32_t tmp = GETREG(uint32_t,(uint8_t *)(radeon_mmio_base),addr);
    return le2me_32(tmp);
}
#define OUTREG(addr,val)	SETREG(uint32_t,(uint8_t *)(radeon_mmio_base),addr,le2me_32(val))
#define OUTREGP(addr,val,mask)						\
	do {								\
		unsigned int _tmp = INREG(addr);			\
		_tmp &= (mask);						\
		_tmp |= (val);						\
		OUTREG(addr, _tmp);					\
	} while (0)


enum radeon_montype
{
    MT_NONE,
    MT_CRT, /* CRT-(cathode ray tube) analog monitor. (15-pin VGA connector) */
    MT_LCD, /* Liquid Crystal Display */
    MT_DFP, /* DFP-digital flat panel monitor. (24-pin DVI-I connector) */
    MT_CTV, /* Composite TV out (not in VE) */
    MT_STV  /* S-Video TV out (probably in VE only) */
};

typedef struct radeon_info_s
{
	int crtDispType;
	int dviDispType;
}rinfo_t;

static rinfo_t rinfo;

static char * GET_MON_NAME(int type)
{
  char *pret;
  switch(type)
  {
    case MT_NONE: pret = "no"; break;
    case MT_CRT:  pret = "CRT"; break;
    case MT_DFP:  pret = "DFP"; break;
    case MT_LCD:  pret = "LCD"; break;
    case MT_CTV:  pret = "CTV"; break;
    case MT_STV:  pret = "STV"; break;
    default:	  pret = "Unknown";
  }
  return pret;
}

static void radeon_get_moninfo (rinfo_t *rinfo)
{
	unsigned int tmp;

	tmp = INREG(RHD_BIOS_4_SCRATCH);
	/* primary DVI port */
	if (tmp & RADEON_DFP2_ATTACHED)
		rinfo->dviDispType = MT_DFP;
	else if (tmp & RADEON_CRT2_ATTACHED_MASK)
		rinfo->dviDispType = MT_CRT;

	/* secondary CRT port */
	if (tmp & RADEON_CRT1_ATTACHED_MASK)
		rinfo->crtDispType = MT_CRT;
	else if (tmp & RADEON_LCD1_ATTACHED)
		rinfo->crtDispType = MT_LCD;
	else if (tmp & RADEON_DFP1_ATTACHED)
		rinfo->crtDispType = MT_DFP;
	else if (tmp & RADEON_TV1_ATTACHED_COMP)
		rinfo->crtDispType = MT_CTV;
	else if (tmp & RADEON_TV1_ATTACHED_SVIDEO)
		rinfo->crtDispType = MT_STV;
}

static uint32_t radeon_vid_get_dbpp( void )
{
  uint32_t dbpp,retval;
  dbpp = (INREG(ovl.GRPH_CONTOL))& D1GRPH_DEPTH;
  switch(dbpp)
  {
    case 0: retval = 8; break;
    case 1: retval = 16; break;
    case 2: retval = 32; break;
    case 3: retval = 64; break;
    default: retval=32; break;
  }
  return retval;
}

static uint32_t radeon_get_xres( void )
{
  uint32_t xres,h_total;
  h_total = INREG(ovl.CRTC_H_TOTAL);
  xres = h_total & 0x0fff;
  return xres + 1;
}

static uint32_t radeon_get_yres( void )
{
  uint32_t yres,v_total;
  v_total = INREG(ovl.CRTC_V_TOTAL);
  yres = v_total & 0x0fff;
  return yres + 1;
}

static void radeon_vid_make_default(void)
{
  besr.deinterlace_pattern = 0x900AAAAA;
  besr.deinterlace_on=1;
  besr.double_buff=1;
  besr.ckey_on=0;
  besr.graphics_key_msk=0;
  besr.graphics_key_clr=0;
}

unsigned VIDIX_NAME(vixGetVersion)( void ) { return VIDIX_VERSION; }

static int find_chip(unsigned chip_id)
{
  unsigned i;
  for(i = 0;i < sizeof(ati_card_ids)/sizeof(ati_card_ids_t);i++)
  {
    if(chip_id == ati_card_ids[i].id) return i;
  }
  return -1;
}

static pciinfo_t pci_info;
static int probed=0;

vidix_capability_t def_cap = 
{
    "BES driver for r600 cards",
    "Nickols_K",
    TYPE_OUTPUT | TYPE_FX,
    { 0, 0, 0, 0 },
    8192,
    8192,
    4,
    4,
    -1,
    FLAG_UPSCALER | FLAG_DOWNSCALER | FLAG_EQUALIZER,
    VENDOR_ATI,
    0,
    { 0, 0, 0, 0}
};


int VIDIX_NAME(vixProbe)( int verbose,int force )
{
  pciinfo_t lst[MAX_PCI_DEVICES];
  unsigned i,num_pci;
  int err;
  __verbose = verbose;
  err = pci_scan(lst,&num_pci);
  if(err)
  {
    printf(RADEON_MSG" Error occurred during pci scan: %s\n",strerror(err));
    return err;
  }
  else
  {
    err = ENXIO;
    for(i=0;i<num_pci;i++)
    {
      if(lst[i].vendor == VENDOR_ATI)
      {
	int idx;
	const char *dname;
	idx = find_chip(lst[i].device);
	if(idx == -1 && force == PROBE_NORMAL) continue;
	dname = pci_device_name(VENDOR_ATI,lst[i].device);
	dname = dname ? dname : "Unknown chip";
	printf(RADEON_MSG" Found chip: %s\n",dname);
#if 0
        if ((lst[i].command & PCI_COMMAND_IO) == 0)
        {
          printf("[radeon] Device is disabled, ignoring\n");
          continue;
        }
#endif
	memset(&besr,0,sizeof(bes_registers_t));
	if(force > PROBE_NORMAL)
	{
	    printf(RADEON_MSG" Driver was forced. Was found %sknown chip\n",idx == -1 ? "un" : "");
	    if(idx == -1)
		printf(RADEON_MSG" Assuming it as RadeonHD\n");
	}
	if(idx != -1) besr.chip_flags=ati_card_ids[idx].flags;
	def_cap.device_id = lst[i].device;
	err = 0;
	memcpy(&pci_info,&lst[i],sizeof(pciinfo_t));
	probed=1;
	break;
      }
    }
  }
  if(err && verbose) printf(RADEON_MSG" Can't find chip\n");
  return err;
}

typedef struct saved_regs_s
{
    uint32_t ov0_vid_key_clr;
    uint32_t ov0_vid_key_msk;
    uint32_t ov0_graphics_key_clr;
    uint32_t ov0_graphics_key_msk;
    uint32_t ov0_key_cntl;
    uint32_t disp_merge_cntl;
}saved_regs_t;
static saved_regs_t savreg;

static void save_regs( void )
{
}

static void restore_regs( void )
{
}

int VIDIX_NAME(vixInit)( const char *args )
{
  int err;

  if(__verbose>0) printf("[radeon_vid] version %d args='%s'\n", VIDIX_VERSION, args);

  if(!probed) 
  {
    printf(RADEON_MSG" Driver was not probed but is being initializing\n");
    return EINTR;
  }    
  if((radeon_mmio_base = map_phys_mem(pci_info.base2,0xFFFF))==(void *)-1) return ENOMEM;
  radeon_ram_size = INREG(RHD_CONFIG_MEMSIZE);
  printf(RADEON_MSG" Video memory = %uMb\n",radeon_ram_size/0x100000);
  if(radeon_ram_size>0x10000000UL) {
    radeon_ram_size=0x10000000UL;
    printf(RADEON_MSG" Will use %uMb only\n",radeon_ram_size/0x100000);
  }
  if((radeon_mem_base = map_phys_mem(pci_info.base0,radeon_ram_size))==(void *)-1) return ENOMEM;
  err = mtrr_set_type(pci_info.base0,radeon_ram_size,MTRR_TYPE_WRCOMB);
  if(!err) printf(RADEON_MSG" Set write-combining type of video memory\n");
  radeon_vid_make_default();
  {
    memset(&rinfo,0,sizeof(rinfo_t));
    radeon_get_moninfo(&rinfo);
	printf(RADEON_MSG" DVI port has %s monitor connected\n",GET_MON_NAME(rinfo.dviDispType));
	printf(RADEON_MSG" CRT port has %s monitor connected\n",GET_MON_NAME(rinfo.crtDispType));
	if(rinfo.dviDispType)	route_secondary();
	else			route_primary();
	printf(RADEON_MSG" Graphics: %ux%ux%ubpp\n",radeon_get_xres(),radeon_get_yres(),radeon_vid_get_dbpp());
  }
#ifdef RADEON_ENABLE_BM
  if(bm_open() == 0)
  {
	if((dma_phys_addrs = malloc(radeon_ram_size*sizeof(unsigned long)/4096)) != 0)
					    def_cap.flags |= FLAG_DMA | FLAG_EQ_DMA;
	else
		printf(RADEON_MSG" Can't allocate temopary buffer for DMA\n");
  }
  else
    if(__verbose) printf(RADEON_MSG" Can't initialize busmastering: %s\n",strerror(errno));
#endif
  save_regs();
  return 0;
}

void VIDIX_NAME(vixDestroy)( void )
{
  restore_regs();
  unmap_phys_mem(radeon_mem_base,radeon_ram_size);
  unmap_phys_mem(radeon_mmio_base,0xFFFF);
  bm_close();
}

int VIDIX_NAME(vixGetCapability)(vidix_capability_t *to)
{
  memcpy(to,&def_cap,sizeof(vidix_capability_t));
  return 0; 
}

/*
  Full list of fourcc which are supported by Win2K radeon driver:
  YUY2, UYVY, DDES, OGLT, OGL2, OGLS, OGLB, OGNT, OGNZ, OGNS,
  IF09, YVU9, IMC4, M2IA, IYUV, VBID, DXT1, DXT2, DXT3, DXT4, DXT5
*/
typedef struct fourcc_desc_s
{
    uint32_t fourcc;
    unsigned max_srcw;
}fourcc_desc_t;

fourcc_desc_t supported_fourcc[] = 
{
  { IMGFMT_RGB16, 8192 },
  { IMGFMT_BGR16, 8192 },
  { IMGFMT_RGB32, 8192 },
  { IMGFMT_BGR32, 8192 }
};

__inline__ static int is_supported_fourcc(uint32_t fourcc)
{
  unsigned i;
  for(i=0;i<sizeof(supported_fourcc)/sizeof(fourcc_desc_t);i++)
  {
    if(fourcc==supported_fourcc[i].fourcc)
	return 1;
  }
  return 0;
}

int VIDIX_NAME(vixQueryFourcc)(vidix_fourcc_t *to)
{
    if(is_supported_fourcc(to->fourcc))
    {
	to->depth = VID_DEPTH_1BPP | VID_DEPTH_2BPP |
		    VID_DEPTH_4BPP | VID_DEPTH_8BPP |
		    VID_DEPTH_12BPP| VID_DEPTH_15BPP|
		    VID_DEPTH_16BPP| VID_DEPTH_24BPP|
		    VID_DEPTH_32BPP;
	to->flags = VID_CAP_EXPAND | VID_CAP_SHRINK | VID_CAP_COLORKEY |
		    VID_CAP_BLEND;
	return 0;
    }
    else  to->depth = to->flags = 0;
    return ENOSYS;
}

static void radeon_vid_stop_video( void )
{
    OUTREG(ovl.OVL_ENABLE,0);
}

static void radeon_vid_display_video( void )
{
    OUTREG(ovl.OVL_ENABLE,1);
    OUTREG(ovl.OVL_CONTROL1,besr.control1|D1OVL_ARRAY_LINEAR_ALIGNED);
    OUTREG(ovl.OVL_CONTROL2,besr.control2);
    OUTREG(ovl.OVL_SWAP_CNTL,besr.swap_cntl);
    OUTREG(ovl.OVL_SURFACE_ADDRESS,besr.vid_buf_base_adrs_y[0]);
    OUTREG(ovl.OVL_SURFACE_OFFSET_X,0);
    OUTREG(ovl.OVL_SURFACE_OFFSET_Y,0);
    OUTREG(ovl.OVL_START,besr.y_x_start);
    OUTREG(ovl.OVL_END,besr.y_x_end);
    OUTREG(ovl.OVL_PITCH,besr.vid_buf_pitch0_value);
}

static unsigned radeon_query_pitch(unsigned fourcc,const vidix_yuv_t *spitch)
{
  return 256;
}

static int radeon_vid_init_video( vidix_playback_t *config )
{
    unsigned best_pitch,mpitch,pitch,i;
    radeon_vid_stop_video();
    best_pitch = radeon_query_pitch(config->fourcc,&config->src.pitch);
    mpitch = best_pitch-1;
    switch(config->fourcc)
    {
	/* RGB 4:4:4:4 */
	case IMGFMT_RGB32:
	case IMGFMT_BGR32: pitch = (config->src.w*4 + mpitch) & ~mpitch;
			  config->dest.pitch.y =
			  config->dest.pitch.u =
			  config->dest.pitch.v = best_pitch;
			  break;
	/* 4:2:2 */
	
	default: /* RGB15, RGB16, YVYU, UYVY, YUY2 */
			  pitch = ((config->src.w*2) + mpitch) & ~mpitch;
			  config->dest.pitch.y =
			  config->dest.pitch.u =
			  config->dest.pitch.v = best_pitch;
			  break;
    }
    besr.control2=0;
    besr.swap_cntl=0;
    switch(config->fourcc)
    {
	case IMGFMT_RGB16:
			   besr.swap_cntl=1;
	case IMGFMT_BGR16: besr.control1 = (2<<8)|1;
			   break;
	case IMGFMT_RGB32:
			   besr.swap_cntl=2;
	default:
	case IMGFMT_BGR32: besr.control1=(2<<8)|2;
			   break;
    }
    /* keep everything in 16.16 */
	config->offsets[0] = 0;
	for(i=1;i<besr.vid_nbufs;i++)
	    config->offsets[i] = config->offsets[i-1]+config->frame_size;
	for(i=0;i<besr.vid_nbufs;i++)
	{
	    besr.vid_buf_base_adrs_y[i]=((radeon_overlay_off+config->offsets[i]+config->offset.y)&(~255));
	    besr.vid_buf_base_adrs_v[i]=0;
	    besr.vid_buf_base_adrs_u[i]=0;
	}
	config->offset.y = ((besr.vid_buf_base_adrs_y[0])&(~255)) - radeon_overlay_off;
	config->offset.v = 0;
	config->offset.u = 0;
	besr.y_x_start = (config->dest.x << 16) | (config->dest.y);
	besr.y_x_end = ((config->dest.x + config->dest.w)<<16) | (config->dest.y + config->dest.h);
	besr.vid_buf_pitch0_value = pitch;
	besr.vid_buf_pitch1_value = pitch;
    return 0;
}

static void radeon_compute_framesize(vidix_playback_t *info)
{
  unsigned pitch,awidth,dbpp;
  pitch = radeon_query_pitch(info->fourcc,&info->src.pitch);
  dbpp = radeon_vid_get_dbpp();
  switch(info->fourcc)
  {
    case IMGFMT_RGB32:
    case IMGFMT_BGR32:
		awidth = (info->src.w*4 + (pitch-1)) & ~(pitch-1);
		info->frame_size = awidth*info->src.h;
		break;
    /* YUY2 YVYU, RGB15, RGB16 */
    default:	
		awidth = (info->src.w*2 + (pitch-1)) & ~(pitch-1);
		info->frame_size = awidth*info->src.h;
		break;
  }
  info->frame_size = (info->frame_size+4095)&~4095;
}

int VIDIX_NAME(vixConfigPlayback)(vidix_playback_t *info)
{
  unsigned rgb_size,nfr;
  uint32_t radeon_video_size;
  if(!is_supported_fourcc(info->fourcc)) return ENOSYS;
  if(info->num_frames>VID_PLAY_MAXFRAMES) info->num_frames=VID_PLAY_MAXFRAMES;
  if(info->num_frames==1) besr.double_buff=0;
  else			  besr.double_buff=1;
  radeon_compute_framesize(info);
    
  rgb_size = radeon_get_xres()*radeon_get_yres()*((radeon_vid_get_dbpp()+7)/8);
  nfr = info->num_frames;
  radeon_video_size = radeon_ram_size;
#ifdef RADEON_ENABLE_BM
  if(def_cap.flags & FLAG_DMA)
  {
     /* every descriptor describes one 4K page and takes 16 bytes in memory 
	Note: probably it's ont good idea to locate them in video memory
	but as initial release it's OK */
	radeon_video_size -= radeon_ram_size * sizeof(bm_list_descriptor) / 4096;
	radeon_dma_desc_base = (void *)(pci_info.base0 + radeon_video_size);
  }
#endif
  for(;nfr>0; nfr--)
  {
      radeon_overlay_off = radeon_video_size - info->frame_size*nfr;
      radeon_overlay_off &= 0xffff0000;
      if(radeon_overlay_off >= (int)rgb_size ) break;
  }
  if(nfr <= 3)
  {
   nfr = info->num_frames;
   for(;nfr>0; nfr--)
   {
      radeon_overlay_off = radeon_video_size - info->frame_size*nfr;
      radeon_overlay_off &= 0xffff0000;
      if(radeon_overlay_off > 0) break;
   }
  }
  if(nfr <= 0) return EINVAL;
  info->num_frames = nfr;
  besr.vid_nbufs = info->num_frames;
  info->dga_addr = (char *)radeon_mem_base + radeon_overlay_off;
  radeon_vid_init_video(info);
  return 0;
}

int VIDIX_NAME(vixPlaybackOn)( void )
{
  radeon_vid_display_video();
  return 0;
}

int VIDIX_NAME(vixPlaybackOff)( void )
{
  radeon_vid_stop_video();
  return 0;
}

int VIDIX_NAME(vixPlaybackFrameSelect)(unsigned frame)
{
    if(!besr.double_buff) return 0;
    if(frame > besr.vid_nbufs) frame = besr.vid_nbufs-1;
    OUTREG(ovl.OVL_SURFACE_ADDRESS,besr.vid_buf_base_adrs_y[frame]);
    return 0;
}

vidix_video_eq_t equal =
{
 VEQ_CAP_BRIGHTNESS | VEQ_CAP_SATURATION
 | VEQ_CAP_CONTRAST | VEQ_CAP_HUE | VEQ_CAP_RGB_INTENSITY,
 0, 0, 0, 0, 0, 0, 0, 0 };

int	VIDIX_NAME(vixPlaybackGetEq)( vidix_video_eq_t * eq)
{
  memcpy(eq,&equal,sizeof(vidix_video_eq_t));
  return 0;
}

int	VIDIX_NAME(vixPlaybackSetEq)( const vidix_video_eq_t * eq)
{
  return 0;
}

int	VIDIX_NAME(vixPlaybackSetDeint)( const vidix_deinterlace_t * info)
{
  return 0;
}

int	VIDIX_NAME(vixPlaybackGetDeint)( vidix_deinterlace_t * info)
{
  info->flags = CFG_NON_INTERLACED;
  return 0;
}


/* Graphic keys */
static vidix_grkey_t radeon_grkey;

static int set_gr_key( void )
{
    int result = 0;
    if(radeon_grkey.ckey.op == CKEY_TRUE)
    {
	besr.ckey_on=1;
	OUTREG(ovl.OVL_KEY_CONTOL,D1GRPH_TRUE|(D1OVL_TRUE<<8)|(D1OVL_COMPARE_AND<<16));
	OUTREG(ovl.GRPH_KEY_RANGE_RED,radeon_grkey.ckey.red);
	OUTREG(ovl.GRPH_KEY_RANGE_GREEN,radeon_grkey.ckey.green);
	OUTREG(ovl.GRPH_KEY_RANGE_BLUE,radeon_grkey.ckey.blue);
    }
    else if(radeon_grkey.ckey.op == CKEY_ALPHA)
    {
	besr.ckey_on=1;
	OUTREG(ovl.OVL_KEY_CONTOL,D1GRPH_RGBA|(D1OVL_TRUE<<8)|(D1OVL_COMPARE_AND<<16));
	OUTREG(ovl.GRPH_KEY_RANGE_RED,radeon_grkey.ckey.red);
	OUTREG(ovl.GRPH_KEY_RANGE_GREEN,radeon_grkey.ckey.green);
	OUTREG(ovl.GRPH_KEY_RANGE_BLUE,radeon_grkey.ckey.blue);
	OUTREG(ovl.GRPH_KEY_RANGE_ALPHA,radeon_grkey.ckey.reserved);
    }
    else
    {
	besr.ckey_on=0;
	besr.graphics_key_msk=0;
	besr.graphics_key_clr=0;
	OUTREG(ovl.OVL_KEY_CONTOL,D1GRPH_FALSE|(D1OVL_FALSE<<8)|(D1OVL_COMPARE_OR<<16));
	OUTREG(ovl.GRPH_KEY_RANGE_RED,0);
	OUTREG(ovl.GRPH_KEY_RANGE_GREEN,0);
	OUTREG(ovl.GRPH_KEY_RANGE_BLUE,0);
    }
    return result;
}

int VIDIX_NAME(vixGetGrKeys)(vidix_grkey_t *grkey)
{
    memcpy(grkey, &radeon_grkey, sizeof(vidix_grkey_t));
    return(0);
}

int VIDIX_NAME(vixSetGrKeys)(const vidix_grkey_t *grkey)
{
    memcpy(&radeon_grkey, grkey, sizeof(vidix_grkey_t));
    return (set_gr_key());
}

#ifdef RADEON_ENABLE_BM
static int radeon_setup_frame( vidix_dma_t * dmai )
{
    bm_list_descriptor * list = (bm_list_descriptor *)radeon_dma_desc_base;
    unsigned long dest_ptr;
    unsigned i,n,count;
    int retval;
    if(dmai->dest_offset + dmai->size > radeon_ram_size) return E2BIG;
    n = dmai->size / 4096;
    if(dmai->size % 4096) n++;
    if((retval = bm_virt_to_bus(dmai->src,dmai->size,dma_phys_addrs)) != 0) return retval;
    dest_ptr = dmai->dest_offset;
    count = dmai->size;
    for(i=0;i<n;i++)
    {
	list[i].framebuf_offset = radeon_overlay_off + dest_ptr;
	list[i].sys_addr = dma_phys_addrs[i]; 
	list[i].command = (count > 4096 ? 4096 : count | DMA_GUI_COMMAND__EOL);
	list[i].reserved = 0;
printf("RADEON_DMA_TABLE[%i] %X %X %X %X\n",i,list[i].framebuf_offset,list[i].sys_addr,list[i].command,list[i].reserved);
	dest_ptr += 4096;
	count -= 4096;
    }
    return 0;
}

static int radeon_transfer_frame( void	)
{
    unsigned i;
    radeon_engine_idle();
    for(i=0;i<1000;i++) INREG(BUS_CNTL); /* FlushWriteCombining */
    OUTREG(BUS_CNTL,(INREG(BUS_CNTL) | BUS_STOP_REQ_DIS)&(~BUS_MASTER_DIS));
    OUTREG(MC_FB_LOCATION,
	    ((pci_info.base0>>16)&0xffff)|
	    ((pci_info.base0+INREG(CONFIG_APER_SIZE)-1)&0xffff0000));
    if((INREG(MC_AGP_LOCATION)&0xffff)!=
      (((pci_info.base0+INREG(CONFIG_APER_SIZE))>>16)&0xffff))
    /*Radeon memory controller is misconfigured*/
	    return EINVAL;
    OUTREG(DMA_VID_ACT_DSCRPTR,bus_addr_dma_desc);
//    OUTREG(GEN_INT_STATUS,INREG(GEN_INT_STATUS)|(1<<30));
    OUTREG(GEN_INT_STATUS,INREG(GEN_INT_STATUS)|0x00010000);
    return 0;
}


int VIDIX_NAME(vixPlaybackCopyFrame)( vidix_dma_t * dmai )
{
    int retval;
    if(mlock(dmai->src,dmai->size) != 0) return errno;
    retval = radeon_setup_frame(dmai);
    if(retval == 0) retval = radeon_transfer_frame();
    munlock(dmai->src,dmai->size);
    return retval;
}

int VIDIX_NAME(vixQueryDMAStatus)( void )
{
    int bm_active;
    bm_active=(INREG(GEN_INT_STATUS)&0x00010000)==0?1:0;
    return bm_active?1:0;
}
#endif
