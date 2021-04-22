# paths
LIB = lib
INCLUDE = ../AVLTree
MODULES = ../AVLTree

# Files to read
FILES = 

# compiler
CC = gcc

# Compile options. Το -I<dir> λέει στον compiler να αναζητήσει εκεί include files
CFLAGS = -Wall -Werror -g -I$(INCLUDE)
LDFLAGS = -lm

# Αρχεία .o
OBJS = tree_test.o $(LIB)/libbmp.a $(MODULES)/AVLTree.o

# Το εκτελέσιμο πρόγραμμα
EXEC = tree_test

# Παράμετροι για δοκιμαστική εκτέλεση
ARGS =


$(EXEC): $(OBJS)
	$(CC) $(OBJS) -o $(EXEC) $(LDFLAGS)


# το ίδιο για το libbmp.a
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