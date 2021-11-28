NAME = yemu
CFLAGS := -Wall -Werror -g

CC = gcc
#LD = gcc

#stdint.o: 
#	$(CC) -c $(patsubst %.o, %.h, $@) -o $@

yemu: 
	$(CC) $(CFLAGS) $@.c -o $@
	
clean:
	-rm -rf *.o $(NAME)
