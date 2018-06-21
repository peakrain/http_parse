#include"tool.h"
#include<string.h>
#include<malloc.h>
char *format_time(char *format,time_t time)
{	
	struct tm *timeptr=localtime(&time);
	char timestr[20];
	strftime(timestr,sizeof(timestr),format,timeptr);
	char *ltime=(char *)malloc(sizeof(char));
	strcpy(ltime,timestr);
	return ltime;
}
int getLine(char *buf,char **data)
{
	char *p=*data;
	int t_len=strlen(p);
	if(t_len<=0)
		return EOF;
	int ebuf=sscanf(p,"%[^\r\n]",buf);
	if(ebuf==0)
		strcpy(buf,"");
	int len=strlen(buf)+sizeof("\r\n");
	p=p+len-1;
	*data=p;
	return ebuf;
}
void print(Socket *info)
{
	printf("Src_IP:%s ",info->src_ip);
	printf("Dst_IP:%s ",info->dst_ip);
	printf("Src_Port:%d ",info->src_port);
	printf("Dst_Port:%d ",info->dst_port);
	printf("Protocol:%d\n",info->prot);	
}
 
