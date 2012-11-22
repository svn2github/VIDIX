#ifndef __VIDIX_DRIVER_H_INCLUDED
#define __VIDIX_DRIVER_H_INCLUDED 1

#ifdef __cplusplus
extern "C" {
#endif

/* returns driver version */
extern unsigned vixGetVersion (void);

#define PROBE_NORMAL	0 /* normal probing */
#define PROBE_FORCE	1 /* ignore device_id but recognize device if it's known */

/* Probes video hw.
   verbose - specifies verbose level.
   force   - specifies force mode - driver should ignore
   device_id (danger but useful for new devices)
   Returns 0 if ok else errno */
extern int vixProbe (int verbose, int force);

/* Initializes driver.
   args	    - specifies driver specific parameters
   Returns 0 if ok else errno */
extern int vixInit (const char *args);

/* Destroys driver */
extern void vixDestroy (void);

/* Should fill at least type before init.
   Returns 0 if ok else errno */
extern int vixGetCapability (vidix_capability_t *);

/* Returns 0 if ok else errno */
extern int vixQueryFourcc (vidix_fourcc_t *);

/* Returns 0 if ok else errno */
extern int vixConfigPlayback (vidix_playback_t *);

/* Returns 0 if ok else errno */
extern int vixPlaybackOn (void);

/* Returns 0 if ok else errno */
extern int vixPlaybackOff (void);

/* Returns 0 if ok else errno */
extern int vixPlaybackFrameSelect (unsigned frame_idx);

/* Returns 0 if ok else errno */
extern int vixGetGrKeys (vidix_grkey_t *);

/* Returns 0 if ok else errno */
extern int vixSetGrKeys (const vidix_grkey_t *);

/* Returns 0 if ok else errno */
extern int vixPlaybackGetEq (vidix_video_eq_t *);

/* Returns 0 if ok else errno */
extern int vixPlaybackSetEq (const vidix_video_eq_t *);

/* Returns 0 if ok else errno */
extern int vixPlaybackGetDeint (vidix_deinterlace_t *);

/* Returns 0 if ok else errno */
extern int vixPlaybackSetDeint (const vidix_deinterlace_t *);

/* Returns 0 if ok else errno */
extern int vixPlaybackCopyFrame (vidix_dma_t *);

/* Returns 0 if DMA is available else errno (EBUSY) */
extern int vixQueryDMAStatus (void);

/* Returns 0 if ok else errno */
extern int vixQueryNumOemEffects (unsigned * number);

/* Returns 0 if ok else errno */
extern int vixGetOemEffect (vidix_oem_fx_t *);

/* Returns 0 if ok else errno */
extern int vixSetOemEffect (const vidix_oem_fx_t *);

#ifdef VIDIX_BUILD_STATIC
#define VIDIX_NAME(name) VIDIX_STATIC##name
#else
#define VIDIX_NAME(name) name
#endif

#ifdef __cplusplus
}
#endif

#endif
