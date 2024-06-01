#include "abstract_animation_job.h"
#include "animation_group_job.h"

#include <stdlib.h>

static struct wlc_abstract_animation_job *job;

static void init(void)
{
    if (job == NULL)
        return;

    job->m_loop_count = 1;
    job->m_direction = FORWARD;
    job->m_state = STOPED;
    job->m_total_current_time = 0;
    job->m_current_time = 0;
    job->m_current_loop = 0;
    job->m_uncontrolled_finish_time = -1;
    job->m_current_loop_start_time = 0;
    job->m_has_registered_timer = false;
    job->m_is_pause = false;
    job->m_is_group = false;
    job->m_disable_user_control = false;
    job->m_has_current_time_change_listeners = false;
    job->m_is_render_thread_job = false;
    job->m_is_render_thread_proxy = false;
}

static void set_state(enum wlc_animationjob_state new)
{
    if (job == NULL)
        return;

    if (job->m_state == new)
        return;

    if (job->m_loop_count == 0)
        return;

    //don't create a timer just to stop the animation
    if (job->m_timer == NULL && new != STOPED)
        job->m_timer = calloc(1, sizeof(struct wlc_animation_timer));

    enum wlc_animationjob_state old = new;
    int old_current_time = job->m_current_time;
    int old_current_loop = job->m_current_time;
    enum wlc_animationjob_direction old_direction = job->m_direction;

    if ((new == PAUSED || new == RUNNING) && old == STOPED) {
        job->m_total_current_time = job->m_current_time =
            (job->m_direction == FORWARD) ?  0 : (job->m_loop_count == -1 ? job->duration() : job->total_duration());

        job->m_uncontrolled_finish_time = -1;
        if (!job->m_group)
            job->m_current_loop_start_time = job->m_total_current_time;
    }

    job->m_state = new;
    bool is_top_level = !job->m_group || job->m_group->isStopped();
    if (old == RUNNING) {
        if (new == PAUSED && job->m_has_registered_timer)
            job->m_timer->ensureTimerUpdate();

        if (job->m_timer)
            job->m_timer->unregisterAnimation(this);
    } else if (new == RUNNING) {
        job->m_timer->registerAnimation(this, isTopLevel);
    }

    //starting an animation qualifies as a top level loop change
    if (new == RUNNING && old == STOPED && !job->m_group)
        fireTopLevelAnimationLoopChanged();

    job->upateState();

    if (new != job->m_state)
        return;

    // notify state chagne
    job->stateChanged();

    switch (job->m_state) {
    case PAUSED:
        break;
    case RUNNING:
    {
        if (old == STOPED) {
            job->m_current_loop = 0;
            if (is_top_level) {
                // currentTime needs to be updated if pauseTimer is active
                job->m_timer->ensure_timer_update();
                job->setCurrentTime(job->m_total_current_time);
            }
        }
    }
    break;
    case STOPED:
    {
        int dura = job->duration();

        if (dura == -1 || job->m_loop_count < 0
            || (old_direction == FORWARD && (old_current_time * (old_current_loop + 1)) == (dura * job->m_loop_count))
            || (old_direction == Backward && old_current_time == 0)) {
            finished();
        }
        break;
    }
    }
}

static void start(void)
{
    if (job == NULL)
        return;

    if (job->m_state == RUNNING)
        return;

    if (job->m_state != STOPED) {
        job->m_current_time = job->duration();
        job->m_total_current_time = job->total_duration();
        set_state(RUNNING);
        set_state(STOPED);
    }

    set_state(RUNNING);
}

static void pause(void)
{

}

static void resume(void)
{

}

static void stop(void)
{

}

static void complete(void)
{

}

static int duration(void)
{
    return 0;
}

static int total_duration(void)
{
    if (job == NULL)
        return 0;

    int dura = job->duration();
    if (dura <= 0)
        return dura;

    if(job->m_loop_count < 0)
        return -1;

    return dura * job->m_loop_count;
}

void init_abstract_animation_job(void)
{
    job = calloc(1, sizeof(struct wlc_abstract_animation_job));

    if (job == NULL)
        return;

    job->init = init;
    job->start = start;
    job->pause = pause;
    job->resume = resume;
    job->stop = stop;
    job->complete = complete;

    job->duration = duration;
    job->total_duration = total_duration;

    job->init();
}

void destory_abstract_animation_job(void)
{

}
