/**
  ******************************************************************************
  * @file    stm32l1xx_lp_modes.h
  * @author  MCD Application Team
  * @version V1.2.0
  * @date    11-April-2014
  * @brief   Header for stm32l1xx_ulp_modes.c module
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2014 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32L1xx_ULP_MODES_H
#define __STM32L1xx_ULP_MODES_H

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
#if !defined (LP_RUN_SRAM_MODE) && !defined (LP_RUN_FLASH_MODE) && !defined (SLEEP_MODE)\
 && !defined (STOP_RTC_LSI_MODE) && !defined (STANDBY_RTC_LSI_MODE)
/* Uncomment the corresponding line to select the STM32L1xx Ultra Low Power mode */
#define LP_RUN_SRAM_MODE
//#define LP_RUN_FLASH_MODE
//#define SLEEP_MODE
//#define STOP_RTC_LSI_MODE
//#define STANDBY_RTC_LSI_MODE
#endif

#define BOR_MODIFY
/* Select the BOR level */
#ifdef BOR_MODIFY
#define BOR_LEVEL OB_BOR_OFF  /* BOR is disabled at power down, the reset is asserted when the VDD power supply reachs the PDR(Power Down Reset) threshold (1.5V) */
//#define BOR_LEVEL OB_BOR_LEVEL1  /* BOR Reset threshold levels for 1.7V - 1.8V VDD power supply */
//#define BOR_LEVEL OB_BOR_LEVEL2  /* BOR Reset threshold levels for 1.9V - 2.0V VDD power supply */
//#define BOR_LEVEL OB_BOR_LEVEL3  /* BOR Reset threshold levels for 2.3V - 2.4V VDD power supply */
//#define BOR_LEVEL OB_BOR_LEVEL4  /* BOR Reset threshold levels for 2.55V - 2.65V VDD power supply */
//#define BOR_LEVEL OB_BOR_LEVEL5  /* BOR Reset threshold levels for 2.8V - 2.9V VDD power supply */
#endif

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void LowPowerRunMode_Measure(void);
void LowPowerRunModeSRAM_Measure(void);
void SleepMode_Measure(void);
void StopRTCLSIMode_Measure(void);
void StandbyRTCLSIMode_Measure(void);

#endif /* __STM32L1xx_ULP_MODES_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
