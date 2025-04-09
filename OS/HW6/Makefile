CC = gcc
CFLAGS = -Wall -pthread
TARGET = main

$(TARGET): RW_LOCK_TEST-4c.o RW_lock.o
	$(CC) $(CFLAGS) -o $(TARGET) RW_LOCK_TEST-4c.o RW_lock.o

RW_LOCK_TEST-4c.o: RW_LOCK_TEST-4c.c RW_lock.h
	$(CC) $(CFLAGS) -c RW_LOCK_TEST-4c.c

RW_lock.o: RW_lock.c RW_lock.h
	$(CC) $(CFLAGS) -c RW_lock.c

clean:
	rm $(TARGET).o