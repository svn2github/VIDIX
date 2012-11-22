/*
 * vidix.h
 * VIDIX - VIDeo Interface for *niX
 *   This interface is introduced as universal one to MPEG decoder,
 *   BES == Back End Scaler and YUV2RGB hw accelerators.
 * In the future it may be expanded up to capturing and audio things.
 * Main goal of this this interface imlpementation is providing DGA
 * everywhere where it's possible (unlike X11 and other).
 * Copyright 2002 Nickols_K
 * Licence: GPL
 * This interface is based on v4l2, fbvid.h, mga_vid.h projects
 * and personally my ideas.
 * NOTE: This interface is introduces as driver interface.
 * Don't use it for APP.
*/
#ifndef __VIDIX_H_INCLUDED
#define __VIDIX_H_INCLUDED 1

#define VIDIX_VERSION 150

#include "vidix_enums.h"
#include "vidix_types.h"
#include "vidixlib.h"

#endif
