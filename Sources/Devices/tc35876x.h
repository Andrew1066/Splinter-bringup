/*
 * tc35876x.h
 *
 *  Created on: May 19, 2015
 *      Author: alexf
 */

#ifndef TC35876X_H_
#define TC35876X_H_


struct tegra_dc_mode {
    int pclk;
    int rated_pclk;
    int h_ref_to_sync;
    int v_ref_to_sync;
    int h_sync_width;
    int v_sync_width;
    int h_back_porch;
    int v_back_porch;
    int h_active;
    int v_active;
    int h_front_porch;
    int v_front_porch;
    int stereo_mode;
    uint32_t flags;
    uint8_t  avi_m;
    uint32_t vmode;
};

extern struct tegra_dc_mode mode_dual;
extern struct tegra_dc_mode mode_single;

void tc35876x_init(byte bus, byte dev);
#endif /* TC35876X_H_ */
