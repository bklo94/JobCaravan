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
TEST		= bin/test

#directories
SRCDIR	= src
OBJDIR	= obj
BINDIR	= bin
TESTDIR  = test

SRC		= $(wildcard $(SRCDIR)/*.c)
OBJ		= $(SRC:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

TESTSRC	= $(wildcard $(TESTDIR)/*.c)
TESTOBJ	= $(TESTSRC:$(TESTDIR)/%.c=$(OBJDIR)/%.o)

rm			= rm -f

.PHONY: all clean remove .FORCE

.FORCE:

all: $(EXE) $(TEST)

$(EXE): $(OBJ)
	@$(CC) $(OBJ) $(LFLAGS) $(LDLIBS) -o $@

$(OBJ): $(OBJDIR)/%.o: $(SRCDIR)/%.c .FORCE
	$(CC) -c $(CFLAGS) -o $@ $< $(CPPFLAGS)

$(TEST): $(TESTOBJ)
	@$(CC) $(TESTOBJ) $(LFLAGS) $(LDLIBS) -o $@

$(TESTOBJ): $(OBJDIR)/%.o: $(TESTDIR)/%.c .FORCE
	$(CC) -c $(CFLAGS) -o $@ $< $(CPPFLAGS)

clean: remove
	$(rm) $(EXE)
	$(rm) $(TEST)
	$(rm) *.log
	@echo "Cleanup complete!"

remove:
	$(rm) $(OBJDIR)/*.o
	$(rm) $(TESTOBJ)/*.o
	$(rm) vgcore*
	@echo "Object files removed"
