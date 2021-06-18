#ifndef HANDLEREQUEST_HPP
# define HANDLEREQUEST_HPP

#include "includes.hpp"
#include "response.hpp"

// for delete
#include <cstdio>

// for struct Session
#include "main.hpp"

// for remove
#include <cstdio>

// структура для понимания структуры запрошенного файла (можно перенести в HandleRequest)
struct PathComponents
{
	enum PathType
	{
		PATH_TO_FILE = 0,
		DIR = 1,
		NON_EXISTING_FILE = 2,
		NON_EXISTING_DIR = 3
	};

	std::string		_path;
	std::string		_index_page_name;
	PathType		_pathType;

	// req_target - запрошенный ресурс на сервере
	// root - путь к локации в конфиге
	PathComponents(const std::string &req_target, const std::string &root, const std::string &index_page_name, size_t location_len);
};

// объявляение прототипа для получения типа по запрошенному файлу
// можно отдавать в класс респонс - для автоматического определения
std::string ReturnContentTypeByExtention(std::string &file_name);

struct HandleRequest
{
	// локация на стеке - заполняется исходя из запроса пользователя и списка локаций в конфиге сервера
	Location	_location;

	// получение типа до файла - обрезание лишних слешей - можно убрать
	std::string _getPagePath(const Request &req);

	// ********************************************************************************************************************
	// основной метод обработки запроса - возвращает объект ответа (или вызывает ошибку - которая возвращает тот же объект)
	// при вызове отдаем распарсенный запрос от пользователя и ссылку на конфиг сервера куда пришел запрос (ip:port)
	Response handleRequest(Request &cur_request, ServerConf *server_conf, Session *cur_session);
};

#endif 