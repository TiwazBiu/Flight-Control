/*
************************************************************************************************************************
*                                                TI-TM4C Flight Control
*                                               SCIE/Shanghai University
*                                              
* File    : renesas_interface.c
* By      : Bicbrv
* Note    : Competition interface
*
* TERMS OF USE:
* ---------------
*           We provide ALL the source code for your convenience and to help you 
*           keep developing our flight control firmware.  
*
*           Please help us continue to provide our project with the finest software available.
*           Your dedicated work is greatly appreciated. Feel free to ameliorate any 
*           part of our code without any restriction to pursue maximum performance.
*
************************************************************************************************************************
*/

#include "renesas_interface.h"


uint16 UART2_RX_STA = 0;
uint8 UART2_RX_BUF[UART2_REC_LEN];
uint8 uart2_count = 0;
uint8 rcv_state=0;
int16 last_dx=0;
int16 last_dy=0;
int16 temp_dx=0;
int16 temp_dy=0;
int count_right;
int count_wrong;
struct ImgResult imgresult;
struct ImgResult imgresult_temp;
RENESAS_Struct RENESAS=
{
  0,
  0,
  0
};
/*
========================================================================================================================
*                                               UART2口初始化函数
========================================================================================================================
*/
void UART2_STInit(uint32 Baud_rate)
{
  
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
  SysCtlPeripheralEnable(SYSCTL_PERIPH_UART2);
  
  GPIOPinConfigure(GPIO_PD6_U2RX);
  GPIOPinTypeUART(GPIO_PORTD_BASE, GPIO_PIN_6);
  
  GPIOPinConfigure(GPIO_PD7_U2TX);                     
  GPIOPinTypeUART(GPIO_PORTD_BASE, GPIO_PIN_7);
  
  UARTConfigSetExpClk(UART2_BASE,
                      SysCtlClockGet(),
                      Baud_rate,
                      (UART_CONFIG_PAR_NONE | UART_CONFIG_STOP_ONE |
                       UART_CONFIG_WLEN_8));
  IntRegister(INT_UART2,UART2_IRQHandler);     

  UARTFIFODisable(UART2_BASE);  
  
  IntEnable(INT_UART2);
  UARTIntEnable(UART2_BASE, UART_INT_RX);
  
}

/*
========================================================================================================================
*                                              UART2中断服务函数
========================================================================================================================
*/


void UART2_IRQHandler()
{
  OSIntEnter(); 
    uint16 Uart6Data;    //中断处理
    UARTIntClear(UART2_BASE, UART_INT_RX);                                        
  while(!UARTCharsAvail(UART2_BASE));
  if(UARTCharsAvail(UART2_BASE))
  {
  Uart6Data = ROM_UARTCharGet(UART2_BASE);  
	 switch(rcv_state)
	{
          case 0:
                  if(Uart6Data == 0x73)  //找到帧头1
                  {   
                      rcv_state=1;
                  }
                  break;
				  
			  case 1:
				  if(Uart6Data == 0x1a)  //找到帧头2 坐标数据帧
				  {   
					  rcv_state=2;
				  }
                                  else if(Uart6Data ==0x3c)
                                  {
                                       rcv_state=40;
                                  }
				  else
				  {
					  rcv_state=0;
				  }
			  break;  
			  
              case 2:
                  imgresult_temp.move_p_r=Uart6Data;//发现标志位
                  if(!(imgresult_temp.move_p_r&0xa0))
                    imgresult_temp.move_p_r=0;
                  rcv_state=3;
                  break;
                   
              case 3:
                  imgresult_temp.deltaY= Uart6Data;//大正方形y坐标低位
                  rcv_state=4;
                  break;
              case 4:
                  imgresult_temp.deltaY= (Uart6Data<<8)+imgresult_temp.deltaY;//大正方形y坐标高位
                  rcv_state=5;
                  break;
              case 5:
                  imgresult_temp.deltaX= Uart6Data;//大正方形x坐标低位
                  rcv_state=6;
                  break;
              case 6:
                  imgresult_temp.deltaX= (Uart6Data<<8)+imgresult_temp.deltaX;//大正方形x坐标高位
                  rcv_state=7;
                  break;
              
              case 7:
                  imgresult_temp.holdposition= Uart6Data;//
                  rcv_state=8;
                  break;
                  
              case 8:
                  imgresult_temp.movethrottle= Uart6Data;//
                  rcv_state=9;
                  break;
				  
              case 9:
                  imgresult_temp.fndresult= Uart6Data;//状态位
                  rcv_state=10;
                  break;       
              case 10:
                  imgresult_temp.state= Uart6Data;
                  rcv_state=11;
                  break;
              case 11:
                if(Uart6Data==0x88){
                  imgresult.move_p_r=imgresult_temp.move_p_r;
                  imgresult.deltaY=imgresult_temp.deltaY;
                  imgresult.deltaX=imgresult_temp.deltaX;
                  imgresult.holdposition=imgresult_temp.holdposition;
                  imgresult.movethrottle=imgresult_temp.movethrottle;
                  imgresult.fndresult=imgresult_temp.fndresult;
                  imgresult.state=imgresult_temp.state;}
                rcv_state=0;
                break;
              case 40:
                imgresult_temp.state=Uart6Data;
                rcv_state=41;
                break;   
              case 41:
                if(Uart6Data==0x88)
                  imgresult.state=imgresult_temp.state;
                rcv_state=0;
                break;
              default:
                rcv_state=0;
                  break;
                    
      	
          }
     }
     OSIntExit();  
}
				  
          //			  case 0:
