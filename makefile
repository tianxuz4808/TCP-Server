CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17 -I.

# Find all .cpp files in this directory and all subdirectories
SOURCES = $(shell find . -name "*.cpp")

# Convert .cpp filenames to .o filenames
OBJECTS = $(SOURCES:.cpp=.o)

# Output executable
TARGET = myProgram

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(TARGET)

# Rule to compile each .cpp into .o
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET)