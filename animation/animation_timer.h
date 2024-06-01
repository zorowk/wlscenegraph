/*
 * Copyright (C) 2022 Purism SPC
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#pragma once

#include <stdbool.h>
#include <wayland-server-core.h>

struct wlc_animation_timer {
    bool is_registered;
    bool is_paused;
    int pause_duration;

    __int64_t last_tick;
    int current_animation_idx;
    bool inside_tick;
    bool start_animation_pending;
    bool stop_timer_pending;

    struct wl_list animations;
    struct wl_list animations_to_start;

    // this is the count of running animations that are not a group neither a pause animation
    int running_leaf_animations;
    struct wl_list runing_pause_animations;


    /*
        this is used for updating the currentTime of all animations in case the pause
        timer is active or, otherwise, only of the animation passed as parameter.
    */
    void (*ensure_timer_update)(void);

    /*
        this will evaluate the need of restarting the pause timer in case there is still
        some pause animations running.
    */
    void (*update_animation_timer)(void);

    void (*restart_animation_timer)(void);
    void (*update_animations_time)(__int64_t time_step);

    int (*running_animation_count)(void);

    bool (*has_start_animationo_pending)(void);

    void (*start_animations)(void);
    void (*stop_timer)(void);
};

void wlc_
