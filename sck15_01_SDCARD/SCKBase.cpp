#include "Constants.h"
#include "SCKBase.h"
#include "SCKDrivers.h"

SCKDriver driver__;

boolean state_ = false;
boolean state_ant = !state_;

boolean SCKBase::button() {
  if (state_ == true) 
  {
    state_=false;
    return true;
  }
  return state_;
}

uint32_t count = 0;
void blink() {
  count++;
  if (count>100000)
    {
      state_ = true;
      count = 0;
    }
}

void SCKBase::begin() {
  driver__.begin();
  driver__.writeCurrent(200);
  driver__.ADCini();
  driver__.RTCini();
  driver__.RTCadjust(driver__.sckDate(__DATE__,__TIME__));
  driver__.accelDefault();
  driver__.RGBled(0,255,0); 
  attachInterrupt(CONTROL, blink, LOW);
  
//  SCB->SCR |= 1<<2; // Enable deep-sleep mode
//  
//  // Set the EIC (External Interrupt Controller) to wake up the MCU
//  // on an external interrupt from digital pin 7. (It's EIC channel 5)
//  EIC->WAKEUP.reg = EIC_WAKEUP_WAKEUPEN5;

}

void SCKBase::awake() {
  digitalWrite(PS, HIGH);
  driver__.RGBled(0,255,0); 
}

void SCKBase::sleep() {
  driver__.RGBled(0,0,0);
  driver__.ESPoff();
  driver__.ADCoff();
  digitalWrite(PS, LOW);
}

/*******************************************************************************
* Name: standby
* Description: Putting SAMD21G18A into idle mode. This is the lowest current 
*              consumption mode. Requires separate handling of clock and 
*          peripheral management (disabling and shutting down) to achieve 
*          the desired current consumption.
*
* Argument    Description
* =========   ===========
* 1. idleMode   Idle mode level (0, 1, 2) where IDLE_2 level provide lowest 
*         current consumption in this mode.
* 
*******************************************************************************/
void  SCKBase::idle(idle_t idleMode)
{
  SCB->SCR &= ~SCB_SCR_SLEEPDEEP_Msk;
  PM->SLEEP.reg = idleMode;
  __DSB();
  __WFI();
}

/*******************************************************************************
* Name: standby
* Description: Putting SAMD21G18A into standby mode. This is the lowest current 
*              consumption mode. Use this together with the built-in RTC (use 
*              RTCZero library) or external pin interrupt to wake up through 
*              external event triggering.
*
* Argument    Description
* =========   ===========
* 1. NIL
* 
*******************************************************************************/
void  SCKBase::standby()
{
  SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;
  __DSB();
  __WFI();
}


