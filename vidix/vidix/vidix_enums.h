#ifndef __VIDIX_ENUMS_H_INCLUDED
#define __VIDIX_ENUMS_H_INCLUDED 1

typedef enum vidix_dev_type {
  TYPE_OUTPUT  =        0x00000000,	/* Is a video playback device */
  TYPE_CAPTURE =	0x00000001,	/* Is a capture device */
  TYPE_CODEC   =        0x00000002,	/* Device supports hw (de)coding */
  TYPE_FX      =	0x00000004,	/* Is a video effects device */
} vidix_dev_type_t;
#ifdef __cplusplus
extern "C++" {
inline vidix_dev_type_t operator~(vidix_dev_type_t a) { return static_cast<vidix_dev_type_t>(~static_cast<unsigned>(a)); }
inline vidix_dev_type_t operator|(vidix_dev_type_t a, vidix_dev_type_t b) { return static_cast<vidix_dev_type_t>(static_cast<unsigned>(a)|static_cast<unsigned>(b)); }
inline vidix_dev_type_t operator&(vidix_dev_type_t a, vidix_dev_type_t b) { return static_cast<vidix_dev_type_t>(static_cast<unsigned>(a)&static_cast<unsigned>(b)); }
inline vidix_dev_type_t operator^(vidix_dev_type_t a, vidix_dev_type_t b) { return static_cast<vidix_dev_type_t>(static_cast<unsigned>(a)^static_cast<unsigned>(b)); }
inline vidix_dev_type_t operator|=(vidix_dev_type_t a, vidix_dev_type_t b) { return (a=static_cast<vidix_dev_type_t>(static_cast<unsigned>(a)|static_cast<unsigned>(b))); }
inline vidix_dev_type_t operator&=(vidix_dev_type_t a, vidix_dev_type_t b) { return (a=static_cast<vidix_dev_type_t>(static_cast<unsigned>(a)&static_cast<unsigned>(b))); }
inline vidix_dev_type_t operator^=(vidix_dev_type_t a, vidix_dev_type_t b) { return (a=static_cast<vidix_dev_type_t>(static_cast<unsigned>(a)^static_cast<unsigned>(b))); }
}
#endif

typedef enum vidix_dev_flag {
  FLAG_NONE       =	0x00000000, /* No flags defined */
  FLAG_DMA        =	0x00000001, /* Card can use DMA */
  /* Card can use DMA only if src pitch == dest pitch */
  FLAG_EQ_DMA     =	0x00000002,
  /* Possible to wait for DMA to finish. See BM_DMA_SYNC and BM_DMA_BLOCK */
  FLAG_SYNC_DMA   =     0x00000004,
  FLAG_UPSCALER   =	0x00000010, /* Card supports hw upscaling */
  FLAG_DOWNSCALER =	0x00000020, /* Card supports hw downscaling */
  FLAG_SUBPIC	  =	0x00001000, /* Card supports DVD subpictures */
  FLAG_EQUALIZER  =	0x00002000, /* Card supports equalizer */
} vidix_dev_flag_t;
#ifdef __cplusplus
extern "C++" {
inline vidix_dev_flag_t operator~(vidix_dev_flag_t a) { return static_cast<vidix_dev_flag_t>(~static_cast<unsigned>(a)); }
inline vidix_dev_flag_t operator|(vidix_dev_flag_t a, vidix_dev_flag_t b) { return static_cast<vidix_dev_flag_t>(static_cast<unsigned>(a)|static_cast<unsigned>(b)); }
inline vidix_dev_flag_t operator&(vidix_dev_flag_t a, vidix_dev_flag_t b) { return static_cast<vidix_dev_flag_t>(static_cast<unsigned>(a)&static_cast<unsigned>(b)); }
inline vidix_dev_flag_t operator^(vidix_dev_flag_t a, vidix_dev_flag_t b) { return static_cast<vidix_dev_flag_t>(static_cast<unsigned>(a)^static_cast<unsigned>(b)); }
inline vidix_dev_flag_t operator|=(vidix_dev_flag_t a, vidix_dev_flag_t b) { return (a=static_cast<vidix_dev_flag_t>(static_cast<unsigned>(a)|static_cast<unsigned>(b))); }
inline vidix_dev_flag_t operator&=(vidix_dev_flag_t a, vidix_dev_flag_t b) { return (a=static_cast<vidix_dev_flag_t>(static_cast<unsigned>(a)&static_cast<unsigned>(b))); }
inline vidix_dev_flag_t operator^=(vidix_dev_flag_t a, vidix_dev_flag_t b) { return (a=static_cast<vidix_dev_flag_t>(static_cast<unsigned>(a)^static_cast<unsigned>(b))); }
}
#endif

