
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <iostream>
#include <arpa/inet.h> //inet_addr
#include <unistd.h>
#include <fcntl.h>
#include <map>
#include <string>
#include <unistd.h>

#include <fstream>
#include <stdio.h>


int main(int ac, char **av)
{
	if (ac != 3)
	{
		std::cout << "Please, use " << av[0] << " [ip-addres] [port]" << std::endl;
		return (1);
	}

    //создаем сокет
    int fd_socket = socket(AF_INET, SOCK_STREAM, 0);

    if (fd_socket < 0)
        std::cout << "ERROR: failed to create socket" << std::endl;
    else
        std::cout << "SUCESS: created send socket" << std::endl;

    //создаем адрес. 
    sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;					// ip4
    serv_addr.sin_addr.s_addr = inet_addr(av[1]);	// куда конектимся
    serv_addr.sin_port = htons(atoi(av[2]));		// порт куда будем цепляться
    
    //устанавливаем соединение
    int result = connect(fd_socket, (sockaddr *)&serv_addr, sizeof serv_addr);
    if (result < 0)
    {
        std::cout << "ERROR: failed to connect to server" << std::endl;
    }
    else
        std::cout << "SUCESS: connected" << std::endl;

	// вот после этого ошибка
	// уведем сокет в неблокирующий режим
	// fcntl(fd_socket, F_SETFL, O_NONBLOCK);

	// открываем файл и читам запрос оттуда
	std::ifstream	request_file("sandbox/clientHTTP/request.txt");
	std::string		request(( std::istreambuf_iterator<char>(request_file) ), std::istreambuf_iterator<char>());

	ssize_t write_res = write(fd_socket, request.c_str(), request.size());
	std::cout << "SUCESS: Write " << write_res << " of " << request.size() << " bytes" << std::endl;

	std::string return_request;
	ssize_t read_res = 0;
	char buf[8000];

	std::ofstream out("sandbox/clientHTTP/output.txt");

	FILE* pFile = fopen("sandbox/clientHTTP/outputBinary.txt","wb");
	 
	while ((read_res = read(fd_socket, buf, 8000)) > 0)
	{
		return_request += std::string(buf, read_res);
		out << std::string(buf, read_res);

		if (pFile)
			fwrite(buf, read_res, 1, pFile);

		std::cout << "SUCESS: Read " << read_res << " bytes" << std::endl;
	}

	if (read_res < 0)
		perror("Error in read");

	std::cout << "FINISH download" << std::endl;
	
	out.close();
	fclose(pFile);

    return 0;
}