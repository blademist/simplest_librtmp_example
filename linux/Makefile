CC=gcc
CFLAGS=-I include -Wall -g
#CFLAGS=-I include -Wall -g -DDEBUG
LIBS=-lrtmp

RM=rm

TARGET=simplest_librtmp_receive
OBJ=simplest_librtmp_receive.o
SRC=simplest_librtmp_receive.c


all: $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ) $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	$(RM) -f $(TARGET) $(OBJ) *~
