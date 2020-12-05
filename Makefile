CFLAGS = -std=c++11 -O3 -Wall
CC = g++
LD = ld
LDFLAGS = -lpthread -lwiringPi

OBJ = ntc_sc.o 
BIN = ntc_sc

gpio: $(OBJ)
	$(CC) $(CFLAGS) -o $(BIN) $(OBJ) $(LDFLAGS)

%.o: %.cc
	$(CC) $(CFLAGS) -c $<

clean:
	rm -rf $(BIN) $(OBJ)
