#include"tool.h"
#include<math.h>
#include<string.h>
#include<malloc.h>
#include<zlib.h>
#include"pat_gzip.h"
int ascii[2]={0x0A,0x0B};
int split(unsigned char **head,int *head_len,unsigned char ** body,int *body_len,unsigned char *source,int len)
{
	int i;
	char *temp=(char *)malloc(sizeof(char));
	unsigned char *p=source;
	for(i=0;i<len;i++)
		if(i<len-3)
		{
			if(p[i]==0x0d&&p[i+1]==0x0a&&p[i+2]==0x0d&&p[i+3]==0x0a)
			{
				*head_len=i+2;
				*body_len=len-(i+4);
				if(*body_len<=0)
					return EOF;
				memcpy(*head,source,*head_len);
				p=p+i+4;
				memcpy(*body,p,*body_len);
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
int ctoi(unsigned char *str,int len)
{
	int i,result=0,temp;
	for(i=0;i<len;i++)
	{
		if(str[i]<=0x39&&str[i]>=0x30)
			temp=(int)(str[i]-'0');
		else if(str[i]<=0x46&&str[i]>=0x41)
			temp=(int)(str[i]-'A')+10;
		else if(str[i]<=0x66&&str[i]>=0x61)
			temp=(int)(str[i]-'a')+10;
		else
			return EOF;
		result+=temp*pow(16,len-i-1);
	
	}
	return result;
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
int getChunk(unsigned char **source,int slen)
{
	int i;
	unsigned char *p=*source;
	unsigned char *temp=(unsigned char*)malloc(sizeof(unsigned char)*10);
	unsigned char *out_stream=(unsigned char *)malloc(sizeof(unsigned char)*65535*2);
	int len=0;
	int l;
	int done=0;
	while(slen>0&&done==0)
	{	
		for(i=0;i<slen;i++)
			if(i<slen-1&&p[i]==0x0d&&p[i+1]==0x0a)
			{	
				if(i==0)
				{
					p=p+2;
					slen=slen-2;
					break;
				}
				memcpy(temp,p,i);	
				l=ctoi(temp,i);
				if(l==0)
				{
					done=1;
					break;
				}		
				p=p+i+2;
				memcpy(out_stream+len,p,l);
				len+=l;
				
				p=p+l;
				slen=slen-l-(i+2);
				break;
			}
//		printf("slen:%d\n",slen);		
	}
//	print_char(out_stream,len);	
	int out_len=655350;
	unsigned char *out_data=(unsigned char*)malloc(sizeof(unsigned char)*out_len);
	pat_gzip_uncompress(out_stream,len,&out_data,&out_len);
	printf("len:%d \ndata:\n%s\n",out_len,out_data);
	int in_len=1000;
	unsigned char *in_data=(unsigned char *)malloc(sizeof(unsigned char)*in_len);
	if(pat_gzip_compress(out_data,out_len,&in_data,&in_len)==0)
	{
		print_02x(in_data,in_len);
		printf("len:%d \ndata:\n%s\n",in_len,in_data);
	}

	return EOF;
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
void print_02x(unsigned char *data,int len)
{
	int i;
	for(i=0;i<len;i++)
	{
		printf("%02x ",data[i]);
		if((i+1)%16==0)
			printf("\n");
	}
	printf("\n");
}
void print_c(unsigned char *data,int len)
{
	int i;
	for(i=0;i<len;i++)
		printf("%c",data[i]);
}
void print_char(unsigned char *data,int len)
{
	int i,up,down;
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
