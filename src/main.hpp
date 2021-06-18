#ifndef MAIN_HPP
# define MAIN_HPP

#define BUF_SIZE 5000000
//4096

#include "includes.hpp"
#include "response.hpp"
#include "request.hpp"
#include "config.hpp"
#include "utils.hpp"

struct Session
{
	enum			AcceptState {accept_client = 1, read_client = 2, write_client = 3, close_client = 4,
								 change_to_write = 5, change_to_read = 6};

	enum			SessionState {read = 0, write = 1, close = 2};

	ServerConf		*server_conf;

	int				sd;				// дискриптор сокета соединения
	SessionState	ss;				// состояние сессии читаем/пишем/закрыто
	// std::string		request;	// блокируем запись запроса (чтобы проходило бысро на больших запросах)
	std::string		answer;
	std::string 	answer_buf;

    std::string		client_ip;
	uint16_t		client_port;

	Request			request_obj;	// объект обработки данных запросов - сюда сохраняем и очищаем
};

#include "handlerequest.hpp"

#endif