#include "request.hpp"

			Request::Request()
{
	// есть тело
	this->_bodyFlag = false;
	// длинна контента
	this->_contentLength = 0;
	// флаг?
	this->_flagTE = false;
	// длинна ТЕ
	this->_TELength = 0;
	// запрос сформирован?
	this->_isOK = false;
	// запрос плохой?
	this->_isBadRequest = false;
}

			Request::~Request()
{
}

Request		*Request::syntax(std::string req)
{
	// сконкатинируем необработанную часть запроса и пришедшую строку
	req = this->_remainder + req;

	// debug - печать запроса
	// std::cout << "\x1b[32m" << req <<  "\x1b[0m" << '\n';

	// если:
	// - первые символы буфера - перенос строки
	// - длинна контента 0
	// - флаги ТЕ - нет и размер 3 байта
	if (req[0] == '\r' && req[1] == '\n' && this->_contentLength == 0 && this->_flagTE == false  && _startLine.size() == 3)	
		this->_isOK = true;

	// если запрос закончен - то возвращем ссылку на себя
	if (this->_isOK)
		return (this);

	// если есть флаги для содержания - добавляем тело из запроса
	if (this->_bodyFlag)
		this->_tempBody = req;
	else
	{
		// установим тело запроса
		req = this->setBody(req);

		// разделим строку на вектора по разделителю переноса строки
		this->_allLinesRequest = split(req, "\r\n");

		// =================== определить остаток (_remainder):
		// в остаток помещаем все что справа от переноса строки
		size_t	found;

		found = req.rfind("\r\n");
		if (found != std::string::npos)
		{
			found += 2;
			this->_remainder = req.substr(found);
		}
		else
			this->_remainder = req;
		// =================== end of set remainder

		if ((this->_allLinesRequest).size() >= 1)
		{
			if (this->_methodStr == "" && this->_requestTarget == "" && this->_versionProtocol == "")
			{
				// начинамем парсинг всего запроса
				this->setStartLine();
				if (this->_isOK)
					return (this);
			}
			if ((this->_allLinesRequest).size() > 0)
			{
				// выставляем заголовки
				this->setHeaders();
				if (this->_isOK)
					return (this);
			}
		}
	}
	if (this->_bodyFlag)
		this->checkBody();
	if (this->_isOK == true && this->_methodEnum == POST && this->_body.compare(0, 6, "login=") == 0)
	{
		this->_queryString = this->_body;
		this->_body = "";
	}
	return (this);
}

std::string	Request::setBody(std::string req)
{
	size_t		found;
	std::string	new_req;

	if (req[0] == '\r' && req[1] == '\n' && this->_startLine.size() == 3)
	{
		this->_tempBody = req.substr(2);
		this->_bodyFlag = true;
		return ("");
	}
	found = req.find("\r\n\r\n");
	if (found != std::string::npos)
	{
		found += 2;
		new_req = req.substr(0, found);
		found += 2;
		this->_tempBody = req.substr(found);
		this->_bodyFlag = true;
		return (new_req);
	}
	else
		return (req);
}

void						Request::setStartLine()
{
	std::string					word;
	std::size_t					begin = 0;
	std::size_t					end = 0;
	std::size_t					found = 0;

	while (found != std::string::npos)
	{
		found = this->_allLinesRequest[0].find(" ", begin);
		end = found;
		word = this->_allLinesRequest[0].substr(begin, end - begin);
		if (word == "")
		{
			std::cout << "Error: Start Line" << std::endl;
			this->_isOK = true;
			this->_isBadRequest = true;
			return ;
		}
		this->_startLine.push_back(word);
		begin = found + 1;
	}
	this->_allLinesRequest.erase(this->_allLinesRequest.begin());
	this->_methodStr = this->_startLine[0];
	if (this->_methodStr == "GET" || this->_methodStr == "\n\nGET" || this->_methodStr == "\nGET")
		this->_methodEnum = GET;
	else if (this->_methodStr == "PUT" || this->_methodStr == "\n\nPUT" || this->_methodStr == "\nPUT")
		this->_methodEnum = PUT;
	else if (this->_methodStr == "HEAD" || this->_methodStr == "\n\nHEAD" || this->_methodStr == "\nHEAD")
		this->_methodEnum = HEAD;
	else if (this->_methodStr == "POST" || this->_methodStr == "\n\nPOST" || this->_methodStr == "\nPOST")
		this->_methodEnum = POST;
	else if (this->_methodStr == "DELETE" || this->_methodStr == "\n\nDELETE" || this->_methodStr == "\nDELETE")
		this->_methodEnum = DELETE;
	else
		this->_methodEnum = UNSUPPORTED;

	this->_requestTarget = this->_startLine[1];
	found = this->_requestTarget.find("?");
	if (found != std::string::npos)
		this->_queryString = this->_requestTarget.substr(found + 1);
	this->_versionProtocol = this->_startLine[2];
}

