//
//  ServoSyringe.cpp
//  
//
//  Created by Anh Luong on 4/13/12.
//  Copyright (c) 2012 Anh Luong. All rights reserved.
//
#include "ServoSyringe.h"
#include <math.h>

ServoSyringe::ServoSyringe(int p)
{
    pin = p;
    
    linearDistancePermL = 0;
    
    radiusSyringe = 0;
}

ServoSyringe::ServoSyringe(int p, int min, int max)
{
    pin = p;
    
    linearDistancePermL = 0;
    
    radiusSyringe = 0;
}

void ServoSyringe::pushmL(float amount)
{
    int currentPos = read();
    
    double degree = 2 * asin( (double) (linearDistancePermL/2) / radiusSyringe)* 180 / M_PI;
    
    int steps = amount * ceil(degree);
    
    Serial.println(steps);
    
    int limit = currentPos + steps;
    
    Serial.println(limit);
    
    if(limit > 180)
        limit = 180;
    
    for (int i = currentPos; i < limit; i++)
    {
        write(i);
        delay(dropTime/steps);
        Serial.println(dropTime/steps);
    }
}

void ServoSyringe::setLinearDistancePermL(double amount)
{
    linearDistancePermL = amount;
}
                
void ServoSyringe::setRadiusSyringe(double amount)
{
    radiusSyringe = amount;
}

void ServoSyringe::setDropTime(int amount)
{
    dropTime = amount;
}