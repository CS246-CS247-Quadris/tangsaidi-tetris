# Project: Genesis

CPP      = g++
OBJ      = main.o game.o board.o block.o row.o pixel.o level.o score.o levelZero.o levelOne.o levelTwo.o levelThree.o settler.o
LINKOBJ  = main.o game.o board.o block.o row.o pixel.o level.o score.o levelZero.o levelOne.o levelTwo.o levelThree.o settler.o
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

levelZero.o: levelZero.cc
	$(CPP) -c levelZero.cc -o levelZero.o $(CXXFLAGS)

levelOne.o: levelOne.cc
	$(CPP) -c levelOne.cc -o levelOne.o $(CXXFLAGS)

levelTwo.o: levelTwo.cc
	$(CPP) -c levelTwo.cc -o levelTwo.o $(CXXFLAGS)

levelThree.o: levelThree.cc
	$(CPP) -c levelThree.cc -o levelThree.o $(CXXFLAGS)

level.o: level.cc
	$(CPP) -c level.cc -o level.o $(CXXFLAGS)

score.o: score.cc
	$(CPP) -c score.cc -o score.o $(CXXFLAGS)

settler.o: settler.cc
	$(CPP) -c settler.cc -o settler.o $(CXXFLAGS)
