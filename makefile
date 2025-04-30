CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -pedantic
LDFLAGS = -lreadline

TARGET = mus

SRCS = MiniShell.cpp

OBJS = $(SRCS:.cpp=.o)

# Default target
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

# Compile source files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean build files
clean:
	rm -f $(OBJS) $(TARGET)

# Clean and rebuild
rebuild: clean all

# Run the shell
run: $(TARGET)
	./$(TARGET)

# Debugging target
debug: $(TARGET)
	gdb ./$(TARGET)

.PHONY: all clean rebuild run
