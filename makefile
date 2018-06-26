OBJ=main.o packet_info.o http_parse.o tool.o 
GOBJ=main.c packet_info.c http_parse.c tool.c
LIB=pcap
main:${OBJ}
	gcc -o main ${OBJ} -l ${LIB}
clean:
	rm -f *.o main core.*
gdb:
	gcc -g -o main ${GOBJ} -l ${LIB}
