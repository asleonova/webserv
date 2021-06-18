#ifndef REQUEST_HPP
# define REQUEST_HPP

#include "includes.hpp"
#include "config.hpp"

struct Request
{
	std::vector<std::string>			_allLinesRequest;		// все строки запроса
	std::vector<std::string>			_startLine;				
	std::string							_versionProtocol;		// версия протокола
	std::string							_requestTarget;			// запрошенная страница - убрать все начиная с ?params=....
	std::string							_methodStr;				// метод
	std::string							_remainder;				// остаток
	std::string							_body;					// тело-содержание
	bool								_bodyFlag;				// флаги для содержания
	std::map<std::string, std::string>	_headers;				// распарсенные заголовки
	size_t								_contentLength;			// длинна контента
	bool								_flagTE;				// флаги ТЕ
	size_t								_TELength;				// длинна ТЕ
	std::string							_tempBody;				// фременное тело
	bool								_isOK;					// успешность запроса
	Method								_methodEnum;			// методы
	std::string							_queryString;			// строка запроса
	bool								_isBadRequest;			// плохой запрос
	std::string							_userAndPass;			// пользователь и пароль
	std::string							_user;					// пользователь
	std::string							_pass;					// пароль

				Request();
				~Request();
	Request		*syntax(std::string req);

	std::string	setBody(std::string req);

	void		setStartLine();
	void		setHeaders();

	std::string	ft_decodeBase64(std::string auth);

	void		checkBody();

	//-- очистка запроса
	void						clear();
};

std::vector<std::string>	split(std::string req, std::string delimetr);

#endif