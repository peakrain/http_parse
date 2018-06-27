OBJ=main.o packet_info.o  tool.o 
GOBJ=main.c packet_info.c  tool.c
LIB=-lpcap -lm -lz
main:${OBJ}
	gcc -o main ${OBJ}  ${LIB}
clean:
	rm -f *.o main core.*
gdb:
	gcc -g -o main ${GOBJ} ${LIB}
