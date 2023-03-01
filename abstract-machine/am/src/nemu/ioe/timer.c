#include <am.h>
#include <nemu.h>

static AM_TIMER_UPTIME_T boot_time;

void __am_timer_init() {
  boot_time.us = 0;
}

void __am_timer_uptime(AM_TIMER_UPTIME_T *uptime) {
  uptime->us = boot_time.us + 1000;
  boot_time.us = uptime->us;
}

void __am_timer_rtc(AM_TIMER_RTC_T *rtc) {
  rtc->second = 0;
  rtc->minute = 0;
  rtc->hour   = 0;
  rtc->day    = 0;
  rtc->month  = 0;
  rtc->year   = 1900;
}
