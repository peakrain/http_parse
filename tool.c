#include"tool.h"
#include<string.h>
#include<malloc.h>
int ascii[2]={0x0A,0x0B};
int split(unsigned char **head,int *head_len,unsigned char ** body,int *body_len,unsigned char *source,int len)
{
	int i;
	char *temp=(char *)malloc(sizeof(char));
	unsigned char *p=source;
	for(i=0;i<len;i++)
		if(i<len-4)
		{
			if(p[i]==0x0d&&p[i+1]==0x0a&&p[i+2]==0x0d&&p[i+3]==0x0a)
			{
				*head_len=i+2;
				memcpy(*head,source,*head_len);
				printf("head:%s\n",source);
				print_02x(temp,*head_len);
				printf("headtest:%c\n",source[0]);
				p=p+i+4;
				*body_len=len-(i+4);
			//	memcpy(temp,p,*body_len);
				printf("body:\n");
			//	print_02x(temp,*body_len);
				return 0;
			}
		}
	return EOF;
}
int is_ascii(unsigned char c)
{
	int up=c-0x7E;
	int down=0x20-c;
	if(up<=0&&down<=0)
		return 1;
	/*int i;
	for(i=0;i<2;i++)
	if(c==ascii[i])
		return 1;*/
	return 0;
}
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
void print_02x(u_char *data,int len)
{
	int i;
	for(i=0;i<len;i++)
	{
		printf("%02x ",data[i]);
		if((i+1)%16==0)
			printf("\n");
	}
	printf("\n");
	int up,down;
	for(i=0;i<len;i++)
	{
		if(i<len-1&&data[i+1]==0x0a&&data[i]==0x0d)
		{
			printf("\n");
			i++;
			continue;
		}
		if(is_ascii(data[i]))
			printf("%c",data[i]);
		else
			printf(".");
	}
	printf("\n");
} 
