CC=gcc
CFLAGS=-lm -I -g -Wall -lgmp
DEPS = util.o
TARGET = dh_assign_1 rsa_assign_1 unit_testing

all: $(TARGET)


dh_assign_1: $(DEPS) dh_assign_1.o
	$(CC) $^ -o $@ $(CFLAGS)

rsa_assign_1: $(DEPS) rsa_assign_1.o
	$(CC) $^ -o $@ $(CFLAGS) 


unit_testing: $(DEPS) unit_testing.o
	$(CC) $^ -o $@ $(CFLAGS)

clean:
	$(RM) $(TARGET)
	$(RM) -f *.txt *.o *.key dh_assign_1 rsa_assign_1 unit_testing
	



#CC=gcc
#CFLAGS = -lm -I -g -Wall
#TARGET = dh_assign_1 rsa_assign_1 unit_testing
