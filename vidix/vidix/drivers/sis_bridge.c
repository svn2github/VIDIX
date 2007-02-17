/**
    Video bridge detection for SiS 300 and 310/325 series chips.

    Copyright 2003 Jake Page, Sugar Media.

    Based on SiS Xv driver:
    Copyright 2002-2003 by Thomas Winischhofer, Vienna, Austria.

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

**/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "libdha/libdha.h"
#include "sis_regs.h"
#include "sis_defs.h"


static void sis_ddc2_delay(unsigned short delaytime)
{
  unsigned short i;
  int temp;

  for (i = 0; i < delaytime; i++)
    inSISIDXREG(SISSR, 0x05, temp);
}

static int sis_do_sense(int type, int test)
{
  int temp, mytest, result, i, j;

  for (j = 0; j < 10; j++)
  {
    result = 0;
    for (i = 0; i < 3; i++)
    {
      mytest = test;
      outSISIDXREG(SISPART4, 0x11, (type & 0x00ff));
      temp = (type >> 8) | (mytest & 0x00ff);
      setSISIDXREG(SISPART4, 0x10, 0xe0, temp);
      sis_ddc2_delay(0x1500);
      mytest >>= 8;
      mytest &= 0x7f;
      inSISIDXREG(SISPART4, 0x03, temp);
      temp ^= 0x0e;
      temp &= mytest;
      if (temp == mytest)
        result++;
      outSISIDXREG(SISPART4, 0x11, 0x00);
      andSISIDXREG(SISPART4, 0x10, 0xe0);
      sis_ddc2_delay(0x1000);
    }
    if ((result == 0) || (result >= 2))
      break;
  }
  return result;
}

/* sense connected devices on 30x bridge */
static void sis_sense_30x(void)
{
  unsigned char backupP4_0d, backupP2_00, backupP2_4d, backupSR_1e;
  unsigned char biosflag = 0;
  unsigned short svhs = 0, svhs_c = 0;
  unsigned short cvbs = 0, cvbs_c = 0;
  unsigned short vga2 = 0, vga2_c = 0;
  int myflag, result;

  if (!(sis_vbflags & VB_SISBRIDGE))
    return;

  if (sis_vbflags & VB_301)
  {
    svhs = 0x00b9; cvbs = 0x00b3; vga2 = 0x00d1;
    inSISIDXREG(SISPART4, 0x01, myflag);
    if (myflag & 0x04)
    {
      svhs = 0x00dd; cvbs = 0x00ee; vga2 = 0x00fd;
    }
  }
  else if (sis_vbflags & (VB_301B | VB_302B))
  {
    svhs = 0x016b; cvbs = 0x0174; vga2 = 0x0190;
  }
  else if (sis_vbflags & (VB_301LV | VB_302LV))
  {
    svhs = 0x0200; cvbs = 0x0100;
  }
  else
    return;

  vga2_c = 0x0e08; svhs_c = 0x0404; cvbs_c = 0x0804;
  if (sis_vbflags & (VB_301LV | VB_302LV))
  {
    svhs_c = 0x0408; cvbs_c = 0x0808;
  }
  biosflag = 2;

  if (sis_vga_engine == SIS_300_VGA)
  {
    inSISIDXREG(SISSR, 0x3b, myflag);
    if (!(myflag & 0x01))
      vga2 = vga2_c = 0;
  }

  if (!(sis_vbflags & VB_SISBRIDGE))
    vga2 = vga2_c = 0;

  inSISIDXREG(SISSR, 0x1e, backupSR_1e);
  orSISIDXREG(SISSR, 0x1e, 0x20);
  inSISIDXREG(SISPART4, 0x0d, backupP4_0d);

  if (sis_vbflags & VB_301)
    setSISIDXREG(SISPART4, 0x0d, ~0x07, 0x05);
  else
    orSISIDXREG(SISPART4, 0x0d, 0x04);

  sis_ddc2_delay(0x2000);

  inSISIDXREG(SISPART2, 0x00, backupP2_00);
  outSISIDXREG(SISPART2, 0x00, ((backupP2_00 | 0x3c) & 0xfc));
  inSISIDXREG(SISPART2, 0x4d, backupP2_4d);

  sis_do_sense(0,0);

  andSISIDXREG(SISCR, 0x32, ~0x14);

  if (vga2_c || vga2)
  {
    if (sis_do_sense(vga2, vga2_c)) 
    {
      if (biosflag & 0x01) 
      {
        if (sis_verbose > 1)
          printf("[SiS] SiS30x: Detected TV connected to SCART output\n");
        sis_vbflags |= TV_SCART;
        orSISIDXREG(SISCR, 0x32, 0x04);
      }
      else
      {
        if (sis_verbose > 1)
          printf("[SiS] SiS30x: Detected secondary VGA connection\n");
        sis_vbflags |= VGA2_CONNECTED;
        orSISIDXREG(SISCR, 0x32, 0x10);
      }
    }
  }
  andSISIDXREG(SISCR, 0x32, 0x3f);
  andSISIDXREG(SISCR, 0x32, ~0x03);

  /* scanning for TV */
  if ((result = sis_do_sense(svhs, svhs_c)))
  {
    if (sis_verbose > 1)
      printf("[SiS] SiS30x: Detected TV connected to SVIDEO output\n");
    /* TW: So we can be sure that there IS a SVIDEO output */
    sis_vbflags |= TV_SVIDEO;
    orSISIDXREG(SISCR, 0x32, 0x02);
  }

  if ((biosflag & 0x02) || (!result))
  {
    if (sis_do_sense(cvbs, cvbs_c))
    {
      if (sis_verbose > 1)
        printf("[SiS] SiS30x: Detected TV connected to COMPOSITE output\n");
      sis_vbflags |= TV_AVIDEO;
      orSISIDXREG(SISCR, 0x32, 0x01);
    }
  }

  sis_do_sense(0, 0);

  outSISIDXREG(SISPART2, 0x00, backupP2_00);
  outSISIDXREG(SISPART4, 0x0d, backupP4_0d);
  outSISIDXREG(SISSR, 0x1e, backupSR_1e);
  outSISIDXREG(SISPART2, 0x00, backupP2_00);
}

