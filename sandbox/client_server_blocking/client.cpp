
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

int main()
{
    //создаем сокет
    int fd_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (fd_socket < 0)
        std::cout << "ERROR: failed to create socket" << std::endl;
    else
        std::cout << "SUCESS: created send socket" << std::endl;
    //создаем адрес. 
    struct sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(8080);
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    
    //устанавливаем соединение
    int result = connect(fd_socket, (sockaddr*)&serv_addr, sizeof(serv_addr));
    if (result < 0)
    {
        std::cout << "ERROR: failed to connect to server" << std::endl;
    }
    else
        std::cout << "SUCESS: connected" << std::endl;
    int i = 0;
    write(fd_socket, "Hello", 5);
    // while (i < 10)
    // {
    //     i++;
    //     std::cout << i << std::endl;
    // }
    // sleep(10);
    // i = 0;
    // while (i < 10)
    // {
    //     write(fd_socket, "Hello", 5);
    //     i++;
    //     std::cout << i << std::endl;
    // }
    sleep(1000);
    std::cout << "Start writing" << std::endl;
    return 0;
}