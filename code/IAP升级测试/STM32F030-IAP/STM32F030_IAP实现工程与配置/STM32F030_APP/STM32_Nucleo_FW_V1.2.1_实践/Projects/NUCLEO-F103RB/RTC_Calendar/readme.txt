/**
  @page RTC_Calendar  RTC Calendar example
  
  @verbatim
  ******************** (C) COPYRIGHT 2014 STMicroelectronics *******************
  * @file    RTC_Calendar/readme.txt 
  * @author  MCD Application Team
  * @version V1.2.0
  * @date    11-April-2014
  * @brief   Description of the RTC Calendar example
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2014 STMicroelectronics</center></h2>
  *
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
   @endverbatim

@par Example Description

This example demonstrates how to setup the RTC peripheral, in terms of prescaler 
and interrupts, to be used to keep time and to generate Second interrupt. 

In the RTC interrupt service routine, the LED2 toggles every 1 s.

To show time, put variable showtime in Watch window.

@par Hardware and Software environment

  - This example runs on STM32F10x Devices.
  - This example has been tested with STMicroelectronics STM32NUCLEO kit(MB1136) rev C 
    and can be easily tailored to any other supported device and development board.
    

@par How to use it? 

In order to make the program work, you must do the following :
 + EWARM
    - Open the RTC_Calendar.eww workspace 
    - Rebuild all files: Project->Rebuild all
    - Load project image: Project->Debug
    - Run program: Debug->Go(F5)
  
 + MDK-ARM
    - Open the RTC_Calendar.uvproj project
    - Rebuild all files: Project->Rebuild all target files
    - Load project image: Debug->Start/Stop Debug Session
    - Run program: Debug->Run (F5)  

 + TrueSTUDIO:
    - Open the TrueSTUDIO toolchain.
    - Click on File->Switch Workspace->Other and browse to TrueSTUDIO workspace directory.
    - Click on File->Import, select General->'Existing Projects into Workspace' and then click "Next". 
    - Browse to the TrueSTUDIO workspace directory, select the project RTC_Calendar.
    - Rebuild all project files: Select the project in the "Project explorer" 
      window then click on Project->build project menu.
    - Run program: Run->Debug (F11)
              
 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
