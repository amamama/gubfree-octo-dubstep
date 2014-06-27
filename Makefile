CC = gcc
TARGET = 2048
SRC = main.c
OBJ = $(SRC:.c=.o)
OPT = -O2
CFLAGS += -ansi -std=c99 -W -Wall -Werror $(OPT)
LDFLAGS += $(OPT)
LIBS = ncurses
CFLAGS += $(shell pkg-config --cflags $(LIBS))
LDFLAGS += $(shell pkg-config --libs $(LIBS))

all : $(TARGET)

$(TARGET) : $(OBJ)
	$(CC) $(OBJ) -o $(TARGET) $(LDFLAGS)

.c.o :
	$(CC) -c $< $(CFLAGS)

run : $(TARGET)
	./$(TARGET)

clean :
	rm $(TARGET) $(OBJ)