typedef enum vidix_depth {
  VID_DEPTH_NONE  =		0x0000,
  VID_DEPTH_1BPP  =		0x0001,
  VID_DEPTH_2BPP  =		0x0002,
  VID_DEPTH_4BPP  =		0x0004,
  VID_DEPTH_8BPP  =		0x0008,
  VID_DEPTH_12BPP =		0x0010,
  VID_DEPTH_15BPP =		0x0020,
  VID_DEPTH_16BPP =		0x0040,
  VID_DEPTH_24BPP =		0x0080,
  VID_DEPTH_32BPP =		0x0100,
  VID_DEPTH_ALL   =             VID_DEPTH_1BPP  | VID_DEPTH_2BPP  | \
                                VID_DEPTH_4BPP  | VID_DEPTH_8BPP  | \
		                VID_DEPTH_12BPP | VID_DEPTH_15BPP | \
		                VID_DEPTH_16BPP | VID_DEPTH_24BPP | \
                                VID_DEPTH_32BPP,
} vidix_depth_t;
#ifdef __cplusplus
extern "C++" {
inline vidix_depth_t operator~(vidix_depth_t a) { return static_cast<vidix_depth_t>(~static_cast<unsigned>(a)); }
inline vidix_depth_t operator|(vidix_depth_t a, vidix_depth_t b) { return static_cast<vidix_depth_t>(static_cast<unsigned>(a)|static_cast<unsigned>(b)); }
inline vidix_depth_t operator&(vidix_depth_t a, vidix_depth_t b) { return static_cast<vidix_depth_t>(static_cast<unsigned>(a)&static_cast<unsigned>(b)); }
inline vidix_depth_t operator^(vidix_depth_t a, vidix_depth_t b) { return static_cast<vidix_depth_t>(static_cast<unsigned>(a)^static_cast<unsigned>(b)); }
inline vidix_depth_t operator|=(vidix_depth_t a, vidix_depth_t b) { return (a=static_cast<vidix_depth_t>(static_cast<unsigned>(a)|static_cast<unsigned>(b))); }
inline vidix_depth_t operator&=(vidix_depth_t a, vidix_depth_t b) { return (a=static_cast<vidix_depth_t>(static_cast<unsigned>(a)&static_cast<unsigned>(b))); }
inline vidix_depth_t operator^=(vidix_depth_t a, vidix_depth_t b) { return (a=static_cast<vidix_depth_t>(static_cast<unsigned>(a)^static_cast<unsigned>(b))); }
}
#endif

