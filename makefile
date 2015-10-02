SRCDIR=src/
BINDIR=bin/
DESTDIR=/$(BINDIR)  
INSTALL_LOCATION=$(DESTDIR)/usr/bin/
CC=gcc
NAME=threadbench
CFLAGS:=$(shell dpkg-buildflags --get CFLAGS)
LDFLAGS:=$(shell dpkg-buildflags --get LDFLAGS)
C_FILES := $(wildcard $(SRCDIR)*.c)
O_FILES := $(C_FILES:.c=.o)

all: $(NAME) clean
$(NAME): $(O_FILES)
	$(CC) -pthread $(LDFLAGS) $(CFLAGS) -o $(BINDIR)/$@ $^ -fno-inline -Wall -Werror -O0 -lm -leasy -lrt -fopenmp
%.o: %.c
	$(CC) -pthread -c -o $@ $< -lm -leasy -lrt -fopenmp
clean:
	rm -f $(SRCDIR)*.o
cleanall:
	rm -f $(SRCDIR)*.o
	rm -f $(BINDIR)*
