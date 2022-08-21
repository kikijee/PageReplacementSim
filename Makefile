CC = g++			# use g++ for compiling c++ code or gcc for c code
CFLAGS = -O -Wall -std=c++11		# compilation flags: -g for debugging. Change to -O or -O2 for optimized code.
LIB = -lm			# linked libraries	
LDFLAGS = -L.			# link flags
PROG = PageReplacementSim			# target executable (output)
SRCS = main.cpp pagetable.cpp      # .c or .cpp source files.
OBJ = $(SRCS:.cpp=.o) 	# object files for the target. 

all : $(PROG)

$(PROG): $(OBJ) 
	$(CC) -o $(PROG) $(OBJ) $(LDFLAGS) $(LIB)

.cpp.o:
	$(CC) -c $(CFLAGS) $< -o $@

depend: .depend

.depend: $(SRCS)
	rm -f ./.depend
	$(CC) $(CFLAGS) -MM $^ > ./.depend;

include .depend

# cleanup
clean:
	/bin/rm -f *.o $(PROG)

# DO NOT DELETE
