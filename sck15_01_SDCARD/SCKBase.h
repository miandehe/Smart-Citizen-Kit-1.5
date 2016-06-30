#include <Arduino.h>

enum idle_t
{
  IDLE_0,
  IDLE_1,
  IDLE_2
};

class SCKBase {
public:  
    void begin();
    void awake();
    boolean button();
    void sleep();
    
    void  idle(idle_t idleMode);
    void  standby();
private:


};

