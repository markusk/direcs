#include "main.h"


/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
  /*!< At this stage the microcontroller clock setting is already configured, 
       this is done through SystemInit() function which is called from startup
       file (startup_stm32f4xx.s) before to branch to application main.
       To reconfigure the default setting of SystemInit() function, refer to
        system_stm32f4xx.c file
     */


  // call my new USART init
  usartInit();


  /* GPIOD Periph clock enable */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

  /* Configure PD12, PD13, PD14 and PD15 in output pushpull mode */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13| GPIO_Pin_14| GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOD, &GPIO_InitStructure);

  while (1)
  {
    uint16_t buchstabe = 64;


    // blocking read on serial port USART2
    while (USART_GetFlagStatus(USART2, USART_FLAG_RXNE) == RESET);
    buchstabe = USART_ReceiveData(USART2);

    if (buchstabe==64)
    {
    // blocking send on USART2
    while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);
    USART_SendData(USART2, 'A');
}


    /* PD12 to be toggled */
    GPIO_SetBits(GPIOD, GPIO_Pin_12);
    
    /* Insert delay */
    Delay(0x3FFFFF);
    
    /* PD13 to be toggled */
    GPIO_SetBits(GPIOD, GPIO_Pin_13);
    
    /* Insert delay */
    Delay(0x3FFFFF);
  
    /* PD14 to be toggled */
    GPIO_SetBits(GPIOD, GPIO_Pin_14);
    
    /* Insert delay */
    Delay(0x3FFFFF);
    
    /* PD15 to be toggled */
    GPIO_SetBits(GPIOD, GPIO_Pin_15);
    
    /* Insert delay */
    Delay(0x7FFFFF);
    
    GPIO_ResetBits(GPIOD, GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15);
    
    /* Insert delay */
    Delay(0xFFFFFF);
  }
}

/**
  * @brief  Delay Function.
  * @param  nCount:specifies the Delay time length.
  * @retval None
  */
void Delay(__IO uint32_t nCount)
{
  while(nCount--)
  {
  }
}


/**
*/
void usartInit(void)
{
  USART_InitTypeDef USART_InitStructure;

   USART_InitStructure.USART_BaudRate = 38400;
   USART_InitStructure.USART_WordLength = USART_WordLength_8b;
   USART_InitStructure.USART_StopBits = USART_StopBits_1;
   USART_InitStructure.USART_Parity = USART_Parity_No;
   USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
   USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

   //   GPIO_InitTypeDef GPIO_InitStructure;

   //configure clock for USART
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
    
    //configure clock for GPIO, Port A
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

    //configure AF, Port A
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_USART2);

    //configure ports, &GPIO_InitStructure);
    // TX
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // RX
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    USART_Init(USART2, &USART_InitStructure);

     /* Enable USART2 */
     USART_Cmd(USART2, ENABLE);
}


#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif
