CC = g++
SRCDIR = src
OBJDIR = obj
INCDIR = include
CXXFLAGS = -std=c++17 -Wall -Iinclude

SRCS = $(wildcard $(SRCDIR)/*.cpp)
OBJS = $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o,$(SRCS))

all: meu_programa

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp | $(OBJDIR)
	$(CC) $(CXXFLAGS) -c -o $@ $<

$(OBJDIR):
	mkdir -p $(OBJDIR)

meu_programa: $(OBJS)
	$(CC) $(CXXFLAGS) -o $@ $^

clean:
	rm -rf $(OBJDIR)
	rm -f meu_programa

run: meu_programa
	./meu_programa