typedef enum vidix_cap {
  VID_CAP_NONE               =	0x0000,
  /* if overlay can be bigger than source */
  VID_CAP_EXPAND             =	0x0001,
  /* if overlay can be smaller than source */
  VID_CAP_SHRINK             =	0x0002,
  /* if overlay can be blended with framebuffer */
  VID_CAP_BLEND              =	0x0004,
  /* if overlay can be restricted to a colorkey */
  VID_CAP_COLORKEY           =	0x0008,
  /* if overlay can be restricted to an alpha channel */
  VID_CAP_ALPHAKEY           =	0x0010,
  /* if the colorkey can be a range */
  VID_CAP_COLORKEY_ISRANGE   =	0x0020,
  /* if the alphakey can be a range */
  VID_CAP_ALPHAKEY_ISRANGE   =	0x0040,
  /* colorkey is checked against framebuffer */
  VID_CAP_COLORKEY_ISMAIN    =	0x0080,
  /* colorkey is checked against overlay */
  VID_CAP_COLORKEY_ISOVERLAY =	0x0100,
  /* alphakey is checked against framebuffer */
  VID_CAP_ALPHAKEY_ISMAIN    =	0x0200,
  /* alphakey is checked against overlay */
  VID_CAP_ALPHAKEY_ISOVERLAY =  0x0400,
} vidix_cap_t;
#ifdef __cplusplus
extern "C++" {
inline vidix_cap_t operator~(vidix_cap_t a) { return static_cast<vidix_cap_t>(~static_cast<unsigned>(a)); }
inline vidix_cap_t operator|(vidix_cap_t a, vidix_cap_t b) { return static_cast<vidix_cap_t>(static_cast<unsigned>(a)|static_cast<unsigned>(b)); }
inline vidix_cap_t operator&(vidix_cap_t a, vidix_cap_t b) { return static_cast<vidix_cap_t>(static_cast<unsigned>(a)&static_cast<unsigned>(b)); }
inline vidix_cap_t operator^(vidix_cap_t a, vidix_cap_t b) { return static_cast<vidix_cap_t>(static_cast<unsigned>(a)^static_cast<unsigned>(b)); }
inline vidix_cap_t operator|=(vidix_cap_t a, vidix_cap_t b) { return (a=static_cast<vidix_cap_t>(static_cast<unsigned>(a)|static_cast<unsigned>(b))); }
inline vidix_cap_t operator&=(vidix_cap_t a, vidix_cap_t b) { return (a=static_cast<vidix_cap_t>(static_cast<unsigned>(a)&static_cast<unsigned>(b))); }
inline vidix_cap_t operator^=(vidix_cap_t a, vidix_cap_t b) { return (a=static_cast<vidix_cap_t>(static_cast<unsigned>(a)^static_cast<unsigned>(b))); }
}
#endif

typedef enum vidix_color_key_op {
  CKEY_FALSE =	0,
  CKEY_TRUE  =	1,
  CKEY_EQ    =	2,
  CKEY_NEQ   =	3,
  CKEY_ALPHA =	4,
} vidix_color_key_op_t;

typedef enum vidix_video_key_op {
  VKEY_FALSE =	0,
  VKEY_TRUE  =	1,
  VKEY_EQ    =	2,
  VKEY_NEQ   =	3,
} vidix_video_key_op_t;

typedef enum vidix_interleave {
  VID_PLAY_INTERLEAVED_UV =             0x00000001,
  /* UVUVUVUVUV used by Matrox G200 */
  INTERLEAVING_UV         =             0x00001000,
  /* VUVUVUVUVU */
  INTERLEAVING_VU         =		0x00001001,
} vidix_interleave_t;
#ifdef __cplusplus
extern "C++" {
inline vidix_interleave_t operator~(vidix_interleave_t a) { return static_cast<vidix_interleave_t>(~static_cast<unsigned>(a)); }
inline vidix_interleave_t operator|(vidix_interleave_t a, vidix_interleave_t b) { return static_cast<vidix_interleave_t>(static_cast<unsigned>(a)|static_cast<unsigned>(b)); }
inline vidix_interleave_t operator&(vidix_interleave_t a, vidix_interleave_t b) { return static_cast<vidix_interleave_t>(static_cast<unsigned>(a)&static_cast<unsigned>(b)); }
inline vidix_interleave_t operator^(vidix_interleave_t a, vidix_interleave_t b) { return static_cast<vidix_interleave_t>(static_cast<unsigned>(a)^static_cast<unsigned>(b)); }
inline vidix_interleave_t operator|=(vidix_interleave_t a, vidix_interleave_t b) { return (a=static_cast<vidix_interleave_t>(static_cast<unsigned>(a)|static_cast<unsigned>(b))); }
inline vidix_interleave_t operator&=(vidix_interleave_t a, vidix_interleave_t b) { return (a=static_cast<vidix_interleave_t>(static_cast<unsigned>(a)&static_cast<unsigned>(b))); }
inline vidix_interleave_t operator^=(vidix_interleave_t a, vidix_interleave_t b) { return (a=static_cast<vidix_interleave_t>(static_cast<unsigned>(a)^static_cast<unsigned>(b))); }
}
#endif

