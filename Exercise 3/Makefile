# linking "pgmop.o" with Library "mypgm" to executeable "pgmop"

OTH_OPT = -static
pgmop.elf:pgmop.o libmypgm.a
	gcc -g -Wall $(OTH_OPT) -std=c99 -o pgmop.elf pgmop.o -I. -L. -lmypgm
	ls -l pgmop.elf


# compile "pgmop.c" to "pgmop.o"

pgmop.o:pgmop.c
	gcc -g -Wall -O2 -c pgmop.c -I.


# create static Library "libmypgm" (.a) from libmypgm.o

libmypgm.a:libmypgm.o
	ar -rcs libmypgm.a libmypgm.o


# compile "libmypmg.c" to "libmypmg.o"

libmypgm.o:libmypgm.c
	gcc -g -Wall -O2 -c libmypgm.c -I.

clean:
	rm -rf *.o 2>/dev/null
	rm -rf *~ 2>/dev/null

clear:
	rm -rf *.elf 2>/dev/null
	rm -rf *.a 2>/dev/null
	rm -rf *.o 2>/dev/null
	rm -rf *~ 2>/dev/null

run:
	./pgmop.elf
