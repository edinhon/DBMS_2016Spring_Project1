CC = g++ -lm -std=c++11 -Wall

SRCS = ./*.cpp
OBS = ./*.o

QDBM_INCDIR := -I /usr/local/include

QDBM_LIBDIR := -L /usr/local/lib

QDBM_LIB := -lxqdbm -lqdbm

CXXFLAGS := $(QDBM_INCDIR) $(QDBM_LIBDIR) $(QDBM_LIB)

# The following line means that do SRC first.
DBMSProject1: SRC
	$(CC) -o $@  *.o $(CXXFLAGS)

SRC: $(SRCS)
	$(CC) -c $(CXXFLAGS) $(SRCS)
	
.PHONY :
	clean
clean: 
	rm DBMSProject1 DBMSProject1.exe *.o 
