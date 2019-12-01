CFLAGS = -std=c++11 -O3 -Wall
CC = g++
LD = ld
LDFLAGS = -lpthread -lwiringPi

OBJ = ntc.o 
BIN = ntc

gpio: $(OBJ)
	$(CC) $(CFLAGS) -o $(BIN) $(OBJ) $(LDFLAGS)

%.o: %.cc
	$(CC) $(CFLAGS) -c $<

clean:
	rm -rf $(BIN) $(OBJ)
