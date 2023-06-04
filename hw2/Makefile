# Compiler and flags
CXX = g++
CXXFLAGS = -Wall -Wextra -pedantic

# Directories
SRCDIR = src
INCDIR = includes
BUILDDIR = build
TARGET = operateArrays

# Source and object files
SRCS = $(wildcard $(SRCDIR)/*.cpp)
OBJS = $(patsubst $(SRCDIR)/%.cpp,$(BUILDDIR)/%.o,$(SRCS))
DEPS = $(wildcard $(INCDIR)/*.h)

$(TARGET): $(OBJS)
	g++ $(CXXFLAGS) -o $@ $^

$(BUILDDIR)/%.o: $(SRCDIR)/%.cpp $(DEPS)
	g++ $(CXXFLAGS) -c -o $@ $< -I$(INCDIR)

# Clean the build directory
clean:
	rm -f $(BUILDDIR)/*.o $(TARGET)
	rm $(TARGET)

