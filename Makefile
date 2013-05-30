CC = gcc
CFLAGS = -Wall -Wextra -Wdeclaration-after-statement -Wbad-function-cast \
	-Wcast-qual -Wstrict-prototypes -Wmissing-declarations \
	-Wmissing-prototypes -Werror -g
LIBS = -pthread -lm
TARGET = test_thread_pool
SOURCES = task.c  thread_pool.c task_queue.c test_thread_pool.c
OBJECTS = $(SOURCES:%.c=%.o) 

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS) 

-include .depend

.depend: $(SOURCES) *.h
	$(CC) -MM $(SOURCES) > .depend

.PHONY: clean

clean:
	rm -f $(TARGET) $(OBJECTS) .depend
