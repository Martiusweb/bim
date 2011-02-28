CXX = g++
CPPFLAGS = -Wall -ansi -pedantic -g -DREENTRANT

BIN=bin/
OBJ=obj/
SRC=src/

all : $(BIN)server

$(OBJ)%.o : $(SRC)%.cpp
	$(CXX) $(CPPFLAGS) -c $< -o $@

$(BIN)server : $(OBJ)main.o $(OBJ)Listenable.o $(OBJ)Server.o $(OBJ)EventDispatcher.o
	$(CXX) $(CPPFLAGS) $+ -o $@ -lpthread -D_GNU_SOURCE

main.o : Server.o Listenable.o 
Server.o : Listenable.o
EventDispatcher.o :
Listenable.o :

clean :
	rm $(OBJ)* $(BIN)*

run : all
	$(BIN)server
