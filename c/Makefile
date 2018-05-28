#compiler and flags
CC			= gcc
CFLAGS	+= -Wall -O3 -Wno-unused-variable -std=c99 -fopenmp
CPPFLAGS	+= -I lib -I include -lcjson -I. -ggdb3

#linker and flags
LINKER	= gcc
LFLAGS	+= -I lib -I include
LDLIBS 	+= -lm -I /usr/include/postgresql -lpq -lcurl -fopenmp

#target directory and name of executable
EXE		= bin/main

#directories
SRCDIR	= src
OBJDIR	= obj
BINDIR	= bin

SRC		= $(wildcard $(SRCDIR)/*.c)
OBJ		= $(SRC:$(SRCDIR)/%.c=$(OBJDIR)/%.o)
rm			= rm -f

.PHONY: all clean remove .FORCE

.FORCE:

all: $(EXE)

$(EXE): $(OBJ)
	@$(CC) $(OBJ) $(LFLAGS) $(LDLIBS) -o $@

$(OBJ): $(OBJDIR)/%.o: $(SRCDIR)/%.c .FORCE
	$(CC) -c $(CFLAGS) -o $@ $< $(CPPFLAGS)

clean: remove
	$(rm) $(EXE)
	$(rm) *.log
	@echo "Cleanup complete!"

remove:
	$(rm) $(OBJDIR)/*.o
	$(rm) vgcore*
	@echo "Object files removed"
