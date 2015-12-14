# Project Name
PRJNAME=threadbench

# Filepaths
SRCDIR=src
INCDIR=inc
LIBDIR=lib
BINDIR=../Executable
INSTALL=/usr/bin

# Toolchain
CC=gcc

# Find source files
C_FILES := $(shell find -L $(SRCDIR) -name '*.c')
O_FILES := $(C_FILES:.c=.o)

# Find header dirs
INC=$(shell find -L $(INCDIR) -name '*.h' -exec dirname {} \; | uniq)
INC+=$(shell find -L $(LIBDIR) -name '*.h' -exec dirname {} \; | uniq)
INCLUDES=$(INC:%=-I%)

# Compiler Flags
PFLAGS=-pthread
CFLAGS=
CFLAGS+= $(INCLUDES)
CFLAGS+=
EFLAGS=-fno-inline -Wall -Werror -O0
LFLAGS=-lm -lextend -lrt -fopenmp
LDFLAGS:=$(shell dpkg-buildflags --get LDFLAGS)

# Compiling Process
all: $(PRJNAME) clean

$(PRJNAME): $(O_FILES)
	$(CC) $(PFLAGS) $(LDFLAGS) $(CFLAGS) -o $(BINDIR)/$@ $^ $(EFLAGS) $(LFLAGS)

%.o: %.c
	$(CC) $(PFLAGS) -c $(INCLUDES) -o $@ $< $(LFLAGS)

clean:
	rm -f $(SRCDIR)/*.o

cleanall:
	rm -f $(SRCDIR)/*.o
	rm -f $(BINDIR)/*
install:
	cp $(BINDIR)/$(PRJNAME) $(INSTALL)/