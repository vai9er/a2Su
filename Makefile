all: program

program:
	gcc -w -o showFDTables main.c

clean:
	rm -f showFDTables