all: begin

begin: main.o Source.o sqlite3.o
	g++ main.o Source.o sqlite3.o -o begin

main.o: Main.c
	g++ -c Main.c

Source.o: Source.c
	g++ -c Source.c

sqlite3.o: sqlite3.c
	g++ -c sqlite3.c

clean:
	rm -rf *.o begin
