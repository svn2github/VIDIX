#include "vidixlibxx.h"

using namespace vidix;

Vidix& Vidix::operator =(const Vidix& _this) {
    vdlFreeCapabilityS(&cap);
    vdlFreeFourccS(&fourcc);
    vdlFreePlaybackS(&playback);
    vdlFreeYUVS(&yuv);
    vdlFreeRectS(&rect);
    vdlFreeGrKeyS(&grkey);
    vdlFreeVideoEqS(&video_eq);
    vdlFreeDeinterlaceS(&deint);
    vdlFreeDmaS(&dma);
    vdlFreeOemFxS(&oemfx);
    vdlClose(handle);

    handle=vdlOpen(_this.path.c_str(),_this.name.c_str(),_this.__cap,_this.__verbose);
    path=_this.path;
    name=_this.name;
    __cap=_this.__cap;
    __verbose=_this.__verbose;
    cap=*vdlAllocCapabilityS();
    fourcc=*vdlAllocFourccS();
    playback=*vdlAllocPlaybackS();
    yuv=*vdlAllocYUVS();
    rect=*vdlAllocRectS();
    grkey=*vdlAllocGrKeyS();
    video_eq=*vdlAllocVideoEqS();
    deint=*vdlAllocDeinterlaceS();
    dma=*vdlAllocDmaS();
    oemfx=*vdlAllocOemFxS();
    return *this;
}

Vidix::Vidix(const Vidix& _this)
     :	handle(vdlOpen(_this.path.c_str(),_this.name.c_str(),_this.__cap,_this.__verbose)),
	path(_this.path),
	name(_this.name),
	__cap(_this.__cap),
	__verbose(_this.__verbose),
	cap(*vdlAllocCapabilityS()),
	fourcc(*vdlAllocFourccS()),
	playback(*vdlAllocPlaybackS()),
	yuv(*vdlAllocYUVS()),
	rect(*vdlAllocRectS()),
	grkey(*vdlAllocGrKeyS()),
	video_eq(*vdlAllocVideoEqS()),
	deint(*vdlAllocDeinterlaceS()),
	dma(*vdlAllocDmaS()),
	oemfx(*vdlAllocOemFxS())
{
}

Vidix::Vidix(const std::string& _path,const std::string& _name,unsigned _cap,int _verbose)
     :	handle(vdlOpen(_path.c_str(),_name.c_str(),_cap,_verbose)),
	path(_path),
	name(_name),
	__cap(_cap),
	__verbose(_verbose),
	cap(*vdlAllocCapabilityS()),
	fourcc(*vdlAllocFourccS()),
	playback(*vdlAllocPlaybackS()),
	yuv(*vdlAllocYUVS()),
	rect(*vdlAllocRectS()),
	grkey(*vdlAllocGrKeyS()),
	video_eq(*vdlAllocVideoEqS()),
	deint(*vdlAllocDeinterlaceS()),
	dma(*vdlAllocDmaS()),
	oemfx(*vdlAllocOemFxS())
{
}

Vidix::Vidix(const std::string& _name,unsigned _cap,int _verbose)
     :	handle(vdlOpen(VIDIX_PATH,_name.c_str(),_cap,_verbose)),
	path(VIDIX_PATH),
	name(_name),
	__cap(_cap),
	__verbose(_verbose),
	cap(*vdlAllocCapabilityS()),
	fourcc(*vdlAllocFourccS()),
	playback(*vdlAllocPlaybackS()),
	yuv(*vdlAllocYUVS()),
	rect(*vdlAllocRectS()),
	grkey(*vdlAllocGrKeyS()),
	video_eq(*vdlAllocVideoEqS()),
	deint(*vdlAllocDeinterlaceS()),
	dma(*vdlAllocDmaS()),
	oemfx(*vdlAllocOemFxS())
{
}

Vidix::~Vidix() {
    vdlFreeCapabilityS(&cap);
    vdlFreeFourccS(&fourcc);
    vdlFreePlaybackS(&playback);
    vdlFreeYUVS(&yuv);
    vdlFreeRectS(&rect);
    vdlFreeGrKeyS(&grkey);
    vdlFreeVideoEqS(&video_eq);
    vdlFreeDeinterlaceS(&deint);
    vdlFreeDmaS(&dma);
    vdlFreeOemFxS(&oemfx);
    vdlClose(handle);
}

unsigned Vidix::version() const { return vdlGetVersion(); }
int	Vidix::get_capabilities() { return vdlGetCapability(handle,&cap); }
int	Vidix::query_fourcc() { return vdlQueryFourcc(handle,&fourcc); }
int	Vidix::config_playback() const { return vdlConfigPlayback(handle,&playback); }
int	Vidix::playback_on() const { return vdlPlaybackOn(handle); }
int	Vidix::playback_off() const { return vdlPlaybackOff(handle); }
int	Vidix::frame_select(unsigned frame_idx) const { return vdlPlaybackFrameSelect(handle,frame_idx); }
int	Vidix::get_gr_keys() { return vdlGetGrKeys(handle,&grkey); }
int	Vidix::set_gr_keys() const { return vdlSetGrKeys(handle,&grkey); }
int	Vidix::get_eq() { return vdlPlaybackGetEq(handle,&video_eq); }
int	Vidix::set_eq() const { return vdlPlaybackSetEq(handle,&video_eq); }
int	Vidix::get_deint() { return vdlPlaybackGetDeint(handle,&deint); }
int	Vidix::set_deint() const { return vdlPlaybackSetDeint(handle,&deint); }
int	Vidix::num_oemfx(unsigned& number) const { return vdlQueryNumOemEffects(handle,&number ); }
int	Vidix::get_oemfx() { return vdlGetOemEffect(handle,&oemfx); }
int	Vidix::set_oemfx() const { return vdlSetOemEffect(handle,&oemfx); }
int	Vidix::dma_copy_frame() const { return vdlPlaybackCopyFrame(handle,&dma); }
int	Vidix::dma_status() const { return vdlQueryDMAStatus(handle); };

