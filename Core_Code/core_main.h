#ifndef __CORE_MAIN_H
#define __CORE_MAIN_H

#include <chiplevel_includes.h>
#include <includes.h>

#include "data_common.h"
#include "param_common.h"
#include "driverlib/eeprom.h"
#include "attitudesolving.h"
#include "stabilization.h"
#include "motor_control.h"
#include "sensorfetch.h"
#include "PX4Flow.h"
#include "KS103.h"

#include "common.h"
#include "include.h"
#include "IRQ_handler.h"

#include "core_uart.h"
#include "core_camera.h"

/*
**********************************************************************************************************
*UCOSIII���������ȼ��û�������ʹ��
*����Щ���ȼ��������UCOSIII��5��ϵͳ�ڲ�����
*���ȼ�0���жϷ������������� OS_IntQTask()
*���ȼ�1��ʱ�ӽ������� OS_TickTask()
*���ȼ�2����ʱ���� OS_TmrTask()
*���ȼ�OS_CFG_PRIO_MAX-2��ͳ������ OS_StatTask()
*���ȼ�OS_CFG_PRIO_MAX-1���������� OS_IdleTask()
**********************************************************************************************************
*/ 
/*
**********************************************************************************************************
                                            start task
**********************************************************************************************************
*/ 
#define START_TASK_PRIO      3
#define START_STK_SIZE       512
extern OS_TCB	StartTaskTCB;
extern CPU_STK	START_TASK_STK[START_STK_SIZE];
void start_task(void *p_arg);

/*
**********************************************************************************************************
                                    flight hardware initialization task
**********************************************************************************************************
*/ 
#define FLIGHT_INIT_TASK_PRIO      3
#define FLIGHT_INIT_STK_SIZE       512
extern OS_TCB	FlightINITTCB;
extern CPU_STK	FLIGHT_INIT_TASK_STK[FLIGHT_INIT_STK_SIZE];
void flight_init_task(void *p_arg);

#define MOTORRESET                 0u
extern CPU_INT08U flight_init_task_parg;
/*
**********************************************************************************************************
                                            flight routine task
**********************************************************************************************************
*/ 
#define FLIGHT_ROUTINE_TASK_PRIO      4
#define FLIGHT_ROUTINE_STK_SIZE       1536
extern OS_TCB	FlightRoutineTCB;
extern CPU_STK	FLIGHT_ROUTINE_TASK_STK[FLIGHT_ROUTINE_STK_SIZE];
void flight_routine_task(void *p_arg);
/*
**********************************************************************************************************
                                        flight routine control task
**********************************************************************************************************
*/ 
#define FLIGHT_ROUTINE_CONTROL_TASK_PRIO      5
#define FLIGHT_ROUTINE_CONTROL_STK_SIZE       128
extern OS_TCB	FlightRoutineControlTCB;
extern CPU_STK	FLIGHT_ROUTINE_CONTROL_TASK_STK[FLIGHT_ROUTINE_CONTROL_STK_SIZE];
void flight_routine_control_task(void *p_arg);
/*
**********************************************************************************************************
                                        flight routine KS103 task
**********************************************************************************************************
*/ 
#define FLIGHT_ROUTINE_KS103_TASK_PRIO        6
#define FLIGHT_ROUTINE_KS103_STK_SIZE       128
extern OS_TCB	FlightRoutineKS103TCB;
extern CPU_STK	FLIGHT_ROUTINE_KS103_TASK_STK[FLIGHT_ROUTINE_KS103_STK_SIZE];
void flight_routine_ks103_task(void *p_arg);

/*
**********************************************************************************************************
                                              camera task
**********************************************************************************************************
*/ 
#define CAMERA_TASK_PRIO        10
#define CAMERA_STK_SIZE       128
extern OS_TCB	CameraTCB;
extern CPU_STK	CAMERA_TASK_STK[CAMERA_STK_SIZE];
void camera_task(void *p_arg);

/*
**********************************************************************************************************
                                        MAIN.C Function Decleration
**********************************************************************************************************
*/ 
#endif  //__MAIN_H__
