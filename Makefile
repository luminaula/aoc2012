CC = gcc
CFLAGS = -g -march=native
objects_c = 1.o 2.o 3.o 5.o 6.o 7.o 8.o 9.o 10.o 11.o 12.o 13.o
common_object = utils.o

%.o : %.c
	$(CC) $(CFLAGS) $< -c $@


.PHONY: all
all : $(objects_c) $(common_object)

clean:
	rm -r $(objects_c)