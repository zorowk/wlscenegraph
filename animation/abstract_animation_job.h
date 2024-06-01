/*
 * Copyright (C) 2022 Purism SPC
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#pragma once

#include <stdbool.h>
#include <wayland-server-core.h>

enum wlc_animationjob_direction {
    FORWARD,
    Backward
};

enum wlc_animationjob_state {
    STOPED,
    PAUSED,
    RUNNING
};

enum wlc_animationjob_changetype {
    COMPLETION = 0x01,
    STATE_CHANGE = 0x02,
    CURRENT_LOOP= 0x04,
    CURRENT_TIME = 0x08
};

struct wlc_abstract_animation_job;
struct wlc_animation_group_job;
struct wlc_animation_timer;

struct wlc_animation_job_change_listener {
    void (*finished)(struct wlc_abstract_animation_job *);
    void (*state_changed)(struct wlc_abstract_animation_job *, enum wlc_animationjob_state, enum wlc_animationjob_state);
    void (*current_loop)(struct wlc_abstract_animation_job *);
    void (*current_time_changed)(struct wlc_abstract_animation_job *, int);

    enum wlc_animationjob_changetype types;

    struct wl_list link;
};

struct wlc_abstract_animation_job {
    int m_loop_count;
    struct wlc_animation_group_job *m_group;
    enum wlc_animationjob_direction m_direction;

    enum wlc_animationjob_state m_state;
    int m_total_current_time;
    int m_current_time;
    int m_current_loop;
    //records the finish time for an uncontrolled animation (used by animation groups)
    int m_uncontrolled_finish_time;
    // used together with m_uncontrolledFinishTime
    int m_current_loop_start_time;

    struct wl_list m_change_listener;
    struct wlc_animation_timer *m_timer;

    bool m_has_registered_timer:1;
    bool m_is_pause:1;
    bool m_is_group:1;
    bool m_disable_user_control:1;
    bool m_has_current_time_change_listeners:1;
    bool m_is_render_thread_job:1;
    bool m_is_render_thread_proxy:1;

    struct wl_list link;

    void (*init)(void);
    void (*destory)(void);

    void (*set_current_time)(int msecs);

    void (*start)(void);
    void (*pause)(void);
    void (*resume)(void);
    void (*stop)(void);
    void (*complete)(void);

    int (*duration)(void);
    int (*total_duration)(void);
};

void init_abstract_animation_job(void);
void destory_abstract_animation_job(void);
