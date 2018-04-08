#compiler and flags
CC			= gcc
CFLAGS	+= -Wall
CPPFLAGS	+=-I include -lcjson

#linker and flags
LINKER	= gcc
LIB_PATH	= -I /usr/include/postgresql
LFLAGS	+= -I lib -I include
LDLIBS 	+= -lm $(LIB_PATH) -lpq -lcurl

#target directory and name of executable
EXE	= bin/requests

#directories
SRCDIR	= src
OBJDIR	= obj
BINDIR	= bin

SRC		= $(wildcard $(SRCDIR)/*.c)
OBJ		= $(SRC:$(SRCDIR)/%.c=$(OBJDIR)/%.o)
rm			= rm -f

.PHONY: all clean

all: $(EXE)

$(EXE): $(SRC)
	$(CC) $(LFLAGS) $^ $(LDLIBS) -o $@

$(OBJDIR)/.o: $(SRCDIR)/%.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

clean:
	$(rm) $(EXE)
	@echo "Cleanup complete!"