typedef enum vidix_key_op {
  KEYS_PUT =	0,
  KEYS_AND =	1,
  KEYS_OR  =	2,
  KEYS_XOR =	3,
} vidix_key_op_t;

typedef enum vidix_veq_cap {
  VEQ_CAP_NONE		=	0x00000000UL,
  VEQ_CAP_BRIGHTNESS	=	0x00000001UL,
  VEQ_CAP_CONTRAST	=	0x00000002UL,
  VEQ_CAP_SATURATION	=	0x00000004UL,
  VEQ_CAP_HUE		=	0x00000008UL,
  VEQ_CAP_RGB_INTENSITY =	0x00000010UL,
} vidix_veq_cap_t;
#ifdef __cplusplus
extern "C++" {
inline vidix_veq_cap_t operator~(vidix_veq_cap_t a) { return static_cast<vidix_veq_cap_t>(~static_cast<unsigned>(a)); }
inline vidix_veq_cap_t operator|(vidix_veq_cap_t a, vidix_veq_cap_t b) { return static_cast<vidix_veq_cap_t>(static_cast<unsigned>(a)|static_cast<unsigned>(b)); }
inline vidix_veq_cap_t operator&(vidix_veq_cap_t a, vidix_veq_cap_t b) { return static_cast<vidix_veq_cap_t>(static_cast<unsigned>(a)&static_cast<unsigned>(b)); }
inline vidix_veq_cap_t operator^(vidix_veq_cap_t a, vidix_veq_cap_t b) { return static_cast<vidix_veq_cap_t>(static_cast<unsigned>(a)^static_cast<unsigned>(b)); }
inline vidix_veq_cap_t operator|=(vidix_veq_cap_t a, vidix_veq_cap_t b) { return (a=static_cast<vidix_veq_cap_t>(static_cast<unsigned>(a)|static_cast<unsigned>(b))); }
inline vidix_veq_cap_t operator&=(vidix_veq_cap_t a, vidix_veq_cap_t b) { return (a=static_cast<vidix_veq_cap_t>(static_cast<unsigned>(a)&static_cast<unsigned>(b))); }
inline vidix_veq_cap_t operator^=(vidix_veq_cap_t a, vidix_veq_cap_t b) { return (a=static_cast<vidix_veq_cap_t>(static_cast<unsigned>(a)^static_cast<unsigned>(b))); }
}
#endif

