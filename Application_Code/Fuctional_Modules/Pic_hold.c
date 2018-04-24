/*
========================================================================================================================
*                                               PIC调整函数（未使用）
========================================================================================================================
*/
#include "Pic_hold.h"

#define Pic_x_i_max 50000
#define Pic_out_max 6.5f

//#define Pic_factor 1.5
#define Pic_factor 1.3f

float Pic_x_out,Pic_y_out;

float pic_x_cm,pic_y_cm;
static float last_pic_x_cm=0,last_pic_y_cm=0;	
static float count_hold=0;
//static float last_deltaX=0,last_deltaY=0;
float pic_x_i=0,pic_y_i=0;
static float PID_PIC_XOUT=0,PID_PIC_YOUT=0;
static volatile float delta_picx,delta_picy;
float deltaX=0 ,deltaY=0;
float deltaX_out=0 ,deltaY_out=0,flag_hold=1;
float Pic_cotrol_xout=0,Pic_cotrol_yout=0;
float Pic_cotrol_yaw=0;
int control_flag=0;

void PIC_Control()
{ 
  if(flag_hold==1)
  {if(imgresult.holdposition==1)
    count_hold++;
   if(count_hold==5)
    flag_hold=0;
  }
  deltaX=(float)imgresult.deltaX;
  deltaY=(float)imgresult.deltaY+60*flag_hold*imgresult.state;//寻找到并且舵机没有落下时给一个向前的偏量。
  
//  if(deltaX-last_deltaX>50||deltaX-last_deltaX<-50)
//    deltaX=last_deltaX;
//  else
//    last_deltaX=deltaX;
//  if(deltaY-last_deltaY>50||deltaY-last_deltaY<-50)
//    deltaY=last_deltaY;
//  else
//    last_deltaY=deltaY; 
//  deltaX_out=deltaX;
//  deltaY_out=deltaY;
  if(deltaX<=-140)
  {
    deltaX = -140;
  }
  else if(deltaX >= 140)
  {
    deltaX =  140;
  }
  
   if(deltaY<=-120)
  {
    deltaY = -120;
  }
  else if(deltaY>= 120)
  {
    deltaY=  120;
  }
//Pic_cotrol_yaw=
deltaX_out=deltaX;
deltaY_out=deltaY;
 
  pic_x_cm = deltaX;                                                          //SumX/Y as px4_data_fix() output
  pic_y_cm = deltaY;
  pic_x_i += pic_x_cm;
  pic_y_i += pic_y_cm;
  
  if(pic_x_i > Pic_x_i_max)                                                     //Pic_x_i_max 50000 is defined
    pic_x_i = Pic_x_i_max;
  if(pic_x_i < -Pic_x_i_max)
    pic_x_i = -Pic_x_i_max;
  if(pic_y_i > Pic_x_i_max)
    pic_y_i = Pic_x_i_max;
  if(pic_y_i < -Pic_x_i_max)
    pic_y_i = -Pic_x_i_max;
  
   if(ks103_distance<=600)
  {
    pic_y_cm=0;
    pic_y_i=0;
    pic_x_cm=0;
    pic_x_i=0;
  }
  
  delta_picx=pic_x_cm - last_pic_x_cm;
  delta_picy=pic_y_cm - last_pic_y_cm;
  
/* PID_PIC_XOUT  = 0.045*( pic_x_cm + 0.00714 * pic_x_i + 35* (pic_x_cm - last_pic_x_cm));              */
/* PID_PIC_YOUT  = 0.045*( pic_y_cm + 0.0035 * pic_y_i + 37.6* (pic_y_cm - last_pic_y_cm));              */

  
//  if(pic_x_cm<=90 && pic_y_cm<=90){
//    PID_PIC_XOUT  = UART_PIDadjust.FLOW_XP 
//      * ( pic_x_cm + UART_PIDadjust.FLOW_XI * pic_x_i + UART_PIDadjust.FLOW_XD * (pic_x_cm - last_pic_x_cm));
//    PID_PIC_YOUT  = UART_PIDadjust.FLOW_YP 
//      * ( pic_y_cm + UART_PIDadjust.FLOW_YI * pic_y_i + UART_PIDadjust.FLOW_YD * (pic_y_cm - last_pic_y_cm));
//  }
//  if(pic_x_cm>90 ){
//    PID_PIC_XOUT  = 0.055 * ( pic_x_cm + UART_PIDadjust.FLOW_XD * (pic_x_cm - last_pic_x_cm));
//    
//  }
//  if(pic_y_cm>90 ){
//
//    PID_PIC_YOUT  = 0.055 *( pic_y_cm +  UART_PIDadjust.FLOW_YD * (pic_y_cm - last_pic_y_cm));
//  }
  
  
   PID_PIC_XOUT  = UART_PIDadjust.FLOW_XP 
      * ( pic_x_cm + UART_PIDadjust.FLOW_XI * pic_x_i + UART_PIDadjust.FLOW_XD * (pic_x_cm - last_pic_x_cm));
   PID_PIC_YOUT  = UART_PIDadjust.FLOW_YP 
      * ( pic_y_cm + UART_PIDadjust.FLOW_YI * pic_y_i + UART_PIDadjust.FLOW_YD * (pic_y_cm - last_pic_y_cm));
  
 
  Pic_x_out = PID_PIC_XOUT ;	
  Pic_y_out = PID_PIC_YOUT ;
  
  if(Pic_x_out > Pic_out_max)
    Pic_x_out = Pic_out_max;
  if(Pic_x_out < -Pic_out_max)
    Pic_x_out = -Pic_out_max;
  if(Pic_y_out > Pic_out_max)
    Pic_y_out = Pic_out_max;
  if(Pic_y_out < -Pic_out_max)
    Pic_y_out = -Pic_out_max;
 
    last_pic_x_cm = pic_x_cm;
    last_pic_y_cm = pic_y_cm;
  
  
}


void fix_cotrol()
{
  if(pic_x_cm>50 && ks103_distance > 600 ){
    
    Pic_cotrol_yout=PID_PIC_YOUT + Pic_factor*(pic_y_cm/pic_x_cm);
    Pic_cotrol_xout=PID_PIC_XOUT+Pic_factor;
    control_flag=5;
  }
  else if(pic_x_cm<-50  && ks103_distance > 600 ){
    if(COMPETITON_FLIGHT_MODE==COM_TASK_2 )
    {
      Pic_cotrol_yout=PID_PIC_YOUT - 9*(pic_y_cm/pic_x_cm);
      Pic_cotrol_xout=PID_PIC_XOUT-9;
      control_flag=5;
    }
    if(COMPETITON_FLIGHT_MODE==COM_TASK_1 )
    {
      Pic_cotrol_yout=PID_PIC_YOUT - Pic_factor*(pic_y_cm/pic_x_cm);
      Pic_cotrol_xout=PID_PIC_XOUT-Pic_factor;
      control_flag=5;
    }
  }
  
  if(pic_y_cm>50  && ks103_distance > 600 ){
    
    Pic_cotrol_xout=PID_PIC_XOUT + Pic_factor*(pic_x_cm/pic_y_cm);
    Pic_cotrol_yout=PID_PIC_YOUT+Pic_factor;
    control_flag=5;
  }
  else if(pic_y_cm<-50  && ks103_distance > 600 ){
    
    Pic_cotrol_xout=PID_PIC_XOUT - Pic_factor*(pic_x_cm/pic_y_cm);
    Pic_cotrol_yout=PID_PIC_YOUT-Pic_factor;
    control_flag=5;
  }
}