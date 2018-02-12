#**************************************************
#*                  Galen Helfter
#*               ghelfter@gmail.com
#*                     Makefile
#**************************************************

SHELL = /bin/sh

PROJ = runestone_gen

# Compiler and linker flags
CC = gcc
CFLAGS = -g -W -Wall -Wdeclaration-after-statement \
		 -pedantic -Wextra -O0 -std=c89 $(INC)
LDFLAGS = -g -W -Wall -Wdeclaration-after-statement \
		  -pedantic -Wextra -O0 -std=c89 $(LIB)

LIBS =

TARFILE = $(PROJ).tar.gz
ZIPFILE = $(PROJ).zip


CLEANFILES = *.o *.out core.* $(TARFILE) $(ZIPFILE)

# Object file list
OBJECTS =	main.o \
			generate.o \
			mchoice.o \
			fillblank.o \
			util.o

# Programs

PROGS = run

all:		$(PROGS)

run:		$(OBJECTS) $(LIBS)
			$(CC) $(LDFLAGS) -o $@ $(OBJECTS) $(LIBS)


.PHONY: clean tar zip
clean:
		rm -f $(PROGS) $(CLEANFILES)
tar:
		rm -f $(TARFILE)
		tar -czvf $(TARFILE) ./*
zip:
		rm -f $(ZIPFILE)
		zip $(ZIPFILE) ./*
