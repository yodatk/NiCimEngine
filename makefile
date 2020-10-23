CXX = g++
CXXFLAGS = -Ofast -g -Wall -c

OBJECTS = bin\main.o bin\Uci.o bin\NegaMaxSearch.o bin\Attacks.o bin\Squares.o bin\Board.o bin\Evaluation.o bin\TimeController.o bin\RandomNumbers.o bin\Bitboard.o bin\ZorbiestKeys.o bin\MagicNumbers.o bin\PerftTest.o bin\Search.o bin\TranspositionTable.o bin\Pieces.o

all: bin\NiCim.exe

bin\NiCim.exe : $(OBJECTS)
	$(CXX) -Ofast -g -Wall -o $@ $^

bin\main.o: src\Main.cpp
	$(CXX) $(CXXFLAGS) -o bin\main.o src\main.cpp

bin\Uci.o: src\Uci.cpp
	$(CXX) $(CXXFLAGS) -o bin\Uci.o src\Uci.cpp

bin\Squares.o: src\Squares.cpp
	$(CXX) $(CXXFLAGS) -o bin\Squares.o src\Squares.cpp

bin\Pieces.o: src\Pieces.cpp
	$(CXX) $(CXXFLAGS) -o bin\Pieces.o src\Pieces.cpp

bin\Board.o: src\Board.cpp
	$(CXX) $(CXXFLAGS) -o bin\Board.o src\Board.cpp

bin\Attacks.o: src\Attacks.cpp
	$(CXX) $(CXXFLAGS) -o bin\Attacks.o src\Attacks.cpp

bin\Evaluation.o: src\Evaluation.cpp
	$(CXX) $(CXXFLAGS) -o bin\Evaluation.o src\Evaluation.cpp

bin\TimeController.o: src\TimeController.cpp
	$(CXX) $(CXXFLAGS) -o bin\TimeController.o src\TimeController.cpp

bin\RandomNumbers.o: src\RandomNumbers.cpp
	$(CXX) $(CXXFLAGS) -o bin\RandomNumbers.o src\RandomNumbers.cpp

bin\Bitboard.o: src\Bitboard.cpp
	$(CXX) $(CXXFLAGS) -o bin\Bitboard.o src\Bitboard.cpp

bin\ZorbiestKeys.o: src\ZorbiestKeys.cpp
	$(CXX) $(CXXFLAGS) -o bin\ZorbiestKeys.o src\ZorbiestKeys.cpp

bin\MagicNumbers.o: src\MagicNumbers.cpp
	$(CXX) $(CXXFLAGS) -o bin\MagicNumbers.o src\MagicNumbers.cpp

bin\PerftTest.o: src\PerftTest.cpp
	$(CXX) $(CXXFLAGS) -o bin\PerftTest.o src\PerftTest.cpp

bin\Search.o: src\Search.cpp
	$(CXX) $(CXXFLAGS) -o bin\Search.o src\Search.cpp

bin\TranspositionTable.o: src\TranspositionTable.cpp
	$(CXX) $(CXXFLAGS) -o bin\TranspositionTable.o src\TranspositionTable.cpp

bin\NegaMaxSearch.o: src\NegaMaxSearch.cpp
	$(CXX) $(CXXFLAGS) -o bin\NegaMaxSearch.o src\NegaMaxSearch.cpp
