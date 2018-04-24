#ifndef __RENESAS_INTERFACE_H
#define __RENESAS_INTERFACE_H

#include <core_common.h>
#include <board_includes.h>
#include <device_includes.h>
#include <math_includes.h>
#include <basicflight_includes.h>
#include <app_includes.h>
#include <OS_includes.h>

#define UART2_REC_LEN  			200
struct ImgResult
{
        //发送picth、roll指令
	uint16 move_p_r; 
        //Y偏移量
	int16 deltaY;
        //X偏移量
	int16 deltaX;
        //YAW指令
	uint16 holdposition;
        //油门指令
	uint16 movethrottle;
        //找到结果
	uint16 fndresult;
        //状态
	uint16 state;
};
extern struct ImgResult imgresult;//图像处理结果结构体
extern struct ImgResult imgresult_temp;//buffer
extern int count_right;
extern int count_wrong;
extern void UART2_STInit(uint32 Baud_rate);
extern void UART2_IRQHandler();
extern void UART2SendString(uint8* send);
struct RENESAS_Struct
{
    int16 FLOW_X;
    int16 FLOW_Y;
    int16 YAW;

};

typedef struct RENESAS_Struct RENESAS_Struct;

extern RENESAS_Struct RENESAS;
//extern int recive_sua_flag;
#endif //__RENESAS_INTERFACE_H