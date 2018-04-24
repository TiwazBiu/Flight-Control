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
        //����picth��rollָ��
	uint16 move_p_r; 
        //Yƫ����
	int16 deltaY;
        //Xƫ����
	int16 deltaX;
        //YAWָ��
	uint16 holdposition;
        //����ָ��
	uint16 movethrottle;
        //�ҵ����
	uint16 fndresult;
        //״̬
	uint16 state;
};
extern struct ImgResult imgresult;//ͼ�������ṹ��
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