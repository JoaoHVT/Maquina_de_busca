CC = g++
CFLAGS = -std=c++11 -Wall
SRCDIR = src
INCDIR = include
OBJDIR = obj
CXXFLAGS = -std=c++14 -Wall -Iinclude

SRCS = $(wildcard $(SRCDIR)/*.cpp)
OBJS = $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o,$(SRCS))

all: meu_programa

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir $(OBJDIR) 2>/dev/null || true
	$(CC) $(CFLAGS) $(CXXFLAGS) -I$(INCDIR) -c -o $@ $<

meu_programa: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	@rm -rf $(OBJDIR) meu_programa
	@rm -rf *.exe

run: meu_programa
	./meu_programa
