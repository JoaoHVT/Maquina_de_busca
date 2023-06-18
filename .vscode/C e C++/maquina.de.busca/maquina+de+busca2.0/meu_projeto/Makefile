CC = g++
SRCDIR = src
INCDIR = include
OBJDIR = obj
CXXFLAGS = -std=c++17 -Wall -Iinclude

SRCS = $(wildcard $(SRCDIR)/*.cpp)
OBJS = $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o,$(SRCS))

all: meu_programa

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	if not exist $(OBJDIR) mkdir $(OBJDIR)
	$(CC) $(CFLAGS) $(CXXFLAGS) -I$(INCDIR) -c -o $@ $<

meu_programa: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	@if exist $(OBJDIR) rmdir /s /q $(OBJDIR)
	@del /q meu_programa
	@del /q *.exe

run: meu_programa
	./meu_programa