void	Request::setHeaders()
{
	std::string	headLine;
	std::string	key;
	std::string	value;
	size_t		index;

	for (size_t i = 0; i < this->_allLinesRequest.size(); i++)
	{
		headLine = this->_allLinesRequest[i];
		index = headLine.find(':');
		if (index != std::string::npos)
			key = headLine.substr(0, index);
		else
		{
			this->_isOK = true;
			this->_isBadRequest = true;
			return ;
		}
		if (key.length() == 0)
		{
			std::cout << "Error Headers (key length)" << std::endl;
			this->_isOK = true;
			this->_isBadRequest = true;
			return ;
		}
		if (key[0] == ' ' || key[key.length() - 1] == ' ')
		{
			std::cout << "Error Headers (syntax)" << std::endl;
			this->_isOK = true;
			this->_isBadRequest = true;
			return ;
		}
		index++;
		if (headLine[index] == ' ')
			index++;
		value = headLine.substr(index);
		if (value.length() == 0)
		{
			std::cout << "Error Headers (value length)" << std::endl;
			this->_isOK = true;
			this->_isBadRequest = true;
			return ;
		}
		std::transform(key.begin(), key.end(), key.begin(), tolower);
		std::transform(value.begin(), value.end(), value.begin(), tolower);
		if (key == "content-length")
			this->_contentLength = std::stoi(value);
		else if (key == "transfer-encoding" && value == "chunked")
			this->_flagTE = true;
		if (key == "authorization")
		{
			this->_userAndPass = this->ft_decodeBase64(value);
			index = this->_userAndPass.find(":");
			if (index == std::string::npos)
			{
				this->_isOK = true;
				this->_isBadRequest = true;
				return;
			}
			this->_user = this->_userAndPass.substr(0, index);
			this->_pass = this->_userAndPass.substr(index + 1);
		}
		this->_headers.insert(make_pair(key, value));
	}
}

std::string			Request::ft_decodeBase64(std::string auth)
{
   	std::string alp = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    std::string bin_temp;
    size_t      i;
    size_t		j;
    std::string temp;
    std::string res;

    for (i = 0; i < auth.length(); i++)
    {
        for (j = 0; j < alp.length(); j++)
        {
            if (auth[i] == alp[j])
                break ;
        }
        if (j == alp.length())
            break ;
        std::bitset<8>  auth(j);
        temp = auth.to_string();
        temp = temp.substr(2);
        bin_temp += temp;
    }
    for (; bin_temp.length() >= 8; bin_temp = bin_temp.substr(8))
    {
        temp = bin_temp.substr(0, 8);
        i = std::stoi(temp, NULL, 2);
        res += static_cast<char>(i);
	}
	return (res);
}

void	Request::checkBody()
{
	size_t		found;
	std::string	temp;

	found = 0;
	if (this->_flagTE)
	{
		while (true)
		{
			found = this->_tempBody.find("\r\n");
			if (found == 0)
			{
				this->_tempBody = this->_tempBody.substr(2);
				continue ;
			}
			if (found == std::string::npos)
			{
				this->_remainder = this->_tempBody;
				return ;
			}
			if (this->_TELength == 0)
			{
				temp = this->_tempBody.substr(0, found);
				found += 2;
				this->_TELength = std::stoi(temp, NULL, 16);
				if (this->_TELength == 0)
				{
					this->_isOK = true;
					return ;
				}
				this->_tempBody = this->_tempBody.substr(found);
				temp = this->_tempBody;
			}
			else
				temp = this->_tempBody.substr(0, found);
			if (temp.length() > this->_TELength)
				temp = this->_tempBody.substr(0, this->_TELength);
			this->_tempBody = this->_tempBody.substr(temp.length());
			this->_TELength -= temp.length();
			this->_body += temp;
		}
	}
	else if (this->_contentLength != 0)
	{
		this->_body += this->_tempBody;
		if (this->_body.length() >= this->_contentLength)
		{
			this->_body = this->_body.substr(0, this->_contentLength);
			this->_isOK = true;
		}
	}
	else
		this->_isOK = true;
}

// ================================== UTILS

// создание вектора строк на основе разделителя
std::vector<std::string>	split(std::string req, std::string delimetr)
{
	std::vector<std::string>	v;
	std::string					word;
	std::size_t					begin = 0;
	std::size_t					end = 0;
	std::size_t					found = 0;

	while ( (found = req.find(delimetr, begin)) != std::string::npos)
	{
		end = found;
		word = req.substr(begin, end - begin);
		if (word != "")
			v.push_back(word);
		begin = found + delimetr.length();
	}
	return (v);
}

// ================================== END OF UTILS

// очистка запроса:
void						Request::clear()
{
	this->_allLinesRequest.clear();
	this->_startLine.clear();
	this->_versionProtocol = "";
	this->_requestTarget = "";
	this->_methodStr = "";
	this->_remainder.clear();
	this->_body = "";
	this->_bodyFlag = false;
	this->_headers.clear();
	this->_contentLength = 0;
	this->_flagTE = false;
	this->_TELength = 0;
	this->_tempBody = "";
	this->_isOK = false;
	this->_methodEnum = UNSUPPORTED;
	this->_queryString = "";
	this->_isBadRequest = false;
}
