#A4 makefile
 
CC=g++  
COMPILE=-c
LINK=-o
PROGRAM=clusterer
PROGRAM2=PPM
#TEST=Testing/Test
H=Header.h

#      c++11     all warnings on            
#      |         |     debug info (allows debugging with gdb)
#      |         |     |  check for out-of-bounds errors
#      V         V     V  V                  /=extra info for runtime errors
FLAGS=-std=c++11 -Wall -g -fsanitize=address -fno-omit-frame-pointer
OPTIMIZE=-O3 -std=c++11 -Wall #but do not add error checking at runtime

#for running test cases
#test:
#	$(CC) $(OPTIMIZE) $(COMPILE) $(H) $(PROGRAM).cpp $(TEST).cpp #compile for speed
#	$(CC) $(OPTIMIZE) $(LINK) $(PROGRAM) $(PROGRAM).o Test.o  #link

#and fast executable is an additional target
release:
	$(CC) $(OPTIMIZE) $(COMPILE) $(H) $(PROGRAM).cpp $(PROGRAM2).cpp  #compile for speed
	$(CC) $(OPTIMIZE) $(LINK) $(PROGRAM) $(PROGRAM).o $(PROGRAM2).o  #link

clean:
	rm -f *.o #clear out object files
	rm -f Header.h.gch
	rm -f $(PROGRAM) #and the final executable

