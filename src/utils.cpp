#include "utils.hpp"

// фатальная ошибка при инициализации
void		fatal_error(const char *error_str)
{
	perror(error_str);
	exit(errno);
}

// перегрузка фатальной ошибки когда не можем поднять сервер -> убрать в utils
void		fatal_error(const char *error_str, const char *ip, const unsigned int port)
{
	std::cout << "For: " << ip << ":" << port << ": " << std::endl;
	perror(error_str);
	exit(errno);
}

// вывод текущей конфигурации ServConf -> убрать в utils
std::ostream	&operator<<( std::ostream &o, ServerConf const &i )
{
	o << "[ServerInstance '" << i.server_name << "' (" << i.ip << ":" << i.port << "). LS: " << i.ls << "]"; 
	return o;
}

static	long int	ws_intlen(long int n)
{
    int	i;
    int	sign;

    i = 0;
    if (n == 0)
        return (1);
    if ((sign = n) < 0)
    {
        n = -n;
        i++;
    }
    while (n > 0)
    {
        n /= 10;
        i++;
    }
    return (i);
}

char				*ws_itoa(int n)
{
    char			*str;
    int				len;
    long int		nn;

    nn = (long int)n;
    len = ws_intlen(nn);
    if (!(str = (char *)malloc(len + 1)))
        return (NULL);
    str[len--] = '\0';
    if (nn < 0)
        nn = -nn;
    while (len >= 0)
    {
        str[len--] = nn % 10 + '0';
        nn /= 10;
    }
    if (n < 0)
        str[0] = '-';
    return (str);
}