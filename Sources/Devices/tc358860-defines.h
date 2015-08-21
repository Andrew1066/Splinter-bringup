/*
 * tc358860-defines.h
 *
 *  Created on: May 20, 2015
 *      Author: alexf
 */

#ifndef TC358860_DEFINES_H_
#define TC358860_DEFINES_H_

#define TC358860_IOB_CTRL1						0x0800
#define TC358860_WAIT0_CNT						0x1000
#define TC358860_BOOT_SET0						0x1004
#define TC358860_BOOT_SET1						0x1008
#define TC358860_BOOT_SET2						0x100C
#define TC358860_BOOT_SET3						0x1010
#define TC358860_BOOT_CTRL						0x1014
#define TC358860_BOOT_STATUS					0x1018
#define TC358860_AUX_TXHS_EN					0xB608
#define  TC358860_CHIP_ID                       0x0180

#define TC358860_MAX_DOWNSPREAD					0x8003

#define TC358860_MAX_LINK_RATE						0x8001
#define TC358860_MAX_LANE_CNT						0x8002
#define TC358860_						0x0800
#define TC358860_						0x0800
#define TC358860_						0x0800
#define TC358860_						0x0800
#define TC358860_						0x0800
#define TC358860_						0x0800
#define TC358860_						0x0800


#define  TC358860_DATA0_DPHY_TX_CTRL              0x0004
#define  TC358860_CLOCK_DPHY_RX_CTRL              0x0020
#define  TC358860_DATA3W_CTRL                     0x0050
#define  TC358860_DSI0_PPI_START                  0x0104
#define  TC358860_DSI0_PPI_LPTXTIMECNT            0x0114
#define  TC358860_DSI0_PPI_LANEENABLE             0x0134
#define  TC358860_DSI0_PPI_TX_RX_TA               0x013C
#define  TC358860_PPI_CLS_ANALOG_TMR              0x0140
#define  TC358860_PPI_D0S_ANALOG_TMR              0x0144
#define  TC358860_PPI_D1S_ANALOG_TMR              0x0148
#define  TC358860_PPI_D2S_ANALOG_TMR              0x014C
#define  TC358860_PPI_D3S_ANALOG_TMR              0x0150
#define  TC358860_DSI0_PPI_D0S_CLRSIPOCOUNT       0x0164
#define  TC358860_DSI0_PPI_D1S_CLRSIPOCOUNT       0x0168
#define  TC358860_DSI0_PPI_D2S_CLRSIPOCOUNT       0x016C
#define  TC358860_DSI0_PPI_D3S_CLRSIPOCOUNT       0x0170
#define  TC358860_DSI0_DSI_START                  0x0204
#define  TC358860_DSI0_DSI_LANEENABLE             0x0210
#define  TC358860_DSI_LANESTATUS0                 0x0214
#define  TC358860_DSI_LANESTATUS1                 0x0218
#define  TC358860_DSI_INTERRUPT_STATUS            0x0220
#define  TC358860_DSI_INTERRUPT_CLEAR             0x0228
#define  TC358860_VIDEO_FRAME_CTRL                0x0450
#define  TC358860_HORIZONTAL_TIME0                0x0454
#define  TC358860_HORIZONTAL_TIME1                0x0458
#define  TC358860_VERTICAL_TIME0                  0x045C
#define  TC358860_VERTICAL_TIME1                  0x0460
#define  TC358860_VIDEO_FRAME_UPDATE_ENABLE       0x0464
#define  TC358860_CMD_CTRL                        0x0480
#define  TC358860_LR_SIZE                         0x0484
#define  TC358860_PG_SIZE                         0x0488
#define  TC358860_RM_PXL                          0x048C
#define  TC358860_HORI_SCLR_HCOEF                 0x0490
#define  TC358860_HORI_SCLR_LCOEF                 0x0494
#define  TC358860_SYSTEM_CTRL                     0x0510
#define  TC358860_DP_CTRL                         0x0600
#define  TC358860_NVALUE_VIDEO_CLK_REGEN          0x0614
#define  TC358860_MVALUE_AUDIO_CLK                0x0628
#define  TC358860_NVALUE_AUDIO_CLK_REGEN          0x062C
#define  TC358860_VIDEO_FRAME_OUTPUT_DELAY        0x0644
#define  TC358860_VIDEO_FRAME_SIZE                0x0648
#define  TC358860_VIDEO_FRAME_START               0x064C
#define  TC358860_VIDEO_FRAME_ACTIVE_REGION_SIZE  0x0650
#define  TC358860_VIDEO_FRAME_SYNC_WIDTH          0x0654
#define  TC358860_DP_CONFIG                       0x0658
#define  TC358860_AUX_CHANNEL_CONFIG0             0x0660
#define  TC358860_AUX_CHANNEL_CONFIG1             0x0664
#define  TC358860_AUX_CHANNEL_DPCD_ADDR           0x0668
#define  TC358860_AUX_CHANNEL_DPCD_WR_DATA0       0x066C
#define  TC358860_AUX_CHANNEL_DPCD_RD_DATA0       0x067C
#define  TC358860_AUX_CHANNEL_DPCD_RD_DATA1       0x0680
#define  TC358860_AUX_CHANNEL_STATUS              0x068C
#define  TC358860_LINK_TRAINING_CTRL              0x06A0
#define  TC358860_LINK_TRAINING_LOOP_CTRL         0x06D8
#define  TC358860_LINK_TRAINING_STATUS            0x06D0
#define  TC358860_LINK_TRAINING_SINK_CONFIG       0x06E4
#define  TC358860_PHY_CTRL                        0x0800
#define  TC358860_LINK_CLK_PLL_CTRL               0x0900
#define  TC358860_STREAM_CLK_PLL_CTRL             0x0908
#define  TC358860_STREAM_CLK_PLL_PARAM            0x0914
#define  TC358860_SYSTEM_CLK_PARAM                0x0918
#define  TC358860_DSI1_PPI_START                  0x1104
#define  TC358860_DSI1_PPI_LPTXTIMECNT            0x1114
#define  TC358860_DSI1_PPI_LANEENABLE             0x1134
#define  TC358860_DSI1_PPI_TX_RX_TA               0x113C
#define  TC358860_DSI1_PPI_D0S_CLRSIPOCOUNT       0x1164
#define  TC358860_DSI1_PPI_D1S_CLRSIPOCOUNT       0x1168
#define  TC358860_DSI1_PPI_D2S_CLRSIPOCOUNT       0x116C
#define  TC358860_DSI1_PPI_D3S_CLRSIPOCOUNT       0x1170
#define  TC358860_DSI1_DSI_START                  0x1204
#define  TC358860_DSI1_DSI_LANEENABLE             0x1210


#endif /* TC358860_DEFINES_H_ */