//                           if(Uart6Data == 0x73)  //找到帧头1
//                           {   
//                              rcv_state=1;
//                            }
//                             break;
//				  
//			  case 1:
//				  if(Uart6Data == 0x1a)  //找到帧头2 面积数据帧
//				  {   
//					  rcv_state=20;
//				  }
//                                  else if(Uart6Data ==0x3c)
//                                  {
//                                       rcv_state=40;
//                                  }
//				  else
//				  {
//					  rcv_state=0;
//				  }
//			  break;  
//			  
//              
//				  
//		      case 20:
//				  imgresult_temp.movepitch= (Uart6Data & 0xf0)>>4;//pitch
//				  imgresult_temp.moveroll= Uart6Data & 0x0f;//roll
//                                  rcv_state++;
//                                  break;
//				  
//			  case 21:
//				  if(Uart6Data  & 0x80)
//				  {
//					  temp_dy= 0xff00+Uart6Data;//dy
//                                          
//                                          if(temp_dy<-120)
//                                            imgresult_temp.deltaY=last_dy;
//                                          else{
//                                            last_dy=temp_dy;
//                                            imgresult_temp.deltaY=temp_dy;
//                                          }
//				  }
//				  else
//				  {
//					  temp_dy= Uart6Data;//dy
//                                          if(temp_dy>120)
//                                            imgresult_temp.deltaY=last_dy;
//                                          else{
//                                            last_dy=temp_dy;
//                                            imgresult_temp.deltaY=temp_dy;
//                                          }
//				  }
//				
//                                   rcv_state++;
//                                   break;
//				  
//			  case 22:
//                               temp_dx= Uart6Data;//dxL
//                               rcv_state++;
//                                break;
//                  
//              case 23:
//                  temp_dx= (Uart6Data<<8)+temp_dx;//dxH+dxL
//                  if(temp_dx>160||temp_dx<-160)
//                    imgresult_temp.deltaX=last_dx;
//                  else{
//                    last_dx=temp_dx; 
//                    imgresult_temp.deltaX=temp_dx;
//                  }             
//		  rcv_state++;
//                  break;
//				  
//              case 24:
//		  imgresult_temp.moveyaw= Uart6Data & 0x0f;//yaw
//		  imgresult_temp.holdposition=Uart6Data & 0xf0;//holdflag
//                  rcv_state++;
//                  break;
//				  
//	     case 25:
//		  imgresult_temp.movethrottle= Uart6Data;//
//                  rcv_state++;
//                  break;
//				  
//	     case 26:
//                  imgresult_temp.result=Uart6Data;//发现标志位
//                  if(!(imgresult_temp.result&0xa0))
//                    imgresult_temp.result=0;
//                  rcv_state++;
//                  break;
//                   
//             case 27:
//                   imgresult_temp.state= Uart6Data;//状态位
//                  
//                  rcv_state=28;
//                  break;
//             case 28:
//              
//                if(Uart6Data==0x88)
//                
////                imgresult.deltaX=imgresult_temp.deltaX;
////                imgresult.deltaY=imgresult_temp.deltaY;
////                imgresult.movepitch=imgresult_temp.movepitch;
////                imgresult.moveroll=imgresult_temp.moveroll;
////                imgresult.movethrottle=imgresult_temp.movethrottle;
////                imgresult.moveyaw=imgresult_temp.moveyaw;
////                imgresult.result=imgresult_temp.result;
////                imgresult.state=imgresult_temp.state;
////                imgresult.holdposition=imgresult_temp.holdposition;
////                
//               // count_right++;
////                }
////                else
////                  count_wrong++;
//                    rcv_state=0;
//                    break;
//	     case 40:
//                 if(Uart6Data==0x4d)
//                 {
//                   imgresult_temp.result=0;
// 
//                   rcv_state++;
//                 }
//                 else
//                 {      
//                 rcv_state=0;
//                 }
//                break;
//              case 41:
//                  imgresult_temp.state=Uart6Data;
//                  rcv_state++;
//                  break;
//              case 42:
//                if(Uart6Data==0x88){ 
//                imgresult.deltaX=imgresult_temp.deltaX;
//                imgresult.deltaY=imgresult_temp.deltaY;
//                imgresult.movepitch=imgresult_temp.movepitch;
//                imgresult.moveroll=imgresult_temp.moveroll;
//                imgresult.movethrottle=imgresult_temp.movethrottle;
//                imgresult.moveyaw=imgresult_temp.moveyaw;
//                imgresult.result=imgresult_temp.result;
//                imgresult.state=imgresult_temp.state;
//                imgresult.holdposition=imgresult_temp.holdposition;
//                count_right++;
//                }
//                else
//                  count_wrong++;
//                rcv_state=0;
//                break;


  
//  uint8 Res;
//  
//  UARTIntClear(UART2_BASE, UART_INT_RX);                                        
//  while(!UARTCharsAvail(UART2_BASE));
//  if(UARTCharsAvail(UART2_BASE))
//  {
//    
//    Res = UARTCharGetNonBlocking(UART2_BASE);
//    
//    
//    switch(uart2_count)
//    {
//    case 0:
//      
//      if(Res == 0x55)
//        uart2_count = 1;
//      break;
//    case 1:
//      if(Res == 0xaa){
//        
//        uart2_count = 2;}
//      else
//        uart2_count = 0;
//      break;
//    case 2:
//      UART2_RX_BUF[0] = Res;
//      uart2_count = 3;
//      break;
//    case 3:
//      UART2_RX_BUF[1] = Res;
//      uart2_count = 4;
//      break;
//    case 4:
//      if(Res == '|'){
//        
//        uart2_count = 5;}
//      else
//        uart2_count = 0;
//      break;
//    case 5:
//      UART2_RX_BUF[2] = Res;
//      uart2_count = 6;
//      break;
//    case 6:
//      UART2_RX_BUF[3] = Res;
//      uart2_count = 7;
//      break;
//    case 7:
//      if(Res=='|')
//        uart2_count=8;
//      else
//        uart2_count=0;
//    case 8:
//      UART2_RX_BUF[4] = Res;
//      uart2_count = 9;
//    case 9:
//      UART2_RX_BUF[5] = Res;
//      uart2_count = 10;
//    case 10:
//      if(Res == 0x0d)
//        uart2_count = 11;
//      else
//        uart2_count = 0;
//      break;
//    case 11:
//      if(Res == 0x0a){
//      RENESAS.FLOW_X = (int16)(UART2_RX_BUF[0]|(UART2_RX_BUF[1]<<8));
//      RENESAS.FLOW_Y = (int16)(UART2_RX_BUF[2]|(UART2_RX_BUF[3]<<8));
//      RENESAS.YAW=(int16)(UART2_RX_BUF[4]|(UART2_RX_BUF[5]<<8));
//       uart2_count = 0;}
//      break;
//    default:
//      uart2_count = 0;
//
//    }
//  
//  }
  


