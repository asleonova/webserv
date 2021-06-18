#include "includes.hpp"
#include "config.hpp"

#ifndef UTILS_HPP
#define UTILS_HPP

std::ostream		&operator<<( std::ostream &o, ServerConf const &i );

void				fatal_error(const char *error_str);
void				fatal_error(const char *error_str, const char *ip, const unsigned int port);

// void				debug_connect(Session const &cur_sess, Session::AcceptState client_state, ssize_t count);

char				*ws_itoa(int n);

#endif