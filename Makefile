#compiler and flags
CC			= gcc
CFLAGS	+= -Wall
CPPFLAGS	+= -I lib -I include -lcjson -I.

#linker and flags
LINKER	= gcc
LFLAGS	+= -I lib -I include
LDLIBS 	+= -lm -I /usr/include/postgresql -lpq -lcurl

#target directory and name of executable
EXE		= bin/requests

#directories
SRCDIR	= src
OBJDIR	= obj
BINDIR	= bin

SRC		= $(wildcard $(SRCDIR)/*.c)
OBJ		= $(SRC:$(SRCDIR)/%.c=$(OBJDIR)/%.o)
rm			= rm -f

.PHONY: all clean .FORCE

.FORCE:

all: $(EXE)

$(EXE): $(OBJ)
	@$(CC) $(OBJ) $(LFLAGS) $(LDLIBS) -o $@

$(OBJ): $(OBJDIR)/%.o: $(SRCDIR)/%.c .FORCE
	$(CC) -c $(CFLAGS) -o $@ $< $(CPPFLAGS)

clean:
	$(rm) $(EXE)
	@echo "Cleanup complete!"
