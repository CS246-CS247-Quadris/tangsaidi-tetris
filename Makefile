# Project: Genesis

CPP      = g++
OBJ      = main.o game.o board.o block.o row.o pixel.o
LINKOBJ  = main.o game.o board.o block.o row.o pixel.o
BIN      = Genesis
CXXFLAGS = -std=c++14
RM       = rm -f

.PHONY: all all-before all-after clean clean-custom

all: all-before $(BIN) all-after

clean: clean-custom
	${RM} $(OBJ) $(BIN)

$(BIN): $(OBJ)
	$(CPP) $(LINKOBJ) -o $(BIN)

main.o: main.cc
	$(CPP) -c main.cc -o main.o $(CXXFLAGS)

game.o: game.cc
	$(CPP) -c game.cc -o game.o $(CXXFLAGS)

board.o: board.cc
	$(CPP) -c board.cc -o board.o $(CXXFLAGS)

block.o: block.cc
	$(CPP) -c block.cc -o block.o $(CXXFLAGS)

row.o: row.cc
	$(CPP) -c row.cc -o row.o $(CXXFLAGS)

pixel.o: pixel.cc
	$(CPP) -c pixel.cc -o pixel.o $(CXXFLAGS)
