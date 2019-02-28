CXX = g++
CXXFLAGS = -Wall -Wextra -pedantic -std=c++11 -g

chess: chess.o board.o main.cpp
	$(CXX) $(CXXFLAGS) chess.o board.o main.cpp -o chess 

board.o: board.cpp game.h prompts.h
	$(CXX) $(CXXFLAGS) -c board.cpp

chess.o:  chess.cpp game.h chess.h prompts.h
	$(CXX) $(CXXFLAGS) -c chess.cpp 

unittest: unittest.o chess.o
	$(CXX) $(CXXFLAGS) -o unittest unittest.o chess.o

unittest.o: unittest.cpp chess.h
	$(CXX) $(CXXFLAGS) -c unittest.cpp

clean:
	rm *.o chess
