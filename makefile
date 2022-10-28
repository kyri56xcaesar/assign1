CC=gcc
CFLAGS=-lm -I -g -Wall 
TARGET = dh_assign_1 rsa_assign_1

all: $(TARGET)

#$(TARGET): $(TARGET).c
#	$(CC) -o $@ $^ $(CFLAGS)
dh_assign_1: dh_assign_1.o
	$(CC) $^ -o $@ $(CFLAGS)

rsa_assign_1: dh_assign_1.o
	$(CC) $^ -o $@ $(CFLAGS)

clean:
	$(RM) $(TARGET)
	$(RM) *.txt *.o
	


#
#all: dh_assign_1 rsa_assign_1
#
#dh_assign_1:
#	gcc -Wall -o dh_assign_1 dh_assign_1.c
#
#rsa_assign_1:
#	gcc -Wall -o rsa_assign_1 rsa_assign_1.c
#
#clean:
#	$(RM) *.txt