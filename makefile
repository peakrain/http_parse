OBJ=main.o sup_packet.o  LinkList.o pat_print.o
GOBJ=main.c sup_packet.c LinkList.c pat_print.c
LIB=-lpcap -lm -lz
main:${OBJ}
	gcc -o main ${OBJ}  ${LIB}
clean:
	rm -f *.o main core.*
gdb:
	gcc -g -o main ${GOBJ} ${LIB}
