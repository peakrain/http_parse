#ifndef _tool_h
#define _tool_h

#include"packet_info.h"
#include<time.h>
/****************************************
 * Function:format_time
 * Description:格式化时间
 * Calls:strftime,localtime
 * Called By:
 * Input:时间格式format，time_t类型时间
 * Output:无
 * Return:格式化后的本地时间字符串
****************************************/
char *format_time(char *format,time_t time);

/****************************************
 * Function:getLine
 * Description:逐行获取字符串
 * Calls:sscanf
 * Called By:
 * Input:存放截获的字符buf,原字符串data
 * Output:无
 * Return:成功返0，失败返EOF
****************************************/
int getLine(char *buf,char **data);

/****************************************
 * Function:print
 * Description:打印五元组信息
 * Calls:printf
 * Called By:
 * Input:Socket
 * Output:Sip,Dip,Sport,Dport,Protocol
 * Return:
****************************************/
void print(Socket *info);
/****************************************
 * Function:
 * Description:
 * Calls:
 * Called By:
 * Input:
 * Output:
 * Return:
****************************************/
#endif