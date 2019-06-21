#ifndef __RTC_H
#define __RTC_H

#include "main.h"
#define RTC_CLOCK_SOURCE_LSI  // LSI used as RTC source clock. The RTC Clock



void RTC_init(void);
void RTC_AlarmShow(void);
void RTC_TimeRegulate(void);

#endif

