 
#ifndef MQ7_H
#define MQ7_H
 
#include "mbed.h"
 
// Interface to control an MQ-7 (Carbon Monoxide) sensor 
 
class MQ7 {
public:
 
    /** Create interfaces with gas sensor module
     *
     * @param alr A DigitalIn, alarm output to microcontroller
     * @param hsw A PwmOut, heat switch input from microcontroller, active low
     */
    MQ7(PinName alr, PinName hsw) : _alr(alr), _hsw(hsw){}
    
    int getAlarm(){
        return _alr.read();
    }
    
    void setHeat(float voltage){
        _hsw.write(voltage);
    }
     
 
protected:
    DigitalIn _alr;
    PwmOut _hsw;
 
};
 
#endif