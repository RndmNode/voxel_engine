CC = /usr/bin/clang++
CFLAGS = -std=c++17 -fcolor-diagnostics -Wall -fansi-escape-codes -g -I./include -L./lib -Wno-deprecated
LIBS = -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo -framework CoreFoundation
SRC = $(wildcard ./src/*.cpp) $(wildcard ./src/*.c) $(wildcard ./src/vendor/imgui/*.cpp) ./lib/libglfw.3.3.dylib ./lib/libGLEW.2.2.0.dylib
OUT = ./bin/app

all:
	$(CC) $(CFLAGS) $(SRC) -o $(OUT) $(LIBS)

clean:
	rm -rf $(OUT).dSYM