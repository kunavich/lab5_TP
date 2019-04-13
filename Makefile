all: begin

begin: main.o Source.o sqlite3.o
	gcc main.o Source.o sqlite3.o -o begin

main.o: Main.c
	gcc -c Main.c

Source.o: Source.c
	gcc -c Source.c

sqlite3.o: sqlite3.c
	gcc -c sqlite3.c

clean:
	rm -rf *.o begin
