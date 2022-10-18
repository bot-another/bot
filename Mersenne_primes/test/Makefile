CC=g++
CFLAGS=-O3
DEPS = Bignumber.h
OBJ = test.o\
	string_operation.o
%.o:%.cpp $(DEPS)
	$(CC) -c -o $@ $<  $(CFLAGS)
test: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)
clean:
	rm -rf ./test.o ./string_operation.o ./test