
#include "rtc.h"
__IO uint32_t AsynchPrediv = 0, SynchPrediv = 0;
#define BKP_VALUE    0x32F0 


unsigned char time_buff[50];
/**
  * @brief  Configure the RTC peripheral by selecting the clock source.
  * @param  None
  * @retval None
  */
void RTC_Config(void)
{
  /* Enable the PWR clock */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);

  /* Allow access to RTC */
  PWR_BackupAccessCmd(ENABLE);
    
#if defined (RTC_CLOCK_SOURCE_LSI)  /* LSI used as RTC source clock*/
/* The RTC Clock may varies due to LSI frequency dispersion. */   
  /* Enable the LSI OSC */ 
  RCC_LSICmd(ENABLE);

  /* Wait till LSI is ready */  
  while(RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET)
  {
  }

  /* Select the RTC Clock Source */
  RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);
  
  SynchPrediv = 0x18F;
  AsynchPrediv = 0x63;

#elif defined (RTC_CLOCK_SOURCE_LSE) /* LSE used as RTC source clock */
  /* Enable the LSE OSC */
  RCC_LSEConfig(RCC_LSE_ON);

  /* Wait till LSE is ready */  
  while(RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)
  {
  }

  /* Select the RTC Clock Source */
  RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
  
  SynchPrediv = 0xFF;
  AsynchPrediv = 0x7F;

#else
  #error Please select the RTC Clock source inside the main.c file
#endif /* RTC_CLOCK_SOURCE_LSI */
  
  /* Enable the RTC Clock */
  RCC_RTCCLKCmd(ENABLE);

  /* Wait for RTC APB registers synchronisation */
  RTC_WaitForSynchro();
}

/**
  * @brief  Returns the time entered by user, using Hyperterminal.
  * @param  None
  * @retval None
  */
void RTC_TimeRegulate(void)
{
  RTC_TimeTypeDef RTC_TimeStructure;
  RTC_AlarmTypeDef  RTC_AlarmStructure;
  uint32_t tmp_hh = 0x12, tmp_mm = 0x12, tmp_ss = 0x12;

 // printf("\n\r==============Time Settings=====================================\n\r");
  RTC_TimeStructure.RTC_H12     = RTC_H12_AM;
//  printf("  Please Set Hours:\n\r");
//  while (tmp_hh == 0xFF)
//  {
//    tmp_hh = USART_Scanf(23);
    RTC_TimeStructure.RTC_Hours = tmp_hh;
//  }
//  printf("  %0.2d\n\r", tmp_hh);
//  
//  printf("  Please Set Minutes:\n\r");
//  while (tmp_mm == 0xFF)
//  {
//    tmp_mm = USART_Scanf(59);
    RTC_TimeStructure.RTC_Minutes = tmp_mm;
//  }
//  printf("  %0.2d\n\r", tmp_mm);
//  
//  printf("  Please Set Seconds:\n\r");
//  while (tmp_ss == 0xFF)
//  {
//    tmp_ss = USART_Scanf(59);
    RTC_TimeStructure.RTC_Seconds = tmp_ss;
//  }
//  printf("  %0.2d\n\r", tmp_ss);

  /* Configure the RTC time register */
  if(RTC_SetTime(RTC_Format_BIN, &RTC_TimeStructure) == ERROR)
  {
    printf("\n\r>> !! RTC Set Time failed. !! <<\n\r");
  } 
  else
  {
    printf("\n\r>> !! RTC Set Time success. !! <<\n\r");
    RTC_TimeShow();
    /* Indicator for the RTC configuration */
    RTC_WriteBackupRegister(RTC_BKP_DR0, BKP_VALUE);
  }

  tmp_hh = 0x12;
  tmp_mm = 0x12;
  tmp_ss = 0x12;

  /* Disable the Alarm A */
  RTC_AlarmCmd(RTC_Alarm_A, DISABLE);

  printf("\n\r==============Alarm A Settings=====================================\n\r");
  RTC_AlarmStructure.RTC_AlarmTime.RTC_H12 = RTC_H12_AM;
//  printf("  Please Set Alarm Hours:\n\r");
//  while (tmp_hh == 0xFF)
//  {
//    tmp_hh = USART_Scanf(23);
    RTC_AlarmStructure.RTC_AlarmTime.RTC_Hours = tmp_hh;
//  }
//  printf("  %0.2d\n\r", tmp_hh);
//  
//  printf("  Please Set Alarm Minutes:\n\r");
//  while (tmp_mm == 0xFF)
//  {
//    tmp_mm = USART_Scanf(59);
    RTC_AlarmStructure.RTC_AlarmTime.RTC_Minutes = tmp_mm;
//  }
//  printf("  %0.2d\n\r", tmp_mm);
//  
//  printf("  Please Set Alarm Seconds:\n\r");
//  while (tmp_ss == 0xFF)
//  {
//    tmp_ss = USART_Scanf(59);
    RTC_AlarmStructure.RTC_AlarmTime.RTC_Seconds = tmp_ss;
//  }
//  printf("  %0.2d", tmp_ss);

  /* Set the Alarm A */
  RTC_AlarmStructure.RTC_AlarmDateWeekDaySel = RTC_AlarmDateWeekDaySel_Date;
  RTC_AlarmStructure.RTC_AlarmDateWeekDay = RTC_Weekday_Monday;    
  RTC_AlarmStructure.RTC_AlarmMask = RTC_AlarmMask_DateWeekDay;

  /* Configure the RTC Alarm A register */
  RTC_SetAlarm(RTC_Format_BIN, RTC_Alarm_A, &RTC_AlarmStructure);
  printf("\n\r>> !! RTC Set Alarm success. !! <<\n\r");
  RTC_AlarmShow();

  /* Enable the RTC Alarm A Interrupt */
  RTC_ITConfig(RTC_IT_ALRA, ENABLE);
   
  /* Enable the alarm  A */
  RTC_AlarmCmd(RTC_Alarm_A, ENABLE);
}

