/*
 * rhd.h
 * This	software has been released under the terms of the GNU Public
 * license. See	http://www.gnu.org/copyleft/gpl.html for details.
 *
 * This	collection of definition was written by	Nick Kurshev
 * It's	based on radeonfb, X11,	GATOS sources
 * and partly compatible with Rage128 set (in OV0, CAP0, CAP1 parts)
*/

#ifndef	_RADEONHD_H
#define	_RADEONHD_H

#define RHD_BUS_CNTL				0x5420
#define RHD_CONFIG_CNTL				0x5424
#define RHD_CONFIG_MEMSIZE			0x5428
#define RHD_CONFIG_F0_BASE			0x542C
#define RHD_CONFIG_APER_SIZE			0x5430
#define RHD_ROM_CNTL				0x1600
#	define RHD_SCK_OVERWRITE                  (1 << 1)
#	define RHD_SCK_PRESCALE_CRYSTAL_CLK_SHIFT 28
#	define RHD_SCK_PRESCALE_CRYSTAL_CLK_MASK  (0xf << 28)

#define RHD_BIOS_0_SCRATCH		0x1724
#	define RADEON_FP_PANEL_SCALABLE     (1 << 16)
#	define RADEON_FP_PANEL_SCALE_EN     (1 << 17)
#	define RADEON_FP_CHIP_SCALE_EN      (1 << 18)
#	define RADEON_DRIVER_BRIGHTNESS_EN  (1 << 26)
#	define RADEON_DISPLAY_ROT_MASK      (3 << 28)
#	define RADEON_DISPLAY_ROT_00        (0 << 28)
#	define RADEON_DISPLAY_ROT_90        (1 << 28)
#	define RADEON_DISPLAY_ROT_180       (2 << 28)
#	define RADEON_DISPLAY_ROT_270       (3 << 28)
#define RHD_BIOS_1_SCRATCH		0x1728
#define RHD_BIOS_2_SCRATCH		0x172c
#define RHD_BIOS_3_SCRATCH		0x1730
#define RHD_BIOS_4_SCRATCH		0x1734
#	define RADEON_CRT1_ATTACHED_MASK    (3 << 0)
#	define RADEON_CRT1_ATTACHED_MONO    (1 << 0)
#	define RADEON_CRT1_ATTACHED_COLOR   (2 << 0)
#	define RADEON_LCD1_ATTACHED         (1 << 2)
#	define RADEON_DFP1_ATTACHED         (1 << 3)
#	define RADEON_TV1_ATTACHED_MASK     (3 << 4)
#	define RADEON_TV1_ATTACHED_COMP     (1 << 4)
#	define RADEON_TV1_ATTACHED_SVIDEO   (2 << 4)
#	define RADEON_CRT2_ATTACHED_MASK    (3 << 8)
#	define RADEON_CRT2_ATTACHED_MONO    (1 << 8)
#	define RADEON_CRT2_ATTACHED_COLOR   (2 << 8)
#	define RADEON_DFP2_ATTACHED         (1 << 17)
#define RHD_BIOS_5_SCRATCH		0x1738
#	define RADEON_LCD1_ON               (1 << 0)
#	define RADEON_CRT1_ON               (1 << 1)
#	define RADEON_TV1_ON                (1 << 2)
#	define RADEON_DFP1_ON               (1 << 3)
#	define RADEON_CRT2_ON               (1 << 5)
#	define RADEON_CV1_ON                (1 << 6)
#	define RADEON_DFP2_ON               (1 << 7)
#	define RADEON_LCD1_CRTC_MASK        (1 << 8)
#	define RADEON_LCD1_CRTC_SHIFT       8
#	define RADEON_CRT1_CRTC_MASK        (1 << 9)
#	define RADEON_CRT1_CRTC_SHIFT       9
#	define RADEON_TV1_CRTC_MASK         (1 << 10)
#	define RADEON_TV1_CRTC_SHIFT        10
#	define RADEON_DFP1_CRTC_MASK        (1 << 11)
#	define RADEON_DFP1_CRTC_SHIFT       11
#	define RADEON_CRT2_CRTC_MASK        (1 << 12)
#	define RADEON_CRT2_CRTC_SHIFT       12
#	define RADEON_CV1_CRTC_MASK         (1 << 13)
#	define RADEON_CV1_CRTC_SHIFT        13
#	define RADEON_DFP2_CRTC_MASK        (1 << 14)
#	define RADEON_DFP2_CRTC_SHIFT       14
#define RHD_BIOS_6_SCRATCH		0x173c
#	define RADEON_ACC_MODE_CHANGE       (1 << 2)
#	define RADEON_EXT_DESKTOP_MODE      (1 << 3)
#	define RADEON_LCD_DPMS_ON           (1 << 20)
#	define RADEON_CRT_DPMS_ON           (1 << 21)
#	define RADEON_TV_DPMS_ON            (1 << 22)
#	define RADEON_DFP_DPMS_ON           (1 << 23)
#	define RADEON_DPMS_MASK             (3 << 24)
#	define RADEON_DPMS_ON               (0 << 24)
#	define RADEON_DPMS_STANDBY          (1 << 24)
#	define RADEON_DPMS_SUSPEND          (2 << 24)
#	define RADEON_DPMS_OFF              (3 << 24)
#	define RADEON_SCREEN_BLANKING       (1 << 26)
#	define RADEON_DRIVER_CRITICAL       (1 << 27)
#	define RADEON_DISPLAY_SWITCHING_DIS (1 << 30)
#define RHD_BIOS_7_SCRATCH		0x1740
#	define RADEON_SYS_HOTKEY            (1 << 10)
#	define RADEON_DRV_LOADED            (1 << 12)

