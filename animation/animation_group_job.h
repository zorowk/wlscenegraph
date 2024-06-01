#pragma once

#include <stdbool.h>
#include <wayland-server-core.h>

struct wlc_abstract_animation_job;

// this is tree
struct wlc_animation_group_job {
    struct wlc_abstract_animation_job *ab_job;
};
