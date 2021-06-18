
#include <iostream>
#include <string>

int main (void)
{
	std::string h;
	h.push_back('a');
	h.push_back('b');
	h.push_back('\0');
	h.push_back('b');
	h.push_back('b');
	std::cout << h.size() << std::endl;
	std::cout << h.c_str() << std::endl;
	std::cout << (h.c_str() + 3) << std::endl;

	return 0;
}