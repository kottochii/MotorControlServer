#include "signal_handler.hpp"

struct signal_handler_t signal_handler;

signal_handler_t::signal_handler_t()
{
    handler.sa_handler = on_sig_int;
    sigemptyset(&handler.sa_mask);
    handler.sa_flags = 0;
    sigaction(SIGINT, &handler, NULL);
}