void UART2SendString(uint8* send)
{
  while(*send != NULL)
  {	
    UARTCharPut(UART2_BASE, *send++);
  }
}
//void UART2_IRQHandler()
//{
//  OSIntEnter(); 
//  
//  OS_ERR err;
//  uint8 Res;
//  
//  UARTIntClear(UART2_BASE, UART_INT_RX);                                        
//  while(!UARTCharsAvail(UART2_BASE));
//  if(UARTCharsAvail(UART2_BASE))                                                //handle uart rx interrupt
//  {
//    
//    Res = UARTCharGetNonBlocking(UART2_BASE);
////    UARTCharPut(UART2_BASE, Res);                                               //Test Uart receive(echo)
//    if((UART2_RX_STA&0x8000)==0)
//    {
//      if(UART2_RX_STA&0x4000)
//      {
//        if(Res!=0x0a)
//          UART2_RX_STA=0;
//        else 
//        {  
//          UART2_RX_STA|=0x8000; 
////          if(UART2_RX_BUF[0] == '$')
//          OSTaskSemPost(&RenesasTCB, OS_OPT_POST_NONE, &err);
//        }
//      }
//      else
//      {	
//        if(Res==0x0d)
//          UART2_RX_STA|=0x4000;
//        else
//        {
//          UART2_RX_BUF[UART2_RX_STA&0X3FFF]=Res ;
//          UART2_RX_STA++;
//          if(UART2_RX_STA>(UART2_REC_LEN-1))UART2_RX_STA=0;  
//        }		 
//      }
//    }   		 
//  }
//  
//  //  OSTaskSemPost(&CameraTCB, OS_OPT_POST_NONE, &err);
//  OSIntExit();    	
//  
//}



/*
========================================================================================================================
*                                               uc/OS Task
========================================================================================================================
*/
//int recive_sua_flag=0;
//
//void renesas_interface(void *p_arg)
//{
//  OS_ERR err;
////  CPU_SR_ALLOC();
//  p_arg = p_arg;  
////  CPU_TS ts;
//  
////  uint8 dtbuf[40];
//  uint16 rxlen = 0;
////  int8 seps[2] = "|";
////  int8 *token;
//  while(DEF_TRUE)
//  {
//    OSTaskSemPend(0,OS_OPT_PEND_BLOCKING,0,&err);
//    
//    if(UART2_RX_STA&0X8000 && UART2_RX_BUF[0] == '$')
//    {
//      rxlen = UART2_RX_STA&0X3FFF;
//      UART2_RX_BUF[rxlen] = NULL;
//
////      token = strtok((char*)&UART2_RX_BUF[1], seps);       
////      RENESAS.FLOW_X = atof(token);
////      token = strtok(NULL, seps);
////      RENESAS.FLOW_Y = atof(token);
////      recive_sua_flag=1;
//    }
//    UART2_RX_STA = 0;
//  }  
//}