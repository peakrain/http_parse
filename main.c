#include"packet_info.h"
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
	analysis(n,"tcp",filename);
	return 0;
}
