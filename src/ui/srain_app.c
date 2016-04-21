/**
 * @file srain_app.c
 * @brief Srain's application class implement
 * @author LastAvengers <lastavengers@outlook.com>
 * @version 1.0
 * @date 2016-03-01
 */

#include <gtk/gtk.h>
#include "srain_app.h"
#include "srain_window.h"
#include "srain_chan.h"
#include "meta.h"
#include "rc.h"
#include "theme.h"
#include "log.h"
#include "server.h"
#include "server_cmd.h"

G_DEFINE_TYPE(SrainApp, srain_app, GTK_TYPE_APPLICATION);

/* Only one SrainApp instance in one application */
SrainApp *srain_app = NULL;
SrainWindow *srain_win = NULL;

static void srain_app_init(SrainApp *self){
    if (srain_app) return;

    self->server_join = (ServerJoinFunc)server_join;
    self->server_part = (ServerPartFunc)server_part;
    self->server_send = (ServerSendFunc)server_send;
    self->server_cmd = (ServerCmdFunc)server_cmd;

    srain_app = self;

    return;
}

static void srain_app_activate(GtkApplication *app){
    if (srain_win){
        gtk_window_present(GTK_WINDOW(srain_win));
    } else {
        theme_init();
        srain_win = srain_window_new(SRAIN_APP(app));
        gtk_window_present(GTK_WINDOW(srain_win));
        rc_read();
    }
}

static void srain_app_class_init(SrainAppClass *class){
    G_APPLICATION_CLASS(class)->activate = (void *)(GApplication *)srain_app_activate;
}

SrainApp* srain_app_new(void){
    return g_object_new(SRAIN_TYPE_APP, "application-id", "org.gtk.srain", NULL);
}

SrainChan* srain_app_add_chan(const char *server_name, const char *chan_name){
    return srain_window_add_chan(srain_win, server_name, chan_name);
}

void srain_app_rm_chan(SrainChan *chan){
    return srain_window_rm_chan(srain_win, chan);
}

void srain_app_sys_msg(SrainChan *chan, SysMsgType type, const char *msg){
    return srain_chan_sys_msg_add(chan, type, msg);
}

void srain_app_send_msg(SrainChan *chan, const char *msg){
    return srain_chan_send_msg_add(chan, msg);
}

void srain_app_recv_msg(SrainChan *chan, const char *nick, const char *id, const char *msg){
    return srain_chan_recv_msg_add(chan, nick, id, msg);
}

void srain_app_user_join(SrainChan *chan, const char *nick, IRCUserType type, int notify){
    return srain_chan_user_list_add(chan, nick, type, notify);
}

void srain_app_user_part(SrainChan *chan, const char *nick, const char *reason){
    return srain_chan_user_list_rm(chan, nick, reason);
}

void srain_app_set_topic(SrainChan *chan, const char *topic){
    return srain_chan_set_topic(chan, topic);
}
