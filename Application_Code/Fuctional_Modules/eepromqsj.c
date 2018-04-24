#include "eepromqsj.h"

uint32_t eeprom_readdate[9];
void eeprom_read(int select)                                                    //::note::eeprom_read(1) read from the address 0 of the eeprom; eeprom_read(2) read from address sizeof(pui32Read)+1
{                                                                               //seems only two eeprom address can be visited in this source file
  uint32_t pui32Read[9];
  

  //EEPROMMassErase();
  /*uint32_t pui32Data_init[3];
  pui32Data_init[0]=2000;
  pui32Data_init[1]=17000;
  pui32Data_init[2]=0;
  EEPROMProgram(pui32Data_init, 0x0, sizeof(pui32Data_init));*/
  if(select==1)
  EEPROMRead(pui32Read, 0x0, sizeof(pui32Read));
  else if(select==2)
  EEPROMRead(pui32Read, sizeof(pui32Read)+1, sizeof(pui32Read));
  
  eeprom_readdate[0]=pui32Read[0];
  eeprom_readdate[1]=pui32Read[1];
  eeprom_readdate[2]=pui32Read[2];
  eeprom_readdate[3]=pui32Read[3];
  eeprom_readdate[4]=pui32Read[4];
  eeprom_readdate[5]=pui32Read[5];
  eeprom_readdate[6]=pui32Read[6];
  eeprom_readdate[7]=pui32Read[7];
  eeprom_readdate[8]=pui32Read[8];
}
void eeprom_write(int select)                                                   //::note:: similar to the situation of eeprom_read, only two address can be visit
{ 
  uint32_t pui32Data[9];
  pui32Data[0] = Nrf_Buf_In.Data_int[0];
  pui32Data[1] = Nrf_Buf_In.Data_int[1];
  pui32Data[2] = Nrf_Buf_In.Data_int[2];
  pui32Data[3] = Nrf_Buf_In.Data_int[3];
  pui32Data[4] = Nrf_Buf_In.Data_int[4];
  pui32Data[5] = Nrf_Buf_In.Data_int[5];
  pui32Data[6] = Nrf_Buf_In.Data_int[6];
  pui32Data[7] = Nrf_Buf_In.Data_int[7];
  pui32Data[8] = Nrf_Buf_In.Data_int[8];
 // SysCtlPeripheralEnable(SYSCTL_PERIPH_EEPROM0);
 // EEPROMInit();
  if(select==1)
  EEPROMProgram(pui32Data, 0x0, sizeof(pui32Data));
  else if(select==2)
  EEPROMProgram(pui32Data, sizeof(pui32Data)+1, sizeof(pui32Data));
  
}  
/*
========================================================================================================================
*                                               初始化函数从eeprom中读取飞行参数
========================================================================================================================
*/
void eeprom_PID_INT(void)
{
  eeprom_read(1);
  UART_PIDadjust.FLOW_XP=eeprom_readdate[0];//X_P
  UART_PIDadjust.FLOW_XI=eeprom_readdate[1];//X_I
  UART_PIDadjust.FLOW_XD=eeprom_readdate[2];//X_D
  UART_PIDadjust.FLOW_YP=eeprom_readdate[3];//Y_P
  UART_PIDadjust.FLOW_YI=eeprom_readdate[4];//Y_I
  UART_PIDadjust.FLOW_YD=eeprom_readdate[5];//Y_D
  UART_PIDadjust.Height_P=eeprom_readdate[6];//H_P
  UART_PIDadjust.Height_D=eeprom_readdate[7];//H_D
  eeprom_read(2);
  UART_PIDadjust.fix_pitch=eeprom_readdate[0];//pitch初始偏移量
  UART_PIDadjust.fix_roll=eeprom_readdate[1];//roll初始化偏移量
  UART1SendString("读取结束\r\n");
}
/*
========================================================================================================================
*                                               将飞行参数存入eeprom
========================================================================================================================
*/
void eeprom_PID_WRITE(void)
{
  uint32_t pui32Data[9];
  pui32Data[0]=(uint32_t)UART_PIDadjust.FLOW_XP;//X_P
  pui32Data[1]=(uint32_t)UART_PIDadjust.FLOW_XI;//X_I
  pui32Data[2]=(uint32_t)UART_PIDadjust.FLOW_XD;//X_D
  pui32Data[3]=(uint32_t)UART_PIDadjust.FLOW_YP;//Y_P
  pui32Data[4]=(uint32_t)UART_PIDadjust.FLOW_YI;//Y_I
  pui32Data[5]=(uint32_t)UART_PIDadjust.FLOW_YD;//Y_D
  pui32Data[6]=(uint32_t)UART_PIDadjust.Height_P;//H_P
  pui32Data[7]=(uint32_t)UART_PIDadjust.FLOW_YD;//H_D
  EEPROMProgram(pui32Data, 0x0, sizeof(pui32Data));
  pui32Data[0]=(uint32_t)UART_PIDadjust.fix_pitch;//pitch初始偏移量
  pui32Data[1]=(uint32_t)UART_PIDadjust.fix_roll;//roll初始化偏移量
  EEPROMProgram(pui32Data, sizeof(pui32Data)+1, sizeof(pui32Data));
  UART1SendString("写入结束\r\n");
}
void read_fix(void)
{
  eeprom_read(1);
  UART_PIDadjust.fix_pitch=eeprom_readdate[0];//X_P
  UART_PIDadjust.fix_roll=eeprom_readdate[1];//X_I
}
void write_fix(void)
{
  uint32_t fix[2];
  fix[0]=(uint32_t)UART_PIDadjust.fix_pitch;//pitch初始偏移量
  fix[1]=(uint32_t)UART_PIDadjust.fix_roll;//roll初始化偏移量
  EEPROMProgram(fix, 0x0, sizeof(fix));
}