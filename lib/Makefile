# Find which os we are using
ifeq ($(OS),Windows_NT)
	SUFFIX = win
else ifeq ($(shell uname -s),Linux)
	SUFFIX = linux
else
	SUFFIX = macos
endif

all: libbmp.a

# We "make" libbmp.a just by copying the file which matched the os we are using 
libbmp.a:
	cp libbmp_$(SUFFIX).a libbmp.a

clean:
	$(RM) libbmp.a