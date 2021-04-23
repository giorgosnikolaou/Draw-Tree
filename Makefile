# Paths
LIB = lib
INCLUDE = include
MODULES = modules

# Files to read
FILES = 

# compiler
CC = gcc

# Compile options
CFLAGS = -Wall -Werror -g -I$(INCLUDE)
LDFLAGS = -lm

# .o files
OBJS = main.o $(LIB)/libbmp.a $(MODULES)/AVLTree.o $(MODULES)/draw_tree.o

# Executable
EXEC = main

# Arguments
ARGS =


$(EXEC): $(OBJS)
	$(CC) $(OBJS) -o $(EXEC) $(LDFLAGS)


# To compile libbmp.a, we run make in the lib directory.
$(LIB)/libbmp.a:
	$(MAKE) -C $(LIB) libbmp.a

clean:
	rm -f $(OBJS) $(EXEC)

run: $(EXEC)
	rm -f tree.png
	./$(EXEC) $(ARGS)

valgrind: $(EXEC)
	valgrind --leak-check=full ./$(EXEC) $(ARGS)

time: $(EXEC)
	time ./$(EXEC) $(ARGS)