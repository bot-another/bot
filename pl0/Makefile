CC=g++
CFLAGS=-O3
DEPS = Actuator.h Compiler.h
OBJ = main.o
%.o:%.cpp $(DEPS)
	$(CC) -c -o $@ $<  $(CFLAGS)
pl0: $(OBJ)
	$(CC) -o $@ $< $(CFLAGS)
clean:
	rm -rf ./main.o