static void sis_detect_crt1(void)
{
  unsigned char CR32;
  unsigned char CRT1Detected = 0;
  unsigned char OtherDevices = 0;

  if (!(sis_vbflags & VB_VIDEOBRIDGE))
  {
    sis_crt1_off = 0;
    return;
  }

  inSISIDXREG(SISCR, 0x32, CR32);

  if (CR32 & 0x20)
    CRT1Detected = 1;
  if (CR32 & 0x5F)
    OtherDevices = 1;

  if (sis_crt1_off == -1)
  {
    if (!CRT1Detected) 
    {
      /* BIOS detected no CRT1. */
      /* If other devices exist, switch it off */
      if (OtherDevices)
        sis_crt1_off = 1;
      else
        sis_crt1_off = 0;
    } 
    else
    {
      /* BIOS detected CRT1, leave/switch it on */
      sis_crt1_off = 0;
    }
  }
  
  if (sis_verbose > 0)
    printf("[SiS] %sCRT1 connection detected\n", sis_crt1_off ? "No " : "");
}

static void sis_detect_tv(void)
{
  unsigned char SR16, SR38, CR32, CR38 = 0, CR79;
  int temp = 0;

  if (!(sis_vbflags & VB_VIDEOBRIDGE))
    return;

  inSISIDXREG(SISCR, 0x32, CR32);
  inSISIDXREG(SISSR, 0x16, SR16);
  inSISIDXREG(SISSR, 0x38, SR38);
  switch (sis_vga_engine)
  {
    case SIS_300_VGA:
      if (sis_device_id == DEVICE_SIS_630_VGA)
        temp = 0x35;
      break;
    case SIS_315_VGA:
      temp = 0x38;
      break;
  }

  if (temp)
    inSISIDXREG(SISCR, temp, CR38);

  if (CR32 & 0x47)
    sis_vbflags |= CRT2_TV;

  if (CR32 & 0x04)
    sis_vbflags |= TV_SCART;
  else if (CR32 & 0x02)
    sis_vbflags |= TV_SVIDEO;
  else if (CR32 & 0x01)
    sis_vbflags |= TV_AVIDEO;
  else if (CR32 & 0x40)
    sis_vbflags |= (TV_SVIDEO | TV_HIVISION);
  else if ((CR38 & 0x04) && (sis_vbflags & (VB_301LV | VB_302LV)))
    sis_vbflags |= TV_HIVISION_LV;
  else if ((CR38 & 0x04) && (sis_vbflags & VB_CHRONTEL))
    sis_vbflags |= (TV_CHSCART | TV_PAL);
  else if ((CR38 & 0x08) && (sis_vbflags & VB_CHRONTEL))
    sis_vbflags |= (TV_CHHDTV | TV_NTSC);

  if (sis_vbflags & (TV_SCART | TV_SVIDEO | TV_AVIDEO | TV_HIVISION))
  {
    if (sis_vga_engine == SIS_300_VGA)
    {
      /* TW: Should be SR38 here as well, but this
       *     does not work. Looks like a BIOS bug (2.04.5c).
       */
      if (SR16 & 0x20)
        sis_vbflags |= TV_PAL;
      else
        sis_vbflags |= TV_NTSC;
    }
   else if ((sis_device_id == DEVICE_SIS_550_VGA))
   {
     inSISIDXREG(SISCR, 0x79, CR79);
     if (CR79 & 0x08)
     {
       inSISIDXREG(SISCR, 0x79, CR79);
       CR79 >>= 5;
     }
     if (CR79 & 0x01)
     {
       sis_vbflags |= TV_PAL;
       if (CR38 & 0x40)
         sis_vbflags |= TV_PALM;
       else if (CR38 & 0x80)
         sis_vbflags |= TV_PALN;
     }
     else
       sis_vbflags |= TV_NTSC;
    }
    else if ((sis_device_id == DEVICE_SIS_650_VGA))
    {
      inSISIDXREG(SISCR, 0x79, CR79);
      if (CR79 & 0x20)
      {
        sis_vbflags |= TV_PAL;
        if (CR38 & 0x40)
            sis_vbflags |= TV_PALM;
        else if (CR38 & 0x80)
            sis_vbflags |= TV_PALN;
      }
      else
        sis_vbflags |= TV_NTSC;
    }
    else 
    {        /* 315, 330 */
      if (SR38 & 0x01)
      {
        sis_vbflags |= TV_PAL;
        if (CR38 & 0x40)
          sis_vbflags |= TV_PALM;
        else if (CR38 & 0x80)
          sis_vbflags |= TV_PALN;
      }
      else
        sis_vbflags |= TV_NTSC;
    }
  }

  if (sis_vbflags &
     (TV_SCART | TV_SVIDEO | TV_AVIDEO | TV_HIVISION | TV_CHSCART |
     TV_CHHDTV)) 
  {
    if (sis_verbose > 0)
      printf("[SiS] %sTV standard %s\n",
            ( sis_vbflags & (TV_CHSCART | TV_CHHDTV) ) ? 
            "Using " :"Detected default ",
            (sis_vbflags & TV_NTSC) ? ((sis_vbflags & TV_CHHDTV) ?
            "480i HDTV" : "NTSC")
            : ((sis_vbflags & TV_PALM) ? "PALM"
            : ((sis_vbflags & TV_PALN) ? "PALN" : "PAL")));
  }
}


