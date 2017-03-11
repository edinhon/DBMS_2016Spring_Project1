CC = g++ -lm -std=c++11 -Wall

SRCS = ./*.cpp
OBS = ./*.o

# The following line means that do SRC first.
DBMSProject1: SRC
	$(CC) -o $@ *.o

SRC: $(SRCS)
	$(CC) -c $(SRCS)
	
.PHONY :
	clean
clean: 
	rm DBMSProject1 DBMSProject1.exe *.o 