#define RHD_D1CRTC_H_TOTAL		0x6000
#define RHD_D1CRTC_V_TOTAL		0x6020

#define RHD_D1GRPH_ENABLE		0x6100
#	define D1GRPH_ENABLE		0x00000001
#define RHD_D1GRPH_CONTOL		0x6104
#	define D1GRPH_DEPTH		(3<<0)
#	define D1GRPH_Z			(3<<4)
#	define D1GRPH_FORMAT		(7<<8)
#	define D1GRPH_TILE_COMPACT_EN	(1<12)
#	define D1GRPH_ADDR_TANSLATION_EN (1<<16)
#	define D1GRPH_PRIVILEGED_ACCESS_EN (1<<17)
#	define D1GRPH_ARRAY_MODE	(7<<20)
#	define D1GRPH_16BIT_ALPHA_MODE	(3<<24)
#	define D1GRPH_16BIT_FIXED_ALPHA_RANGE (7<<28)
#define RHD_D1GRPH_PRIMARY_SURFACE_ADDRESS	0x6110
#define RHD_D1GRPH_SECONDARY_SURFACE_ADDRESS	0x6118

#define RHD_D1OVL_ENABLE		0x6180
#	define D1OVL_ENABLE		0x00000001
#define RHD_D1OVL_CONTROL1		0x6184
#	define D1OVL_DEPTH_16BPP	0x00000001
#	define D1OVL_DEPTH_32BPP	0x00000002
#	define D1OVL_DEPTH_Z		(3<<4)
#	define D1OVL_FORMAT_ARGB	(0<<8)
#	define D1OVL_FORMAT_RGB		(1<<8)
#	define D1OVL_FORMAT_BGRA	(2<<8)
#	define D1OVL_TILE_COMPACT_EN	(1<<12)
#	define D1OVL_ADDRESS_TANSLATION_EN	(1<<16)
#	define D1OVL_PRIVILEGED_ACCESS_EN	(1<<17)
#	define D1OVL_ARRAY_LINEAR_GENERAL	(0<<20)
#	define D1OVL_ARRAY_LINEAR_ALIGNED	(1<<20)
#	define D1OVL_ARRAY_1D_TILED_THIN1	(2<<20)
#	define D1OVL_ARRAY_1D_TILED_THICK	(3<<20)
#	define D1OVL_ARRAY_2D_TILED_THIN1	(4<<20)
#	define D1OVL_ARRAY_2D_TILED_THIN2	(5<<20)
#	define D1OVL_ARRAY_2D_TILED_THIN4	(6<<20)
#	define D1OVL_ARRAY_2D_TILED_THICK	(7<<20)
#	define D1OVL_ARRAY_2B_TILED_THIN1	(8<<20)
#	define D1OVL_ARRAY_2B_TILED_THIN2	(9<<20)
#	define D1OVL_ARRAY_2B_TILED_THIN4	(10<<20)
#	define D1OVL_ARRAY_2B_TILED_THICK	(11<<20)
#	define D1OVL_ARRAY_3D_TILED_THIN1	(12<<20)
#	define D1OVL_ARRAY_3D_TILED_THICK	(13<<20)
#	define D1OVL_ARRAY_3B_TILED_THIN1	(14<<20)
#	define D1OVL_ARRAY_3B_TILED_THICK	(15<<20)
#	define D1OVL_COLOR_EXPANSION_MODE	(1<<24)
#define RHD_D1OVL_CONTROL2		0x6188
#	define D1OVL_HALF_RESOLUTION_ENABLE	0x0000001
#define RHD_D1OVL_SWAP_CNTL		0x618C
#	define D1OVL_ENDIAN_SWAP	0x00000003
#	define D1OVL_RED_CROSSBAR	(3<<4)
#	define D1OVL_GREEN_CROSSBAR	(3<<6)
#	define D1OVL_BLUE_CROSSBAR	(3<<8)
#	define D1OVL_ALPHA_CROSSBAR	(3<<10)
#define RHD_D1OVL_SURFACE_ADDRESS		0x6190
#	define D1OVL_DFQ_ENABLE		0x00000001
#	define D1OVL_SURFACE_ADDRESS	0xFFFFFF00
#define RHD_D1OVL_PITCH		0x6198
#	define D1OVL_PITCH		0x00001FF0
#define RHD_D1OVL_SURFACE_OFFSET_X	0x619C
#	define D1OVL_SURFACE_OFFSET_X	0x00000FF0
#define RHD_D1OVL_SURFACE_OFFSET_Y	0x61A0
#	define D1OVL_SURFACE_OFFSET_Y	0x00000FF0
#define RHD_D1OVL_START		0x61A4
#	define D1OVL_X_START		0x00000FFF
#	define D1OVL_Y_START		0x0FFF0000
#define RHD_D1OVL_END			0x61A8
#	define D1OVL_X_END		0x00001FFF
#	define D1OVL_Y_END		0x1FFF0000
#define RHD_D1OVL_UPDATE		0x61AC
#	define D1OVL_UPDATE_PENDING		0x00000001
#	define D1OVL_UPDATE_TAKING		0x00000002
#	define D1OVL_UPDATE_LOCK		0x00010000
#	define D1OVL_DISABLE_MULTIUPDATE	0x01000000
#define RHD_D1OVL_SURFACE_ADDRESS_INUSE	0x61B0
#	define D1OVL_SURFACE_ADDRESS_INUSE	0xFFFFFF00
#define RHD_D1OVL_DFQ_CONTROL		0x61B4 /* deep flip queue */
#	define D1OVL_DFQ_RESET			0x00000001
#	define D1OVL_DFQ_SIZE			(7<<4)
#	define D1OVL_DFQ_MIN_FREE_ENTIES	(7<<8)
#define RHD_D1OVL_DFQ_STATUS		0x61B8 /* deep flip queue */
#	define D1OVL_DFQ_NUM_ENTIES		(3<<0)
#	define D1OVL_DFQ_RESET_FLAG		(1<<8)
#	define D1OVL_DFQ_RESET_ASK		(1<<9)
#define RHD_D1OVL_COLOR_MATIX_TANSFORM_CNTL	0x6140
#	define D1OVL_NO_COLORSPACE_ADJ		0x00000000
#	define D1OVL_DISPLAY_COLORSPACE		0x00000001
#	define D1OVL_CCIRT_601			0x00000002
#	define D1OVL_CCIRT_709			0x00000003
#	define D1OVL_TVRGB			0x00000004
#define RHD_D1OVL_COLOR_MATIX_TANSFORM_EN	0x6200
#	define D1OVL_MATIX_TANSFORM_EN		0x00000001
#define RHD_D1OVL_MATRIX_COEFF_1_1		0x6204 /* format fix-point S3.11 */
#	define D1OVL_MATRIX_COEFF_1_1		0x0003FFF0 /* default 0x198a0*/
#	define D1OVL_MATIX_SIGN_1_1		0x80000000
#define RHD_D1OVL_MATRIX_COEFF_1_2		0x6208 /* format fix-point S3.11 */
#	define D1OVL_MATRIX_COEFF_1_2		0x0003FFF0 /* default 0x12a20*/
#	define D1OVL_MATIX_SIGN_1_2		0x80000000
#define RHD_D1OVL_MATRIX_COEFF_1_3		0x620C /* format fix-point S3.11 */
#	define D1OVL_MATRIX_COEFF_1_3		0x0003FFF0 /* default 0x0*/
#	define D1OVL_MATIX_SIGN_1_3		0x80000000
#define RHD_D1OVL_MATRIX_COEFF_1_4		0x6210 /* format fix-point S11.1 */
#	define D1OVL_MATRIX_COEFF_1_4		0x07FFFFF0 /* default 0x48700*/
#	define D1OVL_MATIX_SIGN_1_4		0x80000000
#define RHD_D1OVL_MATRIX_COEFF_2_1		0x6214 /* format fix-point S3.11 */
#	define D1OVL_MATRIX_COEFF_2_1		0x0003FFF0 /* default 0x72fe0*/
#	define D1OVL_MATIX_SIGN_2_1		0x80000000
#define RHD_D1OVL_MATRIX_COEFF_2_2		0x6218 /* format fix-point S3.11 */
#	define D1OVL_MATRIX_COEFF_2_2		0x0003FFF0 /* default 0x12a20*/
#	define D1OVL_MATIX_SIGN_2_2		0x80000000
#define RHD_D1OVL_MATRIX_COEFF_2_3		0x621C /* format fix-point S3.11 */
#	define D1OVL_MATRIX_COEFF_2_3		0x0003FFF0 /* default 0x79bc0*/
#	define D1OVL_MATIX_SIGN_2_3		0x80000000
#define RHD_D1OVL_MATRIX_COEFF_2_4		0x6220 /* format fix-point S11.1 */
#	define D1OVL_MATRIX_COEFF_2_4		0x07FFFFF0 /* default 0x22100*/
#	define D1OVL_MATIX_SIGN_2_4		0x80000000
#define RHD_D1OVL_MATRIX_COEFF_3_1		0x6224 /* format fix-point S3.11 */
#	define D1OVL_MATRIX_COEFF_3_1		0x0003FFF0 /* default 0x0*/
#	define D1OVL_MATIX_SIGN_3_1		0x80000000
#define RHD_D1OVL_MATRIX_COEFF_3_2		0x6228 /* format fix-point S3.11 */
#	define D1OVL_MATRIX_COEFF_3_2		0x0003FFF0 /* default 0x12a20*/
#	define D1OVL_MATIX_SIGN_3_2		0x80000000
#define RHD_D1OVL_MATRIX_COEFF_3_3		0x622C /* format fix-point S3.11 */
#	define D1OVL_MATRIX_COEFF_3_3		0x0003FFF0 /* default 0x20460*/
#	define D1OVL_MATIX_SIGN_3_3		0x80000000
#define RHD_D1OVL_MATRIX_COEFF_3_4		0x6230 /* format fix-point S11.1 */
#	define D1OVL_MATRIX_COEFF_3_4		0x07FFFFF0 /* default 0x3af80*/
#	define D1OVL_MATIX_SIGN_3_4		0x80000000
/* gamma correction */
#define RHD_D1OVL_PWL_TRANSFORM_EN		0x6280
#	define D1OVL_PWL_TANSFORM_EN		0x00000001
#define RHD_D1OVL_0TOF				0x6284 /* format fix-point 8.1 */
#	define D1OVL_OFFSET			0x000000FF
#	define D1OVL_SLOPE			0x07FF0000
#define RHD_D1OVL_10TO1F			0x6288 /* format fix-point 8.1 */
#define RHD_D1OVL_20TO3F			0x628C /* format fix-point 9.1 */
#define RHD_D1OVL_40TO7F			0x6290 /* format fix-point 9.1 */
#define RHD_D1OVL_80TOBF			0x6294 /* format fix-point 10.1 */
#define RHD_D1OVL_C0TOFF			0x6298 /* format fix-point 10.1 */
#define RHD_D1OVL_100TO13F			0x629C /* format fix-point 10.1 */
#define RHD_D1OVL_140TO17F			0x62A0 /* format fix-point 10.1 */
#define RHD_D1OVL_180TO1BF			0x62A4 /* format fix-point 10.1 */
#define RHD_D1OVL_1C0TO1FF			0x62A8 /* format fix-point 10.1 */
#define RHD_D1OVL_200TO23F			0x62AC /* format fix-point 10.1 */
#define RHD_D1OVL_240TO27F			0x62B0 /* format fix-point 10.1 */
#define RHD_D1OVL_280TO2BF			0x62B4 /* format fix-point 10.1 */
#define RHD_D1OVL_2C0TO2FF			0x62B8 /* format fix-point 10.1 */
#define RHD_D1OVL_300TO3FF			0x62BC /* format fix-point 10.1 */
#define RHD_D1OVL_340TO37F			0x62C0 /* format fix-point 10.1 */
#define RHD_D1OVL_380TO3BF			0x62C4 /* format fix-point 10.1 */
#define RHD_D1OVL_3C0TO3FF			0x62C8 /* format fix-point 10.1 */

