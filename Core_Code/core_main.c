#include "core_main.h"

/*
**********************************************************************************************************
                                            start task
**********************************************************************************************************
*/ 
OS_TCB	StartTaskTCB;
CPU_STK	START_TASK_STK[START_STK_SIZE];
/*
**********************************************************************************************************
                                    flight hardware initialization task
**********************************************************************************************************
*/ 
OS_TCB	FlightINITTCB;
CPU_STK	FLIGHT_INIT_TASK_STK[FLIGHT_INIT_STK_SIZE];
CPU_INT08U flight_init_task_parg = MOTORRESET;
/*
**********************************************************************************************************
                                            flight routine task
**********************************************************************************************************
*/ 
OS_TCB	FlightRoutineTCB;
CPU_STK	FLIGHT_ROUTINE_TASK_STK[FLIGHT_ROUTINE_STK_SIZE];
/*
**********************************************************************************************************
                                        flight routine control task
**********************************************************************************************************
*/ 
OS_TCB	FlightRoutineControlTCB;
CPU_STK	FLIGHT_ROUTINE_CONTROL_TASK_STK[FLIGHT_ROUTINE_CONTROL_STK_SIZE];
/*
**********************************************************************************************************
                                        flight routine KS103 task
**********************************************************************************************************
*/ 
OS_TCB	FlightRoutineKS103TCB;
CPU_STK	FLIGHT_ROUTINE_KS103_TASK_STK[FLIGHT_ROUTINE_KS103_STK_SIZE];
/*
**********************************************************************************************************
                                              camera task
**********************************************************************************************************
*/ 
OS_TCB	CameraTCB;
CPU_STK	CAMERA_TASK_STK[CAMERA_STK_SIZE];
/*
**********************************************************************************************************
                                        MAIN.C Function Decleration
**********************************************************************************************************
*/ 