static void sis_detect_crt2(void)
{
  unsigned char CR32;

  if (!(sis_vbflags & VB_VIDEOBRIDGE))
    return;

  /* CRT2-VGA not supported on LVDS and 30xLV */
  if (sis_vbflags & (VB_LVDS | VB_301LV | VB_302LV))
    return;

  inSISIDXREG(SISCR, 0x32, CR32);

  if (CR32 & 0x10)
    sis_vbflags |= CRT2_VGA;
}


/* Preinit: detect video bridge and sense connected devs */
static void sis_detect_video_bridge(void)
{
  int temp, temp1, temp2;

  sis_vbflags = 0;

  if (sis_vga_engine != SIS_300_VGA && sis_vga_engine != SIS_315_VGA)
    return;

  inSISIDXREG(SISPART4, 0x00, temp);
  temp &= 0x0F;
  if (temp == 1)
  {
    inSISIDXREG(SISPART4, 0x01, temp1);
    temp1 &= 0xf0;
    if (temp1 == 0xE0)
    {
      inSISIDXREG(SISPART4, 0x39, temp2);
      if (temp2 == 0xff)
      {
      sis_vbflags |= VB_302LV;
      if (sis_verbose > 1)
        printf("[SiS] Detected SiS302LV video bridge (ID 1; Revision 0x%x)\n",
             temp1);

      }
    } 
    else if (temp1 == 0xD0)
    {
      sis_vbflags |= VB_301LV;
      if (sis_verbose > 1)
        printf("[SiS] Detected SiS301LV video bridge (ID 1; Revision 0x%x)\n",
             temp1);
    }
    else if (temp1 == 0xB0)
    {
      sis_vbflags |= VB_301B;
      inSISIDXREG(SISPART4, 0x23, temp2);
      if (!(temp2 & 0x02))
        sis_vbflags |= VB_30xBDH;
      if (sis_verbose > 1)
        printf("[SiS] Detected SiS301B%s video bridge (Revision 0x%x)\n",
             (temp2 & 0x02) ? "" : " (DH)", temp1);
    }
    else if (temp1 <= 0xA0)
    {
      sis_vbflags |= VB_301;
      if (sis_verbose > 1)
        printf("[SiS] Detected SiS301 video bridge (Revision 0x%x)\n", temp1);
    }

    sis_sense_30x();

  }
  else if (temp == 2)
  {
    inSISIDXREG(SISPART4, 0x01, temp1);
    temp1 &= 0xf0;
    if (temp1 == 0xE0)
    {
      sis_vbflags |= VB_302LV;
      if (sis_verbose > 1)
        printf("[SiS] Detected SiS302LV video bridge (ID 2; Revision 0x%x)\n",
             temp1);
      
    } 
    else if (temp1 == 0xD0)
    {
      sis_vbflags |= VB_301LV;
      if (sis_verbose > 1)
        printf("[SiS] Detected SiS301LV video bridge (ID 2; Revision 0x%x)\n",
             temp1);
    }
    else if (temp1 <= 0xC0)
    {
      sis_vbflags |= VB_302B;
      inSISIDXREG(SISPART4, 0x23, temp2);
      if (!(temp & 0x02))
        sis_vbflags |= VB_30xBDH;
      if (sis_verbose > 1)
        printf("[SiS] Detected SiS302B%s video bridge (Revision 0x%x)\n",
             (temp2 & 0x02) ? "" : " (DH)", temp1);
    }

    sis_sense_30x();

  }
  else if (temp == 3)
  {
    if (sis_verbose > 1)
        printf("[SiS] Detected SiS303 video bridge - not supported\n");
  }
  else
  {
    if (sis_verbose > 1)
        printf ("[SiS] Detected Chrontel video bridge - not supported\n");
  }

  /* this is probably not relevant to video overlay driver... */
  /* detects if brdige uses LCDA for low res text modes */
  if (sis_vga_engine == SIS_315_VGA)
  {
    if (sis_vbflags & (VB_302B | VB_301LV | VB_302LV))
    {
      inSISIDXREG(SISCR, 0x34, temp);
      if (temp <= 0x13) 
      {
        inSISIDXREG(SISCR, 0x38, temp);
        if ((temp & 0x03) == 0x03) 
        {
          sis_vbflags |= VB_USELCDA;
        }
        else 
        {
          inSISIDXREG(SISCR, 0x30, temp);
          if (temp & 0x20) 
          {
            inSISIDXREG(SISPART1, 0x13, temp);
            if (temp & 0x40)
                sis_vbflags |= VB_USELCDA;
          }
        }
      }
      /*
      if (sis_vbflags & VB_USELCDA) 
      {
        printf("Bridge uses LCDA for low resolution and text modes\n");
      }
      */
    }
  }
}


/* detect video bridge type and sense connected devices */
void sis_init_video_bridge(void)
{
  sis_detect_video_bridge();
  sis_detect_crt1();
  sis_detect_tv();
  sis_detect_crt2();
  sis_detected_crt2_devices = sis_vbflags & (CRT2_LCD | CRT2_TV | CRT2_VGA);

  /* force crt2 type */
  if (sis_force_crt2_type == CRT2_DEFAULT)
  {
    if (sis_vbflags & CRT2_VGA)
      sis_force_crt2_type = CRT2_VGA;
    else if (sis_vbflags & CRT2_LCD)
      sis_force_crt2_type = CRT2_LCD;
    else if (sis_vbflags & CRT2_TV)
      sis_force_crt2_type = CRT2_TV;
  }

  switch (sis_force_crt2_type)
  {
    case CRT2_TV:
      sis_vbflags = sis_vbflags & ~(CRT2_LCD | CRT2_VGA);
      if (sis_vbflags & VB_VIDEOBRIDGE)
        sis_vbflags = sis_vbflags | CRT2_TV;
      else
        sis_vbflags = sis_vbflags & ~(CRT2_TV);
      break;
    case CRT2_LCD:
      sis_vbflags = sis_vbflags & ~(CRT2_TV | CRT2_VGA);
      if (sis_vbflags & VB_VIDEOBRIDGE)
        sis_vbflags = sis_vbflags | CRT2_LCD;
      else
      {
        sis_vbflags = sis_vbflags & ~(CRT2_LCD);
        if (sis_verbose > 0)
          printf("[SiS] Can't force CRT2 to LCD, no panel detected\n");
      }
      break;
    case CRT2_VGA:
      if (sis_vbflags & VB_LVDS)
      {
        if (sis_verbose > 0)
          printf("[SiS] LVDS does not support secondary VGA\n");
        break;
      }
      if (sis_vbflags & (VB_301LV | VB_302LV))
      {
        if (sis_verbose > 0)
        printf("[SiS] SiS30xLV bridge does not support secondary VGA\n");
        break;
      }
      sis_vbflags = sis_vbflags & ~(CRT2_TV | CRT2_LCD);
      if (sis_vbflags & VB_VIDEOBRIDGE)
        sis_vbflags = sis_vbflags | CRT2_VGA;
      else
        sis_vbflags = sis_vbflags & ~(CRT2_VGA);
      break;
    default:
      sis_vbflags &= ~(CRT2_TV | CRT2_LCD | CRT2_VGA);
  }

  /* CRT2 gamma correction?? */

  /* other force modes: */
  /*  have a 'force tv type' (svideo, composite, scart) option? */
  /*  have a 'force crt1 type' (to turn it off, etc??) */

  /* TW: Check if CRT1 used (or needed; this eg. if no CRT2 detected) */
  if (sis_vbflags & VB_VIDEOBRIDGE)
  {

    /* TW: No CRT2 output? Then we NEED CRT1!
     *     We also need CRT1 if depth = 8 and bridge=LVDS|630+301B
     */
    if ((!(sis_vbflags & (CRT2_VGA | CRT2_LCD | CRT2_TV))) || 
        ( ((sis_vbflags & (VB_LVDS | VB_CHRONTEL)) || 
        ((sis_vga_engine == SIS_300_VGA) && (sis_vbflags & VB_301B))))) 
      sis_crt1_off = 0;
    
    /* TW: No CRT2 output? Then we can't use hw overlay on CRT2 */
    if (!(sis_vbflags & (CRT2_VGA | CRT2_LCD | CRT2_TV)))
        sis_overlay_on_crt1 = 1;

  }
  else 
  { /* TW: no video bridge? */

    /* Then we NEED CRT1... */
    sis_crt1_off = 0;
    /* ... and can't use CRT2 for overlay output */
    sis_overlay_on_crt1 = 1;
  }

  /* tvstandard options ? */

  /* determine using CRT1 or CRT2? */
  /* -> NO dualhead right now... */
  if (sis_vbflags & DISPTYPE_DISP2)
  {
    if (sis_crt1_off)
    {
      sis_vbflags |= VB_DISPMODE_SINGLE;
      /* TW: No CRT1? Then we use the video overlay on CRT2 */
      sis_overlay_on_crt1 = 0;
    }
    else  /* TW: CRT1 and CRT2 - mirror or dual head ----- */
      sis_vbflags |= (VB_DISPMODE_MIRROR | DISPTYPE_CRT1);
  }
  else    /* TW: CRT1 only ------------------------------- */
    sis_vbflags |= (VB_DISPMODE_SINGLE | DISPTYPE_CRT1);

  if (sis_verbose > 0)
    printf("[SiS] Using hardware overlay on CRT%d\n",
      sis_overlay_on_crt1 ? 1 : 2);

}
