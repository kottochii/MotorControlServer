#pragma once

#include <signal.h>


struct signal_handler_t
{
    struct sigaction handler;
    signal_handler_t();
    static void on_sig_int(int s);
};