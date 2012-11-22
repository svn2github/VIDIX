#ifndef __VIDIXLIBXX_H_INCLUDED
#define __VIDIXLIBXX_H_INCLUDED 1

#include <stdlib.h>

#include "vidix.h"

namespace vidix {

    class Vidix {
	public:
	    Vidix(const char *path,const char *name,unsigned cap,int verbose);
	    Vidix(const char *name=NULL,unsigned cap=TYPE_OUTPUT,int verbose=0);
	    virtual ~Vidix();
	
	    int		is_error() const { return handle==NULL; }

	    unsigned	version() const;
	    int		get_capabilities();
	    int		query_fourcc();
	    int		config_playback() const;
	    int		playback_on() const;
	    int		playback_off() const;
	    int		frame_select(unsigned frame_idx) const;
	    int		get_gr_keys();
	    int		set_gr_keys() const;
	    int		get_eq();
	    int		set_eq() const;
	    int		get_deint();
	    int		set_deint() const;
	    int		num_oemfx(unsigned&) const;
	    int		get_oemfx();
	    int		set_oemfx() const;
	    int		dma_copy_frame() const;
	    int		dma_status() const;

	    vidix_capability_t&	cap;
	    vidix_fourcc_t&	fourcc;
	    vidix_playback_t&	playback;
	    vidix_yuv_t&	yuv;
	    vidix_rect_t&	rect;
	    vidix_grkey_t&	grkey;
	    vidix_video_eq_t&	video_eq;
	    vidix_deinterlace_t&deint;
	    vidix_dma_t&	dma;
	    vidix_oem_fx_t&	oemfx;
	private:
	    VDL_HANDLE handle;
    };

}

#endif
