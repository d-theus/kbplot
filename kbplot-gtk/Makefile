CC=g++
LDFLAGS=`pkg-config gtk+-2.0 gtkglext-1.0 --libs`
CFLAGS=-Wall `pkg-config gtk+-2.0 gtkglext-1.0 --cflags`
SRC=main.cpp
OBJ=$(SRC:.cpp=.o)
	BIN=program
all: $(SRC) $(BIN)
$(BIN): $(OBJ)
	$(CC) $(LDFLAGS)  $(OBJ) -o $@
.cpp.o:
	$(CC) -c $(CFLAGS) $< -o $@
launch:
	./$(BIN)
