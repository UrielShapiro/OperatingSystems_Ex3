test: reactor.o
	g++ -Wall -Wextra -Werror -ggdb -o test test.cpp reactor.o -pthread

reactor.o: reactor.cpp reactor.hpp
	g++ -Wall -Wextra -Werror -ggdb -c -o reactor.o reactor.cpp
