CC=g++
CFLAGS=-O3
DEPS = Bignumber.h
OBJ = main.o\
	string_operation.o
%.o:%.cpp $(DEPS)
	$(CC) -c -o $@ $<  $(CFLAGS)
main: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)
clean:
	rm -rf ./main.o ./string_operation.o ./main