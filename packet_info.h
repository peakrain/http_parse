#ifndef _packet_info_h
#define _packet_info_h
/*******************************
 *五元组
********************************/
typedef struct{
	char src_ip[20];//源IP地址
	char dst_ip[20];//目的IP地址
	int src_port;//源端口
	int dst_port;//目的端口
	int prot;//网络协议号
}Socket;
/*******************************
 *
 * *****************************/
typedef struct{
	Socket *socket;
	int seq;
	int len;
	char *payload;
}http_info;
/*******************************
 *
 * *****************************/
typedef struct{
	Socket *socket; 
	char payload[65535];
}http_session;
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
 *Function:
 *Description:
 *Calls:
 *Called By:
 *Input:
 *Output:
 *Return:
 *******************************/
/*******************************
 *Function:
 *Description:
 *Calls:
 *Called By:
 *Input:
 *Output:
 *Return:
 *******************************/
/*******************************
 *Function:
 *Description:
 *Calls:
 *Called By:
 *Input:
 *Output:
 *Return:
 *******************************/
#endif