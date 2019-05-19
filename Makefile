CC=g++
CFLAGS=-Wall -O
LDFLAGS=\
-lyaml-cpp \
-lcrypto \
-lncurses \
-lform
EXEC=passtis
SRC=$(wildcard *.cpp) $(wildcard windows/*.cpp)
HEADERS=$(wildcard *.h) $(wildcard windows/*.h)
OBJ=$(SRC:.cpp=.o)

all: $(EXEC)

passtis: $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.cpp $(HEADERS)
	$(CC) -o $@ -c $< $(CFLAGS)

clean:
	rm -f *.o windows/*.o core

mrproper: clean
	rm -f $(EXEC)
