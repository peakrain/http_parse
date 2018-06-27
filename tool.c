#include"tool.h"
#include<math.h>
#include<string.h>
#include<malloc.h>
#include<zlib.h>
int ascii[2]={0x0A,0x0B};
int gzip(unsigned char *pSrc,unsigned int srcSize)
{
	char *pBuf=pSrc+(srcSize-1);
	unsigned int len=*pBuf;
	int result;
	z_stream d_stream;
	int i=0;
	
	printf("#############pSrc 0x%02x 0x%02x 0x%02x 0x%02x\n",pSrc[0],pSrc[1],pSrc[2],pSrc[3]); 
	if((*pSrc!=0x1f)||(*(pSrc+1)!=0x8b))
	{
		printf("non gzip");
		return EOF;
	}
	for(i=0;i<3;i++)
	{
		pBuf--;
		len<<=8;
		len+=*pBuf;
	}
	len=65535;
	if((len==0)||(len>1000000))
	{
		printf("error gzip:%d\n",len);
		return EOF;
	}
	d_stream.zalloc=Z_NULL;
	d_stream.zfree=Z_NULL;
	d_stream.opaque=Z_NULL;
	d_stream.next_in=Z_NULL;
	d_stream.avail_in=0;
	
	result=inflateInit2(&d_stream,47);
	
	if(result!=Z_OK)
	{
		printf("init error:%d\n",result);
		return result;
	}
	
	unsigned char *outstream=(unsigned char*)malloc(sizeof(unsigned char)*65535);
	d_stream.next_in=pSrc;
	d_stream.avail_in=srcSize;
	d_stream.next_out=outstream;
	d_stream.avail_out=65535;
	result=inflate(&d_stream,Z_NO_FLUSH);
	
	switch(result)
	{
		case Z_NEED_DICT:
			result=Z_DATA_ERROR;
		case Z_DATA_ERROR:
		case Z_MEM_ERROR:
			(void)inflateEnd(&d_stream);
			return result;
		
	}
	inflateEnd(&d_stream);
	printf("%s\n",outstream);
	return 0;
}
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
				p=p+i+4;
				*body_len=len-(i+4);
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
	while(slen>0)
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
					printf("slen:%d\n",slen);
				printf("%d\n",len);
				p=p+i+2;
				memcpy(out_stream+len,p,l);
				len+=l;
				
				p=p+l;
				slen=slen-l-(i+2);
				break;
	}		}	
	gzip(out_stream,len);
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
}
void print_char(u_char *data,int len)
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