int main(void)
{
  OS_ERR err;
  
  IntMasterDisable();                                                           /* BSP_Level interrupt disable, better than CPU_IntDis */
  //  CPU_IntDis();
  //IntPriorityGroupingSet(3);
  IntRegister(FAULT_PENDSV,OS_CPU_PendSVHandler);
  IntRegister(FAULT_SYSTICK,OS_CPU_SysTickHandler);
    OSInit(&err);		                                                        //��ʼ��UCOSIII
  /* ������ʼ����                     */  
  OSTaskCreate((OS_TCB 	* )&StartTaskTCB,		                        //������ƿ�
               (CPU_CHAR	* )"start task", 		                //��������
               (OS_TASK_PTR )start_task, 			                //������
               (void		* )0,					        //���ݸ��������Ĳ���
               (OS_PRIO	  )START_TASK_PRIO,                                     //�������ȼ�
               (CPU_STK   * )&START_TASK_STK[0],	                        //�����ջ����ַ
               (CPU_STK_SIZE)START_STK_SIZE/10,	                                //�����ջ�����λ
               (CPU_STK_SIZE)START_STK_SIZE,		                        //�����ջ��С
               (OS_MSG_QTY  )0,					                //�����ڲ���Ϣ�����ܹ����յ������Ϣ��Ŀ,Ϊ0ʱ��ֹ������Ϣ
               (OS_TICK	  )0,					                //��ʹ��ʱ��Ƭ��תʱ��ʱ��Ƭ���ȣ�Ϊ0ʱΪĬ�ϳ��ȣ�
               (void   	* )0,					                //�û�����Ĵ洢��
               (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,           //����ѡ��
               (OS_ERR 	* )&err);				                //��Ÿú�������ʱ�ķ���ֵ	 
  
  OSStart(&err);                                                                //����UCOSIII 
  while(1);
}

//��ʼ������
void start_task(void *p_arg)
{
  CPU_INT32U  cpu_clk_freq;	
  CPU_INT32U  cnts;  
  OS_ERR err;
  p_arg = p_arg;
  
  BSP_Init();
  CPU_Init();
  Mem_Init();
  
  cpu_clk_freq = BSP_SysClkFreqGet();                                           /* Determine SysTick reference freq.                    */
  cnts         = cpu_clk_freq / (CPU_INT32U)OSCfg_TickRate_Hz;                  /* Determine nbr SysTick increments                     */
  OS_CPU_SysTickInit(cnts);	  
  
  /*
  *********************************************************************************************************
  *                                        SYSTEMRELATED MACROS
  *********************************************************************************************************
  */
#if OS_CFG_STAT_TASK_EN > 0u
  OSStatTaskCPUUsageInit(&err);  	                                        //ͳ������                
#endif
  
#ifdef CPU_CFG_INT_DIS_MEAS_EN		                                        //���ʹ���˲����жϹر�ʱ��
  CPU_IntDisMeasMaxCurReset();	
#endif
  
#if     OS_CFG_SCHED_ROUND_ROBIN_EN                                             //��ʹ��ʱ��Ƭ��ת��ʱ��
  OSSchedRoundRobinCfg(DEF_ENABLED,1,&err);  
#endif		
  /*
  **********************************************************************************************************
  **********************************************************************************************************
  */  
  OSTaskCreate((OS_TCB 	* )&FlightINITTCB,		
               (CPU_CHAR	* )"flight hardware initialization task", 		
               (OS_TASK_PTR  )flight_init_task, 			
               (void	* )&flight_init_task_parg,					
               (OS_PRIO	  )FLIGHT_INIT_TASK_PRIO,     
               (CPU_STK    * )&FLIGHT_INIT_TASK_STK[0],	
               (CPU_STK_SIZE )FLIGHT_INIT_STK_SIZE/10,	
               (CPU_STK_SIZE )FLIGHT_INIT_STK_SIZE,		
               (OS_MSG_QTY   )0,					
               (OS_TICK	  )0,					
               (void   	* )0,					
               (OS_OPT       )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,
               (OS_ERR 	* )&err); 
  //  OS_TaskSuspend((OS_TCB*)&StartTaskTCB,&err);		                        //����ʼ����
  OSTaskDel(&StartTaskTCB, &err);
}


void flight_init_task(void *p_arg)
{
  OS_ERR err; 
  p_arg = p_arg;
  
  led_init();
  PX4Flow_uart_init(115200,UART6_IRQHandler);
  
  SysCtlPeripheralEnable(SYSCTL_PERIPH_EEPROM0);
  EEPROMInit();
  
  KS103_init();
  mpu6050_soft_init();  
  
  UART1_STInit(9600);
  
  GPIO_PINB7init();                                                             //wireless uart config
  
  tim1_init(PIT_IRQHandler);

  Camera_init();
  
  IntPriorityGroupingSet(3);  
  
  IntPrioritySet(INT_GPIOC, 0x00);                                              //PX4flow init	
  IntPrioritySet(INT_UART6, 0x01<<5);
  IntPrioritySet(INT_UART1, 0x02<<6);

//  IntPrioritySet(INT_TIMER1A, 0x02<<6);
//  IntPrioritySet(FAULT_SYSTICK, 0x01<<6);
//  IntPrioritySet(INT_GPIOC, 0x00);
  
  data_common_init();                                                           //���ݳ�����ʼ��
  param_common_init();                                                          //����������ʼ��
  motorcontrol_init();
 
  
  if(*(CPU_INT08U*)p_arg)
    motor_reset();                                                              //::note::Why the motors don't rotate if not in debug mode?              
  
  OSTimeDlyHMSM(0,0,5,0,OS_OPT_TIME_HMSM_STRICT,&err);                          //��ʱ200ms
  AttitudeInitialize();
  StabilizationInitialize();
  
  while(!nrf_init(PORTC_IRQHandler));
  
  IntMasterEnable();  
  
  OSTaskCreate((OS_TCB 	* )&FlightRoutineTCB,		
               (CPU_CHAR	* )"flight routine task", 		
               (OS_TASK_PTR  )flight_routine_task, 			
               (void	* )0,					
               (OS_PRIO	  )FLIGHT_ROUTINE_TASK_PRIO,     
               (CPU_STK    * )&FLIGHT_ROUTINE_TASK_STK[0],	
               (CPU_STK_SIZE )FLIGHT_ROUTINE_STK_SIZE/10,	
               (CPU_STK_SIZE )FLIGHT_ROUTINE_STK_SIZE,		
               (OS_MSG_QTY   )0,					
               (OS_TICK	  )0,					
               (void   	* )0,					
               (OS_OPT       )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,
               (OS_ERR 	* )&err);
  
  OSTaskCreate((OS_TCB 	* )&FlightRoutineControlTCB,		
               (CPU_CHAR	* )"flight routine control task", 		
               (OS_TASK_PTR  )flight_routine_control_task, 			
               (void	* )0,					
               (OS_PRIO	  )FLIGHT_ROUTINE_CONTROL_TASK_PRIO,     
               (CPU_STK    * )&FLIGHT_ROUTINE_CONTROL_TASK_STK[0],	
               (CPU_STK_SIZE )FLIGHT_ROUTINE_CONTROL_STK_SIZE/10,	
               (CPU_STK_SIZE )FLIGHT_ROUTINE_CONTROL_STK_SIZE,		
               (OS_MSG_QTY   )0,					
               (OS_TICK	  )0,					
               (void   	* )0,					
               (OS_OPT       )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,
               (OS_ERR 	* )&err);
  
  OSTaskCreate((OS_TCB 	* )&FlightRoutineKS103TCB,		
               (CPU_CHAR	* )"flight routine ks103 task", 		
               (OS_TASK_PTR  )flight_routine_ks103_task, 			
               (void	* )0,					
               (OS_PRIO	  )FLIGHT_ROUTINE_KS103_TASK_PRIO,     
               (CPU_STK    * )&FLIGHT_ROUTINE_KS103_TASK_STK[0],	
               (CPU_STK_SIZE )FLIGHT_ROUTINE_KS103_STK_SIZE/10,	
               (CPU_STK_SIZE )FLIGHT_ROUTINE_KS103_STK_SIZE,		
               (OS_MSG_QTY   )0,					
               (OS_TICK	  )0,					
               (void   	* )0,					
               (OS_OPT       )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,
               (OS_ERR 	* )&err); 
  
  OSTaskCreate((OS_TCB 	* )&CameraTCB,		
               (CPU_CHAR	* )"camera task", 		
               (OS_TASK_PTR  )camera_task, 			
               (void	* )0,					
               (OS_PRIO	  )CAMERA_TASK_PRIO,     
               (CPU_STK    * )&CAMERA_TASK_STK[0],	
               (CPU_STK_SIZE )CAMERA_STK_SIZE/10,	
               (CPU_STK_SIZE )CAMERA_STK_SIZE,		
               (OS_MSG_QTY   )0,					
               (OS_TICK	  )0,					
               (void   	* )0,					
               (OS_OPT       )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,
               (OS_ERR 	* )&err);   
  
  OS_TaskSuspend((OS_TCB*)&FlightINITTCB, &err);
}

