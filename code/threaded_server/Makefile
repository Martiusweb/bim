CXX = clang++
CPPFLAGS = -Wall -ansi -pedantic -g -DREENTRANT

BIN=bin/
OBJ=obj/
SRC=src/

all : $(BIN)server

$(OBJ)%.o : $(SRC)%.cpp $(SRC)%.hpp
	$(CXX) $(CPPFLAGS) -c $< -o $@

$(OBJ)%.o : $(SRC)%.cpp
	$(CXX) $(CPPFLAGS) -c $< -o $@

$(BIN)server : $(OBJ)main.o $(OBJ)server.o $(OBJ)socket.o
	$(CXX) $(CPPFLAGS) $+ -o $@ -lpthread -D_GNU_SOURCE

main.o    : server.hpp
server.o : socket.hpp

clean :
	rm  $(OBJ)* $(BIN)*

run : all
	$(BIN)server
