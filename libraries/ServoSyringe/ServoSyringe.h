//
//  ServoSyringe.h
//  
//
//  Created by Anh Luong on 4/13/12.
//  Copyright (c) 2012 Anh Luong. All rights reserved.
//

#ifndef _ServoSyringe_h
#define _ServoSyringe_h

#if (ARDUINO >= 100)
#include <Arduino.h>
#else
#include <WProgram.h>
#endif

#include "Servo.h"

class ServoSyringe : public Servo
{
    private :
    int pin;
    double linearDistancePermL;
    double radiusSyringe;
    int dropTime;
    
    public :
    ServoSyringe(int p);
    ServoSyringe(int p, int min, int max);
    
    void setLinearDistancePermL(double amount);
    void setRadiusSyringe(double amount);
    void setDropTime(int amount);
    
    void pushmL(float amount);
};

#endif
