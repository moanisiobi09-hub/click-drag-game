# BY: Copilot. Compile all .cpp file in current dir without linking and link with dependencies. Create final.exe.
# Compiler and paths (set manually)
CXX := g++
INCLUDE := "C:\Users\moani\OneDrive\Documents\SimpleFastMediaLibrary\SFML-3.0.2\include"
LINK := "C:\Users\moani\OneDrive\Documents\SimpleFastMediaLibrary\SFML-3.0.2\lib"
COMPFLAGS := -std=c++20
LINKFLAGS := -lsfml-graphics -lsfml-window -lsfml-system

# Automatically gather all .cpp files in the current directory
SRCS := $(wildcard *.cpp)

# Convert .cpp -> .o
OBJECTS := $(SRCS:.cpp=.o)

# Default target
all: final.exe

# Rule to build final executable
final.exe: $(OBJECTS)
	@echo "Linking and making final.exe"
	$(CXX) $(OBJECTS) -o final.exe -L$(LINK) $(LINKFLAGS)

# Pattern rule for compiling each .cpp into .o
%.o: %.cpp
	@echo "Compiling $< source"
	$(CXX) $(COMPFLAGS) -c $< -I$(INCLUDE)

# Clean rule
clean:
	del /Q $(OBJECTS) final.exe