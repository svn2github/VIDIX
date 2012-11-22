#ifndef __VIDIX_TYPES_H_INCLUDED
#define __VIDIX_TYPES_H_INCLUDED 1

typedef struct vidix_capability_s
{
  char name[64]; /* Driver name */
  char author[64]; /* Author name */
  vidix_dev_type_t type;
  unsigned reserved0[4];
  int maxwidth;
  int maxheight;
  int minwidth;
  int minheight;
  int maxframerate; /* -1 if unlimited */
  vidix_dev_flag_t flags;
  unsigned short vendor_id;
  unsigned short device_id;
  unsigned reserved1[4];
} vidix_capability_t;

typedef struct vidix_fourcc_s
{
  unsigned fourcc;      /* input: requested fourcc */
  unsigned srcw;        /* input: hint: width of source */
  unsigned srch;	/* input: hint: height of source */
  vidix_depth_t depth;	/* output: screen depth for given fourcc */
  vidix_cap_t flags;	/* output: capability */
} vidix_fourcc_t;

typedef struct vidix_yuv_s
{
  unsigned y,u,v,a;
} vidix_yuv_t;

typedef struct vidix_rect_s
{
  unsigned x,y,w,h;	/* in pixels */
  vidix_yuv_t pitch;	/* line-align in bytes */
} vidix_rect_t;

typedef struct vidix_color_key_s
{
  vidix_color_key_op_t op;	/* defines logical operation */
  unsigned char	red;
  unsigned char	green;
  unsigned char	blue;
  unsigned char	reserved;
}vidix_ckey_t;

typedef struct vidix_video_key_s {
  vidix_video_key_op_t op;	/* defines logical operation */
  unsigned char	key[8];
} vidix_vkey_t;

#define VID_PLAY_MAXFRAMES 1024	/* unreal limitation */

typedef struct vidix_playback_s
{
  unsigned fourcc;		/* app -> driver: movies's fourcc */
  unsigned capability;	        /* app -> driver: what capability to use */
  unsigned blend_factor;	/* app -> driver: blending factor */
  vidix_rect_t src;             /* app -> driver: original movie size */
  vidix_rect_t dest;            /* app -> driver: destinition movie size.
                                   driver->app dest_pitch */
  vidix_interleave_t flags;     /* driver -> app: interleaved UV planes */
  /* memory model */
  unsigned frame_size;		/* driver -> app: destinition frame size */
  unsigned num_frames;		/* app -> driver: after call: driver -> app */
  unsigned offsets[VID_PLAY_MAXFRAMES];	/* driver -> app */
  vidix_yuv_t offset;		/* driver -> app: relative offsets
                                   within frame for yuv planes */
  void *dga_addr;		/* driver -> app: linear address */
} vidix_playback_t;

typedef struct vidix_grkey_s
{
  vidix_ckey_t ckey;		/* app -> driver: color key */
  vidix_vkey_t vkey;		/* app -> driver: video key */
  vidix_key_op_t key_op;	/* app -> driver: keys operations */
} vidix_grkey_t;

typedef struct vidix_video_eq_s {
  vidix_veq_cap_t cap;	 /* on get_eq should contain capability of
                            equalizer on set_eq should contain using fields */
  /* end-user app can have presets like: cold-normal-hot picture and so on */
  int brightness;	        /* -1000 : +1000 */
  int contrast;	                /* -1000 : +1000 */
  int saturation;	        /* -1000 : +1000 */
  int hue;		        /* -1000 : +1000 */
  int red_intensity;	        /* -1000 : +1000 */
  int green_intensity;          /* -1000 : +1000 */
  int blue_intensity;           /* -1000 : +1000 */
  vidix_veq_flag_t flags;	/* currently specifies ITU YCrCb color
                                   space to use */
} vidix_video_eq_t;

typedef struct vidix_deinterlace_s {
  vidix_interlace_flag_t flags;
  unsigned deinterlace_pattern;	/* app -> driver: deinterlace pattern if
                                   flag CFG_UNIQUE_INTERLACING is set */
} vidix_deinterlace_t;

typedef struct vidix_slice_s {
  void *address;		/* app -> driver */
  unsigned size;		/* app -> driver */
  vidix_rect_t slice;		/* app -> driver */
} vidix_slice_t;

typedef struct vidix_dma_s
{
  void *src;		 /* app -> driver. Virtual address of source */
  unsigned dest_offset;	 /* app -> driver.
                            Destination offset within of video memory */
  unsigned size;	 /* app -> driver. Size of transaction */
  vidix_bm_flag_t flags; /* app -> driver */
  unsigned idx;		 /* app -> driver: idx of src buffer */
  void *internal[VID_PLAY_MAXFRAMES];	/* for internal use by driver */
} vidix_dma_t;

/*
   This structure was introdused to support OEM effects like:
   - sharpness
   - exposure
   - (auto)gain
   - H(V)flip
   - black level
   - white balance
   and many other
*/
typedef struct vidix_oem_fx_s
{
  vidix_fx_type_t type;	/* type of effects */
  int num;		/* app -> driver: effect number.
                           From 0 to max number of effects */
  int minvalue;		/* min value of effect. 0 - for boolean */
  int maxvalue;		/* max value of effect. 1 - for boolean */
  int value;   	        /* current value of effect on get; required on set */
  char *name[80];	/* effect name to display */
} vidix_oem_fx_t;

#endif
