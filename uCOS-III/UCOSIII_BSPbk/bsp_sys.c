/*
*********************************************************************************************************
*                                            EXAMPLE CODE
*
*               This file is provided as an example on how to use Micrium products.
*
*               Please feel free to use any application code labeled as 'EXAMPLE CODE' in
*               your application products.  Example code may be used as is, in whole or in
*               part, or may be used as a reference only. This file can be modified as
*               required to meet the end-product requirements.
*
*               Please help us continue to provide the Embedded community with the finest
*               software available.  Your honesty is greatly appreciated.
*
*               You can find our product's user manual, API reference, release notes and
*               more information at https://doc.micrium.com.
*               You can contact us at www.micrium.com.
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                         BOARD SUPPORT PACKAGE
*
*                                      Texas Instruments TM4C129x
*                                                on the
*
*                                             DK-TM4C129X
*                                           Development Kit
*
* Filename      : bsp_sys.c
* Version       : V1.00
* Programmer(s) : FF
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                             INCLUDES
*********************************************************************************************************
*/

#include  <bsp_cfg.h>
#include  <lib_def.h>
#include  <bsp_sys.h>

#include <stdbool.h>
#include <stdint.h>
#include "driverlib/sysctl.h"

/*$PAGE*/
/*
*********************************************************************************************************
*                                         BSP SYSTEM INITIALIZATION
*
* Description: This function should be called early in the BSP initialization process.
*
* Argument(s): none.
*
* Return(s)  : none.
*
* Caller(s)  : Application.
*
* Note(s)    : 1) Ensure the main oscillator is enable because this is required by the PHY. The system
*                 must have a 25MHz crystal attached to the OSC pins. The SYSCTL_MOS_HIGHFREQ parameter
*                 is used when the crystal frequency is 10MHz or higher.
*
*              2) Depending on the CPU frequency, the application must program the Main Flash and EEPROM
*                 memory timing paremeters according to the following table:
*
*               +-----------------------+--------------------------+-------------+-----------+---------+
*               | CPU Freq. Range(F) in | Time Period Range (t) in | FBCHT/EBCHT | FBCE/EBCE | FWS/EWS |
*               |         MHz           |          ns              |             |           |         |
*               +-----------------------+--------------------------+-------------+-----------+---------+
*               |         16            |          62.5            |     0x0     |     1     |  0x0    |
*               |    16 < f <=  40      |    62.5  > f >= 25       |     0x2     |     0     |  0x1    |
*               |    40 < f <=  60      |    25    > f >= 16.67    |     0x3     |     0     |  0x2    |
*               |    60 < f <=  80      |    16.67 > f >= 12.5     |     0x4     |     0     |  0x3    |
*               |    80 < f <= 100      |    12.5  > f >= 10       |     0x5     |     0     |  0x4    |
*               |   100 < f <= 120      |    10    > f >=  8.33    |     0x6     |     0     |  0x5    |
*               +-----------------------+--------------------------+-------------+-----------+---------+
*********************************************************************************************************
*/

void  BSP_SysInit (void)
{
//    /* Enable xtal crystal oscillator                       */
//    DEF_BIT_CLR(BSP_SYS_REG_MOSCCTL, (BSP_MOSCCTL_NOXTAL));
//
//		// PLL configuration. RCC PLLFREQ0 PLLFREQ1 registers should be edited.
//		// RCC register configuration follows
//		// select main clock oscillator rcc register.
//                                                              // this bit is to be configured before configuring pwrdn bit
//                DEF_BIT_SET(BSP_SYS_REG_RCC, BSP_RCC_BYPASS); //The system clock is derived from the OSC source and divided by the divisor specified by SYSDIV.
//		DEF_BIT_CLR(BSP_SYS_REG_RCC,(BSP_RCC_PWRDN)); //The PLL is operating normally.
//		
//		DEF_BIT_SET(BSP_SYS_REG_RCC2,BSP_RCC2_DIV400); // to use 400MHz
//		
//		DEF_BIT_SET(BSP_SYS_REG_RCC,BSP_RCC_SYSDIV_SET); 
//		DEF_BIT_CLR(BSP_SYS_REG_RCC,BSP_RCC_SYSDIV_CLR); //divided by 5
//		
//		DEF_BIT_SET(BSP_SYS_REG_RCC,BSP_RCC_USESYSDIV);
//		
//		//DEF_BIT_SET(BSP_SYS_REG_RCC,BSP_RCC_XTAL_SET); // 16 MHz external oscillator
//		DEF_BIT_SET(BSP_SYS_REG_RCC,BSP_RCC_XTAL_SET_8Mhz);// 8 MHz external oscillator
//                //DEF_BIT_CLR(BSP_SYS_REG_RCC,BSP_RCC_XTAL_CLR);
//    /* MDIV = MINT + (MFRAC / 1024)                         */
//    /* MDIV = 50   + (0 / 1024) = 50                        */
//    BSP_SYS_REG_PLLFREQ0 = ((BSP_CFG_PLL_MFRAC_USING8M << 10u)  |
//                            (BSP_CFG_PLL_MINT_USING8M  <<  0u));
//		/* Fvco freq = (fmosc * MDIV) / ((Q + 1) * (N + 1))     */
//    /*           = (16MHz * 96) / (0 + 1) * (1 + 1))        */
//		/*           = 400MHz                                   */
//    BSP_SYS_REG_PLLFREQ1 = ((BSP_CFG_PLL_Q_USING8M << 8u)  |
//                            (BSP_CFG_PLL_N_USING8M << 0u));
//
//    
//		// enable the main oscillator
//		DEF_BIT_CLR(BSP_SYS_REG_RCC,BSP_RCC_MOSCDIS); 
//		
//		// selecting the main oscillator clock
//		DEF_BIT_CLR(BSP_SYS_REG_RCC,BSP_RCC_OSCSRC_MOSC); 
//
//		/* Wait for PLL to power and lock.                      */
//    while (DEF_BIT_IS_CLR(BSP_SYS_REG_PLLSTAT, BSP_PLLSTAT_LOCK) == DEF_YES) {
//        ;
//    }
      SysCtlClockSet(SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN |
                      SYSCTL_XTAL_8MHZ);
}


/*
*********************************************************************************************************
*                                         SYSTEM CLOCK FREQUENCY
*
* Description: This function is used to retrieve system or CPU clock frequency.
*
* Arguments  : None
*
* Return     : System clock frequency in cycles.
*
* Caller(s)  : Application.
*
* Note(s)    : None
*********************************************************************************************************
*/

CPU_INT32U  BSP_SysClkFreqGet (void) //SAR: modified to use TI library call.
{
	
    return SysCtlClockGet(); 
	
}
