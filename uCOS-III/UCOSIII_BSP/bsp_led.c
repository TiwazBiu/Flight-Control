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
* Filename      : bsp_led.c
* Version       : V1.00
* Programmer(s) : FF
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                            INCLUDES
*********************************************************************************************************
*/

#include  <cpu.h>
#include  <bsp.h>
#include  <bsp_led.h>
#include  <bsp_sys.h>
#include  <lib_def.h>


/*
*********************************************************************************************************
*                                       LED INITIALIZATION
*
* Description: This function is used to initialize the LEDs on the board.
*
* Argument(s): None.
*
* Return(s)  : None.
*
* Caller(s)  : Application.
*
* Note(s)    : None.
*********************************************************************************************************
*/

void  BSP_LED_Init (void)
{
                                                                /* Enable GPIO clock                                    */
		DEF_BIT_SET(BSP_SYS_REG_RCGCGPIO, (BSP_RCGCGPIO_PORT_A )); 	//SAR Edit

                                                                /* Set PQ7(GREEN) and PQ4(BLUE) pins as output          */
    BSP_GPIO_REG_DIR(BSP_GPIO_PORTA) = (BSP_LED_DS1 |
																				BSP_LED_DS2 );		//SAR Addition

                                                                /* En PQ7(GREEN) and PQ4(BLUE) pins digital functions   */
    BSP_GPIO_REG_DEN(BSP_GPIO_PORTA) = (BSP_LED_DS1 |
																				BSP_LED_DS2 );		//SAR Addition

    BSP_LED_Off(0);                                             /* Turn OFF LEDs.                                       */
		BSP_LED_Off(1);

}


/*
*********************************************************************************************************
*                                               LED ON
*
* Description: This function is used to control any or all the LEDs on the board.
*
* Arguments  : led    is the color of the tricolor User LED to control
*                     0    Turn on Green color
*                     1    Turn on Blue color
* Return(s)  : None.
*
* Caller(s)  : Application.
*
* Note(s)    : None.
*********************************************************************************************************
*/

void  BSP_LED_On (CPU_INT08U  led)
{
		switch (led) {
        case 0:
             DEF_BIT_SET(BSP_GPIO_REG_DATA(BSP_GPIO_PORTA), //SAR Edit
                         BSP_LED_DS1);
             break;

        case 1:
             DEF_BIT_SET(BSP_GPIO_REG_DATA(BSP_GPIO_PORTA), //SAR Edit
                         BSP_LED_DS2);
				
						break;

       default:
             break;
    }
}


/*
*********************************************************************************************************
*                                               LED OFF
*
* Description: This function is used to control any or all the LEDs on the board.
*
* Arguments  : led    is the color of the tricolor User LED to control
*                     0    Turn off Green color
*                     1    Turn off Blue color
* Return(s)  : None.
*
* Caller(s)  : Application.
*
* Note(s)    : None.
*********************************************************************************************************
*/

void  BSP_LED_Off (CPU_INT08U  led)
{
    switch (led) {
        case 0:
             DEF_BIT_CLR(BSP_GPIO_REG_DATA(BSP_GPIO_PORTA), //SAR Edit
                         BSP_LED_DS1);
             break;


        case 1:
             DEF_BIT_CLR(BSP_GPIO_REG_DATA(BSP_GPIO_PORTA), //SAR Edit
                         BSP_LED_DS2);
						 break;

       default:
             break;		
    }
}


/*
*********************************************************************************************************
*                                             LED TOGGLE
*
* Description: This function is used to toggle the state of any or all the LEDs on the board.
*
* Arguments  : led    is the color of the tricolor User LED to control
*                     0    Toggle Green color
*                     1    Toggle on Blue color
* Return(s)  : None.
*
* Caller(s)  : Application.
*
* Note(s)    : None.
*********************************************************************************************************
*/

void  BSP_LED_Toggle (CPU_INT08U  led)
{
    CPU_INT32U  port_val;
    CPU_INT32U  pins_clr;
    CPU_INT32U  pins_set;


		port_val = BSP_GPIO_REG_DATA(BSP_GPIO_PORTA);  //SAR Edit

    switch (led) {
        case 0:
             pins_clr = ( port_val) & BSP_LED_DS1;
             pins_set = (~port_val) & BSP_LED_DS1;
             break;


        case 1:
             pins_clr = ( port_val) & BSP_LED_DS2;
             pins_set = (~port_val) & BSP_LED_DS2;
             break;

        default:
             break;
    }

    DEF_BIT_CLR(BSP_GPIO_REG_DATA(BSP_GPIO_PORTA), pins_clr); //SAR Edit
    DEF_BIT_SET(BSP_GPIO_REG_DATA(BSP_GPIO_PORTA), pins_set); //SAR Edit
}