/**
  * @brief  Display the current time on the Hyperterminal.
  * @param  None
  * @retval None
  */
void RTC_TimeShow(void)
{
	RTC_TimeTypeDef RTC_TimeStructure;
	/* Get the current Time */
	RTC_GetTime(RTC_Format_BIN, &RTC_TimeStructure);
	printf("\n\r  The current time is :  %0.2d:%0.2d:%0.2d \n\r", RTC_TimeStructure.RTC_Hours, RTC_TimeStructure.RTC_Minutes, RTC_TimeStructure.RTC_Seconds);
	sprintf(time_buff,"%0.2d:%0.2d:%0.2d",RTC_TimeStructure.RTC_Hours, RTC_TimeStructure.RTC_Minutes, RTC_TimeStructure.RTC_Seconds);
}

/**
  * @brief  Display the current time on the Hyperterminal.
  * @param  None
  * @retval None
  */
void RTC_AlarmShow(void)
{
	RTC_AlarmTypeDef  RTC_AlarmStructure;
	/* Get the current Alarm */
	RTC_GetAlarm(RTC_Format_BIN, RTC_Alarm_A, &RTC_AlarmStructure);
	printf("\n\r  The current alarm is :  %0.2d:%0.2d:%0.2d \n\r", RTC_AlarmStructure.RTC_AlarmTime.RTC_Hours, RTC_AlarmStructure.RTC_AlarmTime.RTC_Minutes, RTC_AlarmStructure.RTC_AlarmTime.RTC_Seconds);
}

void RTC_init(void)
{
	NVIC_InitTypeDef  NVIC_InitStructure;
	EXTI_InitTypeDef  EXTI_InitStructure;
	RTC_InitTypeDef RTC_InitStructure;
	if (RTC_ReadBackupRegister(RTC_BKP_DR0) != BKP_VALUE)
	  {  
		/* RTC configuration  */
		RTC_Config();

		/* Configure the RTC data register and RTC prescaler */
		RTC_InitStructure.RTC_AsynchPrediv = AsynchPrediv;
		RTC_InitStructure.RTC_SynchPrediv = SynchPrediv;
		RTC_InitStructure.RTC_HourFormat = RTC_HourFormat_24;
	   
		/* Check on RTC init */
		if (RTC_Init(&RTC_InitStructure) == ERROR)
		{
		  printf("\n\r        /!\\***** RTC Prescaler Config failed ********/!\\ \n\r");
		}

		/* Configure the time register */
		RTC_TimeRegulate(); 
	  }
	  else
	  {
		/* Check if the Power On Reset flag is set */
		if (RCC_GetFlagStatus(RCC_FLAG_PORRST) != RESET)
		{
		  printf("\r\n Power On Reset occurred....\n\r");
		}
		/* Check if the Pin Reset flag is set */
		else if (RCC_GetFlagStatus(RCC_FLAG_PINRST) != RESET)
		{
		  printf("\r\n External Reset occurred....\n\r");
		}

		printf("\n\r No need to configure RTC....\n\r");
		
		/* Enable the PWR clock */
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);

		/* Allow access to RTC */
		PWR_BackupAccessCmd(ENABLE);
	 #ifdef RTC_CLOCK_SOURCE_LSI
		/* Enable the LSI OSC */ 
		RCC_LSICmd(ENABLE);
	 #endif /* RTC_CLOCK_SOURCE_LSI */

		/* Wait for RTC APB registers synchronisation */
		RTC_WaitForSynchro();

		/* Clear the RTC Alarm Flag */
		RTC_ClearFlag(RTC_FLAG_ALRAF);

		/* Clear the EXTI Line 17 Pending bit (Connected internally to RTC Alarm) */
		EXTI_ClearITPendingBit(EXTI_Line17);

		/* Display the RTC Time and Alarm */
		RTC_TimeShow();
	  //  RTC_AlarmShow();
	  }
//   
//  /* Configure the external interrupt "KEY", "SEL" and "UP" buttons */
//  STM_EVAL_PBInit(BUTTON_TAMPER, BUTTON_MODE_EXTI); 
//  STM_EVAL_PBInit(BUTTON_SEL, BUTTON_MODE_EXTI);
//  STM_EVAL_PBInit(BUTTON_UP, BUTTON_MODE_EXTI);

//  /* Configure LEDs */
//  STM_EVAL_LEDInit(LED1);
//  STM_EVAL_LEDInit(LED2);

//  /* Turn LED2 ON */
//  STM_EVAL_LEDOn(LED2);

  /* RTC Alarm A Interrupt Configuration */
  /* EXTI configuration *********************************************************/
//  EXTI_ClearITPendingBit(EXTI_Line17);
//  EXTI_InitStructure.EXTI_Line = EXTI_Line17;
//  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
//  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
//  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
//  EXTI_Init(&EXTI_InitStructure);
//  
//  /* Enable the RTC Alarm Interrupt */
//  NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;
//  NVIC_InitStructure.NVIC_IRQChannelPriority = 0;
//  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//  NVIC_Init(&NVIC_InitStructure);

}








