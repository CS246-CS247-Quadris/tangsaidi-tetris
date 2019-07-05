# Project: Genesis

CPP      = g++
OBJ      = main.o
LINKOBJ  = main.o
BIN      = Genesis.exe
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
