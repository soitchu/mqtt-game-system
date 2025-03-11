all: rgb-matrix
	g++ main.cpp -std=c++20 -o main -I/usr/include/mqtt -I./rpi-rgb-led-matrix/include -L./rpi-rgb-led-matrix/lib -lpaho-mqttpp3 -lpaho-mqtt3as -lrgbmatrix

rgb-matrix:
	cd rpi-rgb-led-matrix && make


