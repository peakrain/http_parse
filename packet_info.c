#include"packet_info.h"
#include<pcap.h>
#include<linux/tcp.h>
#include<linux/ip.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<netinet/if_ether.h>
#include<string.h>
#include<malloc.h>
http_session *session;
http_info *info;
int is_samedirection(Socket *socket1,Socket *socket2)
{
	int sip=strcmp(socket1->src_ip,socket2->src_ip);
	int dip=strcmp(socket1->dst_ip,socket2->dst_ip);
	int sport=socket1->src_port==socket2->src_port;
	int dport=socket1->dst_port==socket2->dst_port;
	int prot=socket1->prot==socket2->prot;
	if(sip==0&&dip==0&&sport==1&&dport==1&&prot==1)
		return 1;
	else
		return 0;
}
int is_samesession(Socket *socket1,Socket *socket2)
{
	int sip=strcmp(socket1->src_ip,socket2->src_ip);
	int dip=strcmp(socket1->dst_ip,socket2->dst_ip);
	int sport=socket1->src_port==socket2->src_port;
	int dport=socket1->dst_port==socket2->dst_port;
	int rsip=strcmp(socket1->src_ip,socket2->dst_ip);
	int rdip=strcmp(socket1->dst_ip,socket2->src_ip);
	int rsport=socket1->src_port==socket2->dst_port;
	int rdport=socket1->dst_port==socket2->src_port;
	int prot=socket1->prot==socket2->prot;
	int samedire=(sip==0&&dip==0&&sport==1&&dport==1);
	int redire=(rsip==0&&rdip==0&&rsport==1&&rdport==1);
	if((samedire==1||redire==1)&&prot==1)
		return 1;
	else
		return 0;
}

void parse_tcp(http_info *info,const u_char *packet,int offset,int len)
{
	struct tcphdr *tcp_h;
	tcp_h=(struct tcphdr*)(packet+offset);
	offset+=tcp_h->doff<<2;
	info->socket->src_port=ntohs(tcp_h->source);
	info->socket->dst_port=ntohs(tcp_h->dest);
	info->seq=ntohl(tcp_h->seq);
	info->len=len-offset;
	info->payload=(char *)(packet+offset);
}
void parse_ip(http_info *info,const u_char *packet,int offset,int len)
{
	if(len>offset)
	{
		struct in_addr addr;
		struct iphdr* ip_h;
		ip_h=(struct iphdr*)(packet+offset);
		addr.s_addr=ip_h->saddr;
		strcpy(info->socket->src_ip,inet_ntoa(addr));
		addr.s_addr=ip_h->daddr;
		strcpy(info->socket->dst_ip,inet_ntoa(addr));
		info->socket->prot=ip_h->protocol;
		offset+=sizeof(struct iphdr);
		if(ip_h->protocol==6)
			parse_tcp(info,packet,offset,len);
	}
	else
	{
		printf("packet length error!\n");
		return;
	}
}
void call_back(u_char *user,const struct pcap_pkthdr *pkthdr,const u_char *packet)
{
	int offset=sizeof(struct ether_header);
	parse_ip(info,packet,offset,pkthdr->len);
	printf("%02x\n",info->seq);	
	if(session->count==0)
	{
		session->socket=info->socket;
		session->count=1;
		memcpy(session->payload+session->offset,info->payload,info->len);
		session->offset+=info->len;
		
	}
	else if(is_samedirection(session->socket,info->socket))
	{
		session->count++;
		memcpy(session->payload+session->offset,info->payload,info->len);
		session->offset+=info->len;
	}
	printf("%d %s\n",session->offset,session->payload);
}
void analysis(int num,char *buf,char *filename)
{
	session=(http_session*)malloc(sizeof(http_session));
	session->socket=(Socket *)malloc(sizeof(Socket));
	info=(http_info *)malloc(sizeof(struct info));
	info->socket=(Socket *)malloc(sizeof(Socket));
	char ebuf[PCAP_ERRBUF_SIZE];
	/*open a pcap file*/
	pcap_t *device=pcap_open_offline(filename,ebuf);
	if(!device)
	{
		printf("error:%s\n",ebuf);
		return;
	}
	/*catch pakcet*/
	int i=0;
	struct bpf_program fp;
	pcap_compile(device,&fp,buf,1,0);
	pcap_setfilter(device,&fp);
	pcap_loop(device,num,call_back,NULL);
	print(session->socket);
	printf("%s\n",session->payload);
	pcap_close(device);	
}
