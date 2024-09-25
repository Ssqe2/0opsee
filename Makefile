CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -O2
LDFLAGS = -lrt
SOURCES = p7.c test_program.c 
OBJECTS = $(SOURCES:.c=.o)
EXECUTABLE = test_program

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(EXECUTABLE)
