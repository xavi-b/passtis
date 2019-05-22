CC=g++
CFLAGS=-Wall -O -g \
-I third-party
LDFLAGS=\
-lyaml-cpp \
-lcrypto \
-lncurses \
-lmenu \
-lform \
-lxcb \
-lpthread \
-L third-party/clip -lclip
EXEC=passtis
SRC=\
$(wildcard src/*.cpp) \
$(wildcard src/windows/*.cpp)
HEADERS=\
$(wildcard src/*.h) \
$(wildcard src/windows/*.h)
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
