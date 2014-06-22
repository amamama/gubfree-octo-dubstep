CC = gcc
TARGET = 2048
SRC = main.c
OBJ = $(SRC:.c=.o)
CFLAGS = -ansi -std=c99 -W -Wall -Werror
LIBS = ncurses
CFLAGS += $(shell pkg-config --cflags $(LIBS))
LDFLAGS += $(shell pkg-config --libs $(LIBS))

all : $(OBJ)
	$(CC) $(OBJ) -o $(TARGET) $(LDFLAGS)

.c.o :
	$(CC) -c $< $(CFLAGS)

run : $(TARGET)
	./$(TARGET)

clean :
	rm $(TARGET) $(OBJ)
