#ifndef _sup_packet_h
#define _sup_packet_h

#include"LinkList.h"
#include<stdint.h>
/*******************************
 *五元组
********************************/
typedef struct socket{
	char src_ip[20];//源IP地址
	char dst_ip[20];//目的IP地址
	int src_port;//源端口
	int dst_port;//目的端口
	int prot;//网络协议号
}Socket;
/*******************************
 *数据包主要数据
 * *****************************/
typedef struct{
	Socket *socket;
	int  seq;
	int ack_seq;
	int syn;
	int ack;
	int  fin;
	int len;
	unsigned char *payload;
}packet_info;
/*******************************
 *Function:is_sameconnection
 *Description:判断是否是同一个连接
 *Calls:
 *Called By:
 *Input:socket1,socket2
 *Output:
 *Return:是返1，否返0
 *******************************/
int is_sameconnection(Socket *socket1,Socket *socket2);
/*******************************
 *Function:is_samedirection
 *Description:判断是否同一连接的同一方向
 *Calls:
 *Called By:
 *Input:socket1,sockt2
 *Output:
 *Return:是返1,否返0
 *******************************/
int is_samedirection(Socket *socket1,Socket *socket2);
/*******************************
 *Function:socket_copy
 *Description:复制socket结构休
 *Calls:strpcy
 *Called By:
 *Input:Socket
 *Output:
 *Return:成功返0，失败返-1
 *******************************/
void socket_copy(Socket *socket1,Socket *socket2);
int get_packet(int num,char *filter,list_t *list_packet,char *filename);
#endif