typedef enum vidix_veq_flag {
  VEQ_FLG_ITU_R_BT_601 = 0x00000000, /* ITU-R BT.601 colour space (default) */
  VEQ_FLG_ITU_R_BT_709 = 0x00000001, /* ITU-R BT.709 colour space */
  VEQ_FLG_ITU_MASK     = 0x0000000f,
} vidix_veq_flag_t;
#ifdef __cplusplus
extern "C++" {
inline vidix_veq_flag_t operator~(vidix_veq_flag_t a) { return static_cast<vidix_veq_flag_t>(~static_cast<unsigned>(a)); }
inline vidix_veq_flag_t operator|(vidix_veq_flag_t a, vidix_veq_flag_t b) { return static_cast<vidix_veq_flag_t>(static_cast<unsigned>(a)|static_cast<unsigned>(b)); }
inline vidix_veq_flag_t operator&(vidix_veq_flag_t a, vidix_veq_flag_t b) { return static_cast<vidix_veq_flag_t>(static_cast<unsigned>(a)&static_cast<unsigned>(b)); }
inline vidix_veq_flag_t operator^(vidix_veq_flag_t a, vidix_veq_flag_t b) { return static_cast<vidix_veq_flag_t>(static_cast<unsigned>(a)^static_cast<unsigned>(b)); }
inline vidix_veq_flag_t operator|=(vidix_veq_flag_t a, vidix_veq_flag_t b) { return (a=static_cast<vidix_veq_flag_t>(static_cast<unsigned>(a)|static_cast<unsigned>(b))); }
inline vidix_veq_flag_t operator&=(vidix_veq_flag_t a, vidix_veq_flag_t b) { return (a=static_cast<vidix_veq_flag_t>(static_cast<unsigned>(a)&static_cast<unsigned>(b))); }
inline vidix_veq_flag_t operator^=(vidix_veq_flag_t a, vidix_veq_flag_t b) { return (a=static_cast<vidix_veq_flag_t>(static_cast<unsigned>(a)^static_cast<unsigned>(b))); }
}
#endif

typedef enum vidix_interlace_flag {
  /* stream is not interlaced */
  CFG_NON_INTERLACED       =	0x00000000,
  /* stream is interlaced */
  CFG_INTERLACED           =	0x00000001,
  /* first frame contains even fields but second - odd */
  CFG_EVEN_ODD_INTERLACING =	0x00000002,
  /* first frame contains odd fields but second - even */
  CFG_ODD_EVEN_INTERLACING =	0x00000004,
  /* field deinterlace_pattern is valid */
  CFG_UNIQUE_INTERLACING   =	0x00000008,
  /* unknown deinterlacing - use adaptive if it's possible */
  CFG_UNKNOWN_INTERLACING  =	0x0000000f,
} vidix_interlace_flag_t;
#ifdef __cplusplus
extern "C++" {
inline vidix_interlace_flag_t operator~(vidix_interlace_flag_t a) { return static_cast<vidix_interlace_flag_t>(~static_cast<unsigned>(a)); }
inline vidix_interlace_flag_t operator|(vidix_interlace_flag_t a, vidix_interlace_flag_t b) { return static_cast<vidix_interlace_flag_t>(static_cast<unsigned>(a)|static_cast<unsigned>(b)); }
inline vidix_interlace_flag_t operator&(vidix_interlace_flag_t a, vidix_interlace_flag_t b) { return static_cast<vidix_interlace_flag_t>(static_cast<unsigned>(a)&static_cast<unsigned>(b)); }
inline vidix_interlace_flag_t operator^(vidix_interlace_flag_t a, vidix_interlace_flag_t b) { return static_cast<vidix_interlace_flag_t>(static_cast<unsigned>(a)^static_cast<unsigned>(b)); }
inline vidix_interlace_flag_t operator|=(vidix_interlace_flag_t a, vidix_interlace_flag_t b) { return (a=static_cast<vidix_interlace_flag_t>(static_cast<unsigned>(a)|static_cast<unsigned>(b))); }
inline vidix_interlace_flag_t operator&=(vidix_interlace_flag_t a, vidix_interlace_flag_t b) { return (a=static_cast<vidix_interlace_flag_t>(static_cast<unsigned>(a)&static_cast<unsigned>(b))); }
inline vidix_interlace_flag_t operator^=(vidix_interlace_flag_t a, vidix_interlace_flag_t b) { return (a=static_cast<vidix_interlace_flag_t>(static_cast<unsigned>(a)^static_cast<unsigned>(b))); }
}
#endif

