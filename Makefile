vcraps: crapslib.o vcraps.o
	cc -O -o vcraps vcraps.o crapslib.o -lncurses

vcraps.o: vcraps.c crapslib.h
	cc -O -I. -c vcraps.c

crapslib.o: crapslib.c crapslib.h
	cc -O -I. -c crapslib.c

clean:
	-rm -f vcraps *.o *.a *~ *.BAK *.CKP core a.out mon.out gmon.out

shar:
	shar -n1 -e2 README Makefile crapslib.c crapslib.h > craps.1.shar
	shar -n2 -e2 vcraps.c > craps.2.shar
