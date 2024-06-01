/*
 * Copyright (C) 2022 Purism SPC
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#pragma once

#include <stdbool.h>

// /home/zoro/Downloads/wlrtoots/qtdeclarative/src/quick/util/qquickanimation_p_p.h:154

enum wlc_direction {
    GUI_THREAD = 0,
    RENDER_THREAD = 1,
    ANY_THREAD = 2
};

enum wlc_loops {
    INFINITE = -2,
};


struct wlc_abstract_animation {
    const struct wlc_abstract_animation_impl *impl;
    bool running:1;
    bool paused:1;
    bool always_run_to_end:1;
    bool component_commplete:1;
    bool disable_user_control:1;
    bool needs_deferred_set_running:1;

    int loop_count;

    void (*commence)();
    void (*animation_finished)();

    struct wl_list group;        // animation group

};
