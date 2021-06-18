#ifndef RESPONSE_HPP
# define RESPONSE_HPP


# define SP " "
# define CRLF "\r\n"

#include "includes.hpp"
#include "utils.hpp"

// чтобы вызвать геттайм из handlerequest.htm
char *getTime(char *mbstr, const char *format, const char *pagePath, bool lm);

enum ResponseCode {
	Ok = 200,
	Created = 201,
	BadRequest = 400,
	Forbidden = 403,
	NotFound = 404,
	MethodNotAllowed = 405,
	PayloadTooLarge = 413,
	InternalServerError = 500,
	NotImplemented = 501,
	HTTPVersionNotSupported = 505
};

// функция возврата фразы ошибки по коду
std::string ReturnReasonPhrase(int status_code);

// класс для подготовки ответа на запрос
struct Response
{

	// код ответа: HTTP/1.1 
	const std::string	httpVersion;			// версия всегда HTTP/1.1 
	int					status_code_int_val;	// код ответа сервера
	std::string			reason_phrase;			// кодовая фраза ответа

	// флаг того что возврат идет из CGI скрипта
	bool				return_from_CGI;

	// мапа для хранения заголовков
	std::map<std::string, std::string> headers;

	// тело ответа
	std::string body;

	// конструктор
	Response() : httpVersion("HTTP/1.1"), status_code_int_val(1), return_from_CGI(false) {}

	Response &		operator=( Response const & rhs )
	{
		status_code_int_val = rhs.status_code_int_val;
		reason_phrase = rhs.reason_phrase;
		headers = rhs.headers;
		body = rhs.body;
		return *this;
	}

	// сформировать код ответа
	std::string getStrForAnswer()
	{

		// заполним фразу ответа
		reason_phrase = ReturnReasonPhrase(status_code_int_val);

		// итоговая строка ответа
		std::string		res_str;

		// конвертнем код в цифры + незабыть очистить
		// char *status_code_int_val_c_str = ws_itoa(status_code_int_val);	

		// формируем старлайн:
		// res_str = httpVersion + " " + std::string(status_code_int_val_c_str) + " " + reason_phrase + "\r\n";
		// std::cout << "\tout RESPONSE: " << httpVersion + " " + std::string(status_code_int_val_c_str) + " " + reason_phrase + "\r\n";
		res_str = httpVersion + " " + std::to_string(status_code_int_val) + " " + reason_phrase + "\r\n";
		// очистка буфера после itoa
		// free(status_code_int_val_c_str);									

		// std::cout << "\tout RESPONSE: " << res_str << std::endl;

		// добавляем длинну контента если есть контент (всегда отдаем контент-лен)
		if (0 == headers.count("Content-Length"))
			headers["Content-Length"] = std::to_string(body.size());

		// если есть контент и тип не указан добавляем тип по умолчанию - text/html
		if (0 != body.size() && 0 == headers.count("Content-Type"))
			headers["Content-Type"] = "text/html";

		// добавить дату если дата не указана
		char mbstr[100];
		if (0 == headers.count("Date"))
			headers["Date"] = getTime(mbstr, "%a, %d %b %Y %H:%M:%S GMT", "", false);

		// перечисляем заголовки
		for (std::map<std::string, std::string>::iterator it = headers.begin(); it != headers.end(); it++)
			res_str += it->first + ": " + it->second + "\r\n";

		res_str += "\r\n";
		res_str += body;

		// возвращаем тело ответа
		return res_str;
	}
};

# include "main.hpp"

Response ReturnErrorPage(ResponseCode response_code, std::vector<Method> *allowedMethod, Method req_method, ServerConf *server_conf);

#endif