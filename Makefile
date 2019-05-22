CC=g++
CFLAGS=-Wall -O -g
LDFLAGS=\
-lyaml-cpp \
-lcrypto \
-lncurses \
-lmenu \
-lform
EXEC=passtis
SRC=$(wildcard src/*.cpp) $(wildcard src/windows/*.cpp)
HEADERS=$(wildcard src/*.h) $(wildcard src/windows/*.h)
OBJ=$(SRC:.cpp=.o)

all: $(EXEC)

passtis: $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.cpp $(HEADERS)
	$(CC) -o $@ -c $< $(CFLAGS)

clean:
	rm -f src/*.o src/windows/*.o core

mrproper: clean
	rm -f $(EXEC)
