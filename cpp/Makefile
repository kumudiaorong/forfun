crc:c
	valgrind --leak-check=yes ./main

cr:c
	./main

grc:g
	valgrind --leak-check=yes ./main

gr:g
	./main

Path=~/work/forfun/cpp
FLAGS= -Wall -Wextra -std=c++20 -fdiagnostics-color=always#-Werror
DYNA= -lz -lssl -lpthread -lcrypto -ldl

g:
	/usr/bin/g++ $(FLAGS) -I $(Path)/include -L $(Path)/stat -g $(Path)/main.cpp $(DYNA) -o $(Path)/main
c:
	clang $(FLAGS) -I $(Path)/include -L $(Path)/stat -g $(Path)/main.cpp -lstdc++ $(DYNA) -o $(Path)/main
