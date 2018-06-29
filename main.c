#include"sup_packet.h"
#include<stdio.h>
int main(int argc,char *argv[])
{
	if(argc!=3)
	{
		printf("syntax error!\n");
		return;
	}
	char *filename=argv[1];
	int n=atoi(argv[2]);
	list_t list=list_create();	
	get_packet(n,"tcp",&list,filename);
	int length=list_length(list);
	printf("length:%d\n",length);
	int i;
	packet_info *info;
	for(i=0;i<length;i++)
	{
		info=(packet_info *)list_getElem(list,i+1);
		pat_print_socket(info->socket);
		printf("len:%d\n",info->len);
		printf("%s\n",info->payload);
	//	pat_print_gzc(info->payload,info->len);
	}
	return 0;
}
