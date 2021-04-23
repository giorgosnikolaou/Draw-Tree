# Paths
INCLUDE = ../include
MODULES = ../modules

# Files to read
FILES = 

# compiler
CC = gcc

# Compile options
CFLAGS = -Wall -Werror -g -DDRAW -I$(INCLUDE)
LDFLAGS = -lm

# .o files
# We need to compile the tree before draw_tree.o to have the correct defines for the preproccesor
OBJS = main.o $(MODULES)/AVLTree.o $(MODULES)/bmp.o draw_tree.o

# Executable
EXEC = main

# Arguments
ARGS =


$(EXEC): $(OBJS)
	$(CC) $(OBJS) -o $(EXEC) $(LDFLAGS)


clean:
	rm -f $(OBJS) $(EXEC)

run: $(EXEC)
	./$(EXEC) $(ARGS)

valgrind: $(EXEC)
	valgrind --leak-check=full ./$(EXEC) $(ARGS)

time: $(EXEC)
	time ./$(EXEC) $(ARGS)
