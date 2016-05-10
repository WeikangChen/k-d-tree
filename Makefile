CC = g++ -std=c++11
INC = 
DEBUG = -g
CFLAGS = -Wall -c $(DEBUG) $(INC)
LFLAGS = -Wall $(DEBUG) 

SRC1 = build_kdtree.cpp
OBJ1 = $(SRC1:.cpp=.o)
EXE1 = build_kdtree

SRC2 = query_kdtree.cpp
OBJ2 = $(SRC2:.cpp=.o)
EXE2 = query_kdtree

LIB = KDTree.a
LIB_MAKEFILE = Makefile_lib
MKF = make -f


all: $(LIB) $(EXE1) $(EXE2) 

$(LIB):
	$(MKF) $(LIB_MAKEFILE)

$(EXE1): $(OBJ1) $(LIB)
	$(CC) $(LFLAGS) $(OBJ1) $(LIB) -o $@

$(EXE2): $(OBJ2) $(LIB)
	$(CC) $(LFLAGS) $(OBJ2) $(LIB) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	$(RM)  *.o *~ $(EXE1) $(EXE2)
	$(MKF) $(LIB_MAKEFILE) clean
