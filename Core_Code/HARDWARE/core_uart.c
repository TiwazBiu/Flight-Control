#include "core_uart.h"


uint16 UART1_RX_STA = 0;
uint8 UART1_RX_BUF[UART1_REC_LEN];

void UART1_STInit(uint32 Baud_rate)
{
  
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
  SysCtlPeripheralEnable(SYSCTL_PERIPH_UART1);
  
  GPIOPinConfigure(GPIO_PB0_U1RX);
  GPIOPinTypeUART(GPIO_PORTB_BASE, GPIO_PIN_0);
  
  GPIOPinConfigure(GPIO_PB1_U1TX);                     
  GPIOPinTypeUART(GPIO_PORTB_BASE, GPIO_PIN_1);
  
  //    UARTClockSourceSet(UART1_BASE, UART_CLOCK_SYSTEM);                          //set system clock as the uart clk source
  UARTConfigSetExpClk(UART1_BASE,
                      SysCtlClockGet(),
                      Baud_rate,
                      (UART_CONFIG_PAR_NONE | UART_CONFIG_STOP_ONE |
                       UART_CONFIG_WLEN_8));
  IntRegister(INT_UART1,UART1_IRQHandler);     
  
//  UARTFIFOLevelSet(UART1_BASE,
//                   UART_FIFO_TX1_8,
//                   UART_FIFO_RX1_8);

  UARTFIFODisable(UART1_BASE);  
  
  IntEnable(INT_UART1);                                                       //enable the UART interrupt
  UARTIntEnable(UART1_BASE, UART_INT_RX);                                     //only enable RX and TX interrupts
  
  //    UARTStdioConfig(1, Baud_rate, ROM_SysCtlClockGet());
}

void UART1SendString(uint8* send)
{
  while(*send != NULL)
  {	
    UARTCharPut(UART1_BASE, *send++);
  }
}

void GPIO_PINB7init(void)
{
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB); 
  
  ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
  ROM_GPIOPinTypeGPIOOutputOD(GPIO_PORTB_BASE, GPIO_PIN_7);
  
  GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_7,  0);
}

void UART1_IRQHandler()
{
  OSIntEnter(); 
  
  OS_ERR err;
  uint8 Res;
  
  UARTIntClear(UART1_BASE, UART_INT_RX);                                        
  while(!UARTCharsAvail(UART1_BASE));
  if(UARTCharsAvail(UART1_BASE))                                                //handle uart rx interrupt
  {
    
    Res = UARTCharGetNonBlocking(UART1_BASE);
    UARTCharPut(UART1_BASE, Res);                                               //Test Uart receive(echo)
    if((UART1_RX_STA&0x8000)==0)
    {
      if(UART1_RX_STA&0x4000)
      {
        if(Res!=0x0a)UART1_RX_STA=0;
        else 
        {  
          UART1_RX_STA|=0x8000; 
          OSTaskSemPost(&CameraTCB, OS_OPT_POST_NONE, &err);
        }
      }
      else
      {	
        if(Res==0x0d) UART1_RX_STA|=0x4000;
        else
        {
          UART1_RX_BUF[UART1_RX_STA&0X3FFF]=Res ;
          UART1_RX_STA++;
          if(UART1_RX_STA>(UART1_REC_LEN-1))UART1_RX_STA=0;  
        }		 
      }
    }   		 
  }
  
  //  OSTaskSemPost(&CameraTCB, OS_OPT_POST_NONE, &err);
  OSIntExit();    	
  
}
/*
**********************************************************************************************************
UCOS TASK
**********************************************************************************************************
*/ 
void uart1_task(void *p_arg)
{
  OS_ERR err;	
  CPU_SR_ALLOC();
  p_arg = p_arg;
  
  while(1)
  {
    OS_CRITICAL_ENTER();
    UART1SendString("Hello world!\r\n");
    OS_CRITICAL_EXIT();
    OSTimeDlyHMSM(0,0,1,0,OS_OPT_TIME_HMSM_STRICT,&err); //��ʱ500ms
  }
}

void uart1_int_handler_task(void *p_arg)
{
  OS_ERR err;	
  CPU_SR_ALLOC();
  p_arg = p_arg;
  
  uint8 dtbuf[50];
  uint16 rxlen = 0;
  while(1)
  {
    if(UART1_RX_STA&0X8000)
    {
      
      
      rxlen = UART1_RX_STA&0X3FFF;
      UART1_RX_BUF[rxlen] = NULL;
      OS_CRITICAL_ENTER(); 
      sprintf((char*)dtbuf,"UART1���ܵ�������Ϊ: %s\r\n", UART1_RX_BUF);        
      OS_CRITICAL_EXIT(); 
      UART1SendString(dtbuf);
      
      
      UART1_RX_STA = 0;
      
    }	
    OSTimeDlyHMSM(0,0,0,500,OS_OPT_TIME_HMSM_STRICT,&err); //��ʱ500ms
  }
}
