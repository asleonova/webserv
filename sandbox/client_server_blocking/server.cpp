#include <sys/socket.h> // socket func
#include <netinet/in.h> //sockaddr_in
#include <arpa/inet.h> //inet_addr
#include <iostream>
#include <unistd.h> //read

// struct sockaddr_in {
//     sa_family_t    sin_family; /* address family: AF_INET */
//     in_port_t      sin_port;   /* port in network byte order */
//     struct in_addr sin_addr;   /* internet address */
// };

/* Internet address */
// struct in_addr {
//     uint32_t       s_addr;     /* address in network byte order */
// };

int main(void)
{
    int socket_fd;

    //создаем сокет
    socket_fd = socket(AF_INET, SOCK_STREAM, 0);  
    if (socket_fd < 0)
        std::cout << "Failed to create cocket" << std::endl;
    else
        std::cout << "Success: created socket. Fd: " << socket_fd << std::endl;
    
    //создаем структуру с адресом
    struct sockaddr_in lstn_socket_addr;
    lstn_socket_addr.sin_family = AF_INET;
    lstn_socket_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    lstn_socket_addr.sin_port = htons(8080);
    
    //привязываем адрес
    //int bind(int sockfd, const struct sockaddr *addr,socklen_t addrlen);
    int bind_result = bind(socket_fd, (sockaddr*)&lstn_socket_addr, sizeof(lstn_socket_addr));
    if (bind_result < 0)
        std::cout << "Failed to bind address" << std::endl;
    else
        std::cout << "Success: bound address" << std::endl;
    //переводим в слушающий режим
    int listen_result = listen(socket_fd, SOMAXCONN);
    if (listen_result == 0)
        std::cout << "Sucseed to start listening" << std::endl;
    else
        std::cout << "Failed to start listening" << std::endl;
    
    //int accept(int sockfd, struct sockaddr *restrict addr, socklen_t *restrict addrlen);
    std::cout << "---Before accept" << std::endl;
    int client_fd = accept(socket_fd, NULL, NULL);
    std::cout << "---After accept" << std::endl;
    
    int buf_size = 2;
    char buf[buf_size + 1];
    
    while (true)
    {
        // std::cout << "---Before read: " << std::endl;
        int read_result = read(client_fd, buf, buf_size);
        std::cout << "Read result: " << read_result << std::endl;
    }
    return 0;
}