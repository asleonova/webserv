/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: monie <monie@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/20 15:15:16 by monie             #+#    #+#             */
/*   Updated: 2021/06/12 18:33:17 by monie            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "request.hpp"

std::string Request::ft_tolover(std::string str) {
	std::string new_str;
	int len = str.length();
	for(int i = 0; i < len; i++) {
		if(str[i] >= 65 && str[i] <= 90) {
			new_str += str[i] + 32;
		} else {
			new_str += str[i];
		}
	}
	return new_str;
}

int Request::str_to_un_int(std::string str) {
	int ui = 0;
	int len = str.length();
	for(int i = 0; i < len; i++) {
		if(i != 0) {
			ui *= 10;
		}
		ui += str[i] - '0';
	}
	return ui;
}

int Request::hex_to_dec(std::string st)
{
    int i, k;
    unsigned int s = 0;
    for (i = 0; st[i] != '\0'; i++)
    {
        int c;
        switch (c = toupper(st[i]))
        {
        case 'A':
        case 'B':
        case 'C':
        case 'D':
        case 'E':
        case 'F':
            k = c - 'A' + 10;
            break;
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
        case '0':
            k = c - '0';
            break;
        }
        s = (s << 4) + k;
    }
    return s;
}

void Request::request_init() {
	buf.clear();
	_type_request.clear();
	_resource_name.clear();
	_protocol_version.clear();
	_body.clear();
	_f_sl_status = 0;
	_f_hd_status = 0;
	_f_bd_status = 0;
	_f_end_request = 0;
	method_body = 0;
	index = 0;
	i_end = 0;
	len = 0;
	f_error = 0;
	headers.clear();
}

int Request::find_end(std::string end, int i) {
	int j = buf.find(end);
	if( j != -1) {
		i_end = i + j;
		return 1;
	}
	return 0;
}

void Request::new_buf() {
	std::string tmp_buf;
	for(;i_end < len; i_end++) {
		tmp_buf += buf[i_end];
	}
	buf.clear();
	buf = tmp_buf;
	index = 0;
	i_end = 0;
}

void Request::filling_start_line() {
	std::string word;
	int flag = 0;
	for(int i = 0; i < len; i++) {
		if(buf[i] == ' ' && flag == 0) {
			_type_request = word;
			word.clear();
			flag = 1;
			continue;
		} else if (buf[i] == ' ' && flag == 1) {
			_resource_name = word;
			word.clear();
			flag = 2;
			continue;
		} else if (buf[i] == '\r' && buf[i + 1] == '\n' && flag == 2) {
			_protocol_version = word;
			word.clear();
			flag = 3;
			break;
		} 
		word += buf[i];
	}
	_f_sl_status = 1;
}

void Request::filling_headers() {
	std::string word;
	std::string key;
	int flag = 0;
	for(int i = 0; i < len; i++) {
		if(buf[i] == ':' && buf[i + 1] == ' ' && flag == 0) {
			i += 2;
			key = ft_tolover(word);
			word.clear();
			flag = 1;
		} else if (buf[i] == '\r' && buf[i + 1] == '\n' && flag == 1) {
			i += 2;
			headers.insert(make_pair(key, ft_tolover(word)));
			word.clear();
			flag = 0;
		}
		word += buf[i];
	}
	if(headers.count("content-length")) {
		method_body = str_to_un_int(headers["content-length"]);
	}
	if(headers.count("transfer-encoding")) {
		method_body = -1;
	}
	_f_hd_status = 1;
}

void Request::filling_body_length(int len) {
	int i = 0;
	std::string tmp;
	for(; i < len; i++) {
		_body += buf[i];
	}
	buf.clear();
	buf = tmp;
	_f_bd_status = 1;
}

void Request::filling_body_encoding() {
	std::string tmp;
	static int cache = -1;
	int i = 0;
	for(; i < len; ++i) {
		if(cache == -1 && buf[i] == '\r' && buf[i + 1] == '\n') {
			++i;
			cache = hex_to_dec(tmp) + tmp.length() + 2;
			if(cache == 3) {
				cache = 0;
			}
			tmp.clear();
		} else if(cache == 0) {
			ready = 1;
			return;
		} else if(cache == i) {
			_body += tmp;
			tmp.clear();
			i += 2;
			i_end = i;
			new_buf();
			len = buf.length();
			cache = -1;
		} else {
			tmp += buf[i];
		}
	}
}

int Request::func_request(std::string str) {
	buf += str;
	len = buf.length();
	if(_f_sl_status == 0 && find_end("\r\n", 2)) {
		filling_start_line();
		new_buf();
	} else if(_f_sl_status == 1 && _f_hd_status == 0 &&\
		find_end("\r\n\r\n", 4)) {
		filling_headers();
		new_buf();
	} else if(_f_sl_status == 1 && _f_hd_status == 1 &&\
		_f_bd_status == 0 && method_body > 0) {
		filling_body_length(method_body);
		new_buf();
	} else if(_f_sl_status == 1 && _f_hd_status == 1 &&\
		_f_bd_status == 0 && method_body == -1) {	
		filling_body_encoding();
	}
	if(_f_bd_status == 1) {
		std::cout << "func" << std::endl;
		
		return 1;
	}
	return 0;
}

void Request::see_request() {
	std::cout << "------------------------------------------------------------------------" << std::endl;
	std::cout << "-------------------------------start line-------------------------------" << std::endl;
	std::cout << "Type:\t\t" << _type_request << std::endl;
	std::cout << "Resource:\t" << _resource_name << std::endl;
	std::cout << "Version:\t" << _protocol_version << std::endl;
	std::cout << "-------------------------------headers----------------------------------" << std::endl;
	std::map<std::string, std::string>::iterator it;
	for (it = headers.begin(); it != headers.end(); it++) {
		if((it->first).length() < 9) 
			std::cout << "key\t" << it->first << "\t\t\tvalue\t\t" << it->second << std::endl;
		else if((it->first).length() < 16)
			std::cout << "key\t" << it->first << "\t\tvalue\t\t" << it->second << std::endl;
		else
			std::cout << "key\t" << it->first << "\tvalue\t\t" << it->second << std::endl;
	}
	std::cout << "-------------------------------body-------------------------------------" << std::endl;
	std::cout << _body << std::endl;
	std::cout << "-------------------------------left-------------------------------------" << std::endl;
	std::cout << buf << std::endl;
	std::cout << "-------------------------------end--------------------------------------" << std::endl;
	std::cout << "------------------------------------------------------------------------" << std::endl;
}

int main() {
	Request request;
	std::string piece[11];
	piece[0] = "GET /images/corner1.png HTTP/1.1\r\nHost: mail.example.com\r\n";
	piece[1] = "Referer: http://mail.example.com/send-message.html\r\n";
	piece[2] = "User-Agent: BrowserForDummies/4.67b\r\n";
	piece[3] = "Content-Type: multipart/form-data; boundary=\"Asrf456BGe4h\"\r\nConnection: keep-alive\r\n";
	piece[4] = "Transfer-Encoding: chunked\r\n";
	piece[5] = "Keep-Alive: 300\r\n";
	piece[6] = "\r\n64\r\nSaaaa aadafasdfadfadfdfasf asdfasfsfd asdfsdsdfaas";
	piece[7] = "aaaa aadafasdfadfadfdfasf asdfasfsfd asdfsdsdfaasE\r\n64\r\nSaaa aadafasdfadfadfdfasf asdfasfsfd asdfsdsdfaasd";
	piece[8] = "aaaa aadafasdfadfadfdfasf asdfasfsfd asdfsdsdfaa1R\r\n0\r\nafafaffa";
	piece[9] = "aaaa aadafasdfadfadfdfasf asdfasfsfd asdfsdsdfaasE\r\n64\r\nSaaa aadafasdfadfadfdfasf asdfasfsfd asdfsdsdfaasd";
	piece[10] = "aaaa aadafasdfadfadfdfasf asdfasfsfd asdfsdsdfaasR\r\n0\r\nafafaffa";
	request.request_init();
	for(int i = 0; !request.ready; i++) {
		std::cout << i << " - " << request.ready << std::endl;
		request.func_request(piece[i]);
	}
	request.see_request();
	// request.request_init();
	// request.see_request();
	return 0;
}