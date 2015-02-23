#include "RGBLed.h"

RGBLed::RGBLed(int rp, int gp, int bp)
{
    rpin = rp;
    gpin = gp;
    bpin = bp;
    
    pinMode(rpin, OUTPUT);
    pinMode(bpin, OUTPUT);
    pinMode(gpin, OUTPUT);
    
    digitalWrite(rpin, HIGH);
    digitalWrite(gpin, HIGH);
    digitalWrite(bpin, HIGH);
}

void RGBLed::setColor(int r, int g, int b)
{
    digitalWrite(rpin, !r);
    digitalWrite(gpin, !g);
    digitalWrite(bpin, !b);
}

void RGBLed::setColor(int c)
{
    switch (c)
    {
        case 0: // black // off
            digitalWrite(rpin, HIGH);
            digitalWrite(gpin, HIGH);
            digitalWrite(bpin, HIGH);
            break;
        case 1:
            digitalWrite(rpin, HIGH);
            digitalWrite(gpin, HIGH);
            digitalWrite(bpin, LOW);
            break;
        case 2:
            digitalWrite(rpin, HIGH);
            digitalWrite(gpin, LOW);
            digitalWrite(bpin, HIGH);
            break;
        case 3:
            digitalWrite(rpin, HIGH);
            digitalWrite(gpin, LOW);
            digitalWrite(bpin, LOW);
            break;
        case 4:
            digitalWrite(rpin, LOW);
            digitalWrite(gpin, HIGH);
            digitalWrite(bpin, HIGH);
            break;
        case 5:
            digitalWrite(rpin, LOW);
            digitalWrite(gpin, HIGH);
            digitalWrite(bpin, LOW);
            break;
        case 6:
            digitalWrite(rpin, LOW);
            digitalWrite(gpin, LOW);
            digitalWrite(bpin, HIGH);
            break;
        case 7:
            digitalWrite(rpin, LOW);
            digitalWrite(gpin, LOW);
            digitalWrite(bpin, LOW);
            break;
        default:
            break;
    }
}