typedef enum vidix_bm_flag {
  BM_DMA_ASYNC	     = 0,
  /* await previous dma transfer completion */
  BM_DMA_SYNC        = 1,
  /* app -> driver: app uses buffers which are fixed in memory  */
  BM_DMA_FIXED_BUFFS = 2,
  /* block until the transfer is complete */
  BM_DMA_BLOCK       = 4,
} vidix_bm_flag_t;
#ifdef __cplusplus
extern "C++" {
inline vidix_bm_flag_t operator~(vidix_bm_flag_t a) { return static_cast<vidix_bm_flag_t>(~static_cast<unsigned>(a)); }
inline vidix_bm_flag_t operator|(vidix_bm_flag_t a, vidix_bm_flag_t b) { return static_cast<vidix_bm_flag_t>(static_cast<unsigned>(a)|static_cast<unsigned>(b)); }
inline vidix_bm_flag_t operator&(vidix_bm_flag_t a, vidix_bm_flag_t b) { return static_cast<vidix_bm_flag_t>(static_cast<unsigned>(a)&static_cast<unsigned>(b)); }
inline vidix_bm_flag_t operator^(vidix_bm_flag_t a, vidix_bm_flag_t b) { return static_cast<vidix_bm_flag_t>(static_cast<unsigned>(a)^static_cast<unsigned>(b)); }
inline vidix_bm_flag_t operator|=(vidix_bm_flag_t a, vidix_bm_flag_t b) { return (a=static_cast<vidix_bm_flag_t>(static_cast<unsigned>(a)|static_cast<unsigned>(b))); }
inline vidix_bm_flag_t operator&=(vidix_bm_flag_t a, vidix_bm_flag_t b) { return (a=static_cast<vidix_bm_flag_t>(static_cast<unsigned>(a)&static_cast<unsigned>(b))); }
inline vidix_bm_flag_t operator^=(vidix_bm_flag_t a, vidix_bm_flag_t b) { return (a=static_cast<vidix_bm_flag_t>(static_cast<unsigned>(a)^static_cast<unsigned>(b))); }
}
#endif

typedef enum vidix_fx_type {
  FX_TYPE_BOOLEAN =		0x00000000,
  FX_TYPE_INTEGER =		0x00000001,
} vidix_fx_type_t;
#ifdef __cplusplus
extern "C++" {
inline vidix_fx_type_t operator~(vidix_fx_type_t a) { return static_cast<vidix_fx_type_t>(~static_cast<unsigned>(a)); }
inline vidix_fx_type_t operator|(vidix_fx_type_t a, vidix_fx_type_t b) { return static_cast<vidix_fx_type_t>(static_cast<unsigned>(a)|static_cast<unsigned>(b)); }
inline vidix_fx_type_t operator&(vidix_fx_type_t a, vidix_fx_type_t b) { return static_cast<vidix_fx_type_t>(static_cast<unsigned>(a)&static_cast<unsigned>(b)); }
inline vidix_fx_type_t operator^(vidix_fx_type_t a, vidix_fx_type_t b) { return static_cast<vidix_fx_type_t>(static_cast<unsigned>(a)^static_cast<unsigned>(b)); }
inline vidix_fx_type_t operator|=(vidix_fx_type_t a, vidix_fx_type_t b) { return (a=static_cast<vidix_fx_type_t>(static_cast<unsigned>(a)|static_cast<unsigned>(b))); }
inline vidix_fx_type_t operator&=(vidix_fx_type_t a, vidix_fx_type_t b) { return (a=static_cast<vidix_fx_type_t>(static_cast<unsigned>(a)&static_cast<unsigned>(b))); }
inline vidix_fx_type_t operator^=(vidix_fx_type_t a, vidix_fx_type_t b) { return (a=static_cast<vidix_fx_type_t>(static_cast<unsigned>(a)^static_cast<unsigned>(b))); }
}
#endif

#endif