#define RHD_D1OVL_KEY_CONTOL			0x6300
#	define D1GRPH_KEY_FUNCTION		0x0000003
#	define D1GRPH_FALSE			0
#	define D1GRPH_TRUE			1
#	define D1GRPH_RGBA			2
#	define D1GRPH_NOT_RGBA			3
#	define D1OVL_KEY_FUNCTION		(3<<8)
#	define D1OVL_FALSE			0
#	define D1OVL_TRUE			1
#	define D1OVL_RGBA			2
#	define D1OVL_NOT_RGBA			3
#	define D1OVL_KEY_COMPARE_MIX		(1<<16)
#	define D1OVL_COMPARE_OR			0
#	define D1OVL_COMPARE_AND		1

#define RHD_D1GRPH_ALPHA			0x6304
#	define D1GRPH_ALPHA_MASK		0xFF
#define RHD_D1OVL_ALPHA				0x6308
#	define D1OVL_ALPHA_MASK			0xFF
#define RHD_D1OVL_ALPHA_CONTROL			0x630C
#	define D1OVL_ALPHA_KEYER		0
#	define D1OVL_ALPHA_PERPIXEL_GR		1
#	define D1OVL_ALPHA_GLOBAL		2
#	define D1OVL_ALPHA_PERPIXEL_OVL		3
#	define D1OVL_ALPHA_PREMULT		(1<<8)
#	define D1OVL_ALPHA_INV			(1<<16)
#define RHD_D1GRPH_KEY_RANGE_RED		0x6310
#	define D1GRPH_KEY_LOW			0x0000FFFF
#	define D1GRPH_KEY_HIGH			0xFFFF0000
#define RHD_D1GRPH_KEY_RANGE_GREEN		0x6314
#define RHD_D1GRPH_KEY_RANGE_BLUE		0x6318
#define RHD_D1GRPH_KEY_RANGE_ALPHA		0x631C
#define RHD_D1OVL_KEY_RANGE_RED_CR		0x6320
#	define D1OVL_KEY_LOW			0x000001FF
#	define D1OVL_KEY_HIGH			0x01FF0000
#define RHD_D1OVL_KEY_RANGE_GREEN_Y		0x6324
#define RHD_D1OVL_KEY_RANGE_BLUE_CB		0x6328
#define RHD_D1OVL_KEY_RANGE_ALPHA		0x632C


