all:
	g++ -Isrc/include -Lsrc/lib -o target/main src/main.cpp -lmingw32 -lSDL2main -lSDL2