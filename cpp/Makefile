crc:c
	valgrind --leak-check=yes ./main

cr:c
	./main

g:
	/usr/bin/g++ -std=c++20 -fdiagnostics-color=always -I /home/kmdr/work/lib/cpp/include -L /home/kmdr/work/lib/cpp/stat -g /home/kmdr/work/lib/cpp/main.cpp -lz -lssl -lpthread -lcrypto -ldl -o /home/kmdr/work/lib/cpp/main
c:
	clang -std=c++20 -fcolor-diagnostics -I /home/kmdr/work/lib/cpp/include -L /home/kmdr/work/lib/cpp/stat -g /home/kmdr/work/lib/cpp/main.cpp -lstdc++ -lz -lssl -lpthread -lcrypto -ldl -o /home/kmdr/work/lib/cpp/main
