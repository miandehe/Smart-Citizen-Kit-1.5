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
  attachInterrupt(CONTROL, blink, LOW);
  SCB->SCR |= 1<<2; // Enable deep-sleep mode
  
  // Set the EIC (External Interrupt Controller) to wake up the MCU
  // on an external interrupt from digital pin 7. (It's EIC channel 5)
  EIC->WAKEUP.reg = EIC_WAKEUP_WAKEUPEN5;
}

void SCKBase::awake() {
  digitalWrite(RED, HIGH);
  digitalWrite(GREEN, LOW);
  digitalWrite(BLUE, HIGH); 
  
}

void SCKBase::sleep() {
  digitalWrite(RED, HIGH);
  digitalWrite(GREEN, HIGH);
  digitalWrite(BLUE, HIGH); 
  driver__.ESPoff();
  driver__.ADCoff();
}




