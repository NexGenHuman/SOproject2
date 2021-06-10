OBJ = main.o functions.o
all: sleeping_barber

sleeping_barber: $(OBJ)
	gcc $(OBJ) -pthread -o sleeping_barber

$(OBJ): functions.h

.PHONY: clean
clean:
	rm -f *.o sleeping_barber