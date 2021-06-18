/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: monie <monie@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/20 16:14:24 by monie             #+#    #+#             */
/*   Updated: 2021/06/12 15:28:27 by monie            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "iostream"
#include "map"

struct Request {
	std::string buf;
	std::string	_type_request;
	std::string	_resource_name;
	std::string	_protocol_version;
	std::string _body;
	int			_f_sl_status;
	int			_f_hd_status;
	int			_f_bd_status;
	int			_f_end_request;
	std::map<std::string, std::string> headers;
	std::string ft_tolover(std::string str);
	int index;
	int i_end;
	int len;
	int f_error;
	int method_body;
	int ready;
	int find_end(std::string end, int i);
	void new_buf();
	void filling_start_line();
	void filling_headers();
	void filling_body_length(int len);
	void filling_body_encoding();
	void request_init();
	void func_find_end();
	int func_request(std::string str);
	void see_request();
	int  hex_to_dec(std::string st);
	int str_to_un_int(std::string str);
};