#define RHD_D2CRTC_H_TOTAL			0x6800
#define RHD_D2CRTC_V_TOTAL			0x6820

#define RHD_D2GRPH_ENABLE			0x6900
#define RHD_D2GRPH_CONTOL			0x6904
#define RHD_D2GRPH_PRIMARY_SURFACE_ADDRESS	0x6910
#define RHD_D2GRPH_SECONDARY_SURFACE_ADDRESS	0x6918
#define RHD_D2OVL_ENABLE			0x6980
#define RHD_D2OVL_CONTROL1			0x6984
#define RHD_D2OVL_CONTROL2			0x6988
#define RHD_D2OVL_SWAP_CNTL			0x698C
#define RHD_D2OVL_SURFACE_ADDRESS		0x6990
#define RHD_D2OVL_PITCH				0x6998
#define RHD_D2OVL_SURFACE_OFFSET_X		0x699C
#define RHD_D2OVL_SURFACE_OFFSET_Y		0x69A0
#define RHD_D2OVL_START				0x69A4
#define RHD_D2OVL_END				0x69A8
#define RHD_D2OVL_UPDATE			0x69AC
#define RHD_D2OVL_SURFACE_ADDRESS_INUSE		0x69B0
#define RHD_D2OVL_DFQ_CONTROL			0x69B4 /* deep flip queue */
#define RHD_D2OVL_DFQ_STATUS			0x69B8 /* deep flip queue */
#define RHD_D2OVL_COLOR_MATIX_TANSFORM_CNTL	0x6940
#define RHD_D2OVL_COLOR_MATIX_TANSFORM_EN	0x6A00
#define RHD_D2OVL_MATRIX_COEFF_1_1		0x6A04 /* format fix-point S3.11 */
#define RHD_D2OVL_MATRIX_COEFF_1_2		0x6A08 /* format fix-point S3.11 */
#define RHD_D2OVL_MATRIX_COEFF_1_3		0x6A0C /* format fix-point S3.11 */
#define RHD_D2OVL_MATRIX_COEFF_1_4		0x6A10 /* format fix-point S11.1 */
#define RHD_D2OVL_MATRIX_COEFF_2_1		0x6A14 /* format fix-point S3.11 */
#define RHD_D2OVL_MATRIX_COEFF_2_2		0x6A18 /* format fix-point S3.11 */
#define RHD_D2OVL_MATRIX_COEFF_2_3		0x6A1C /* format fix-point S3.11 */
#define RHD_D2OVL_MATRIX_COEFF_2_4		0x6A20 /* format fix-point S11.1 */
#define RHD_D2OVL_MATRIX_COEFF_3_1		0x6A24 /* format fix-point S3.11 */
#define RHD_D2OVL_MATRIX_COEFF_3_2		0x6A28 /* format fix-point S3.11 */
#define RHD_D2OVL_MATRIX_COEFF_3_3		0x6A2C /* format fix-point S3.11 */
#define RHD_D2OVL_MATRIX_COEFF_3_4		0x6A30 /* format fix-point S11.1 */
/* gamma correction */
#define RHD_D2OVL_PWL_TRANSFORM_EN		0x6A80
#define RHD_D2OVL_0TOF				0x6A84 /* format fix-point 8.1 */
#define RHD_D2OVL_10TO1F			0x6A88 /* format fix-point 8.1 */
#define RHD_D2OVL_20TO3F			0x6A8C /* format fix-point 9.1 */
#define RHD_D2OVL_40TO7F			0x6A90 /* format fix-point 9.1 */
#define RHD_D2OVL_80TOBF			0x6A94 /* format fix-point 10.1 */
#define RHD_D2OVL_C0TOFF			0x6A98 /* format fix-point 10.1 */
#define RHD_D2OVL_100TO13F			0x6A9C /* format fix-point 10.1 */
#define RHD_D2OVL_140TO17F			0x6AA0 /* format fix-point 10.1 */
#define RHD_D2OVL_180TO1BF			0x6AA4 /* format fix-point 10.1 */
#define RHD_D2OVL_1C0TO1FF			0x6AA8 /* format fix-point 10.1 */
#define RHD_D2OVL_200TO23F			0x6AAC /* format fix-point 10.1 */
#define RHD_D2OVL_240TO27F			0x6AB0 /* format fix-point 10.1 */
#define RHD_D2OVL_280TO2BF			0x6AB4 /* format fix-point 10.1 */
#define RHD_D2OVL_2C0TO2FF			0x6AB8 /* format fix-point 10.1 */
#define RHD_D2OVL_300TO3FF			0x6ABC /* format fix-point 10.1 */
#define RHD_D2OVL_340TO37F			0x6AC0 /* format fix-point 10.1 */
#define RHD_D2OVL_380TO3BF			0x6AC4 /* format fix-point 10.1 */
#define RHD_D2OVL_3C0TO3FF			0x6AC8 /* format fix-point 10.1 */

#define RHD_D2OVL_KEY_CONTOL			0x6B00
#define RHD_D2GRPH_ALPHA			0x6B04
#define RHD_D2OVL_ALPHA				0x6B08
#define RHD_D2OVL_ALPHA_CONTROL			0x6B0C
#define RHD_D2GRPH_KEY_RANGE_RED		0x6B10
#define RHD_D2GRPH_KEY_RANGE_GREEN		0x6B14
#define RHD_D2GRPH_KEY_RANGE_BLUE		0x6B18
#define RHD_D2GRPH_KEY_RANGE_ALPHA		0x6B1C
#define RHD_D2OVL_KEY_RANGE_RED_CR		0x6B20
#define RHD_D2OVL_KEY_RANGE_GREEN_Y		0x6B24
#define RHD_D2OVL_KEY_RANGE_BLUE_CB		0x6B28
#define RHD_D2OVL_KEY_RANGE_ALPHA		0x6B2C

#endif	/* R600_H */
