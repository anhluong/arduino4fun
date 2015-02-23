/*
 LedCube.h - Library for controlling a LED cube
 Created by Gamaiel Zavala (gzip), October 9, 2009.
 Released into the public domain.
 */

#ifndef LedCube_h
#define LedCube_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

class LedCube
{
public:
    LedCube(byte size, byte levelPins[], byte colPins[]);
    
    byte getCols();
    byte getLevels();
    int getColPin(int r);
    int getLevelPin(int lv);
    
    void light(byte level, byte col, byte val);
    void lightOn(byte level, byte col);
    void lightOne(byte level, byte col, byte val);
    void lightOff(byte level, byte col);
    void lightPulse(byte level, byte col, unsigned int wait = 5);
    void lightSequence(byte seq[], byte length, unsigned int loops = 5, byte duration = 0);
    void lightLevel(byte r, unsigned int wait);
    void lightRow(byte r, byte level, unsigned int wait = 50);
    void lightColumn(byte col, unsigned int wait = 50);
    void lightDrop(byte col, unsigned int wait = 50);
    void lightPerimeter(byte level, byte rotations, unsigned int wait = 50);
    void randomLight(byte numLights, unsigned int wait = 50);
    void randomColumn(byte numColumns = 1, unsigned int wait =  50);
    
    void enableBuffer(boolean enable = true);
    void invertBuffer(boolean invert = true);
    void clearBuffer();
    void fillBuffer();
    void drawBuffer(unsigned int wait = 5, byte duration = 0);
    byte getBufferAt(byte lv, byte col);
    
private:
    byte cols;
    byte levels;
    /*
     byte *colPins;
     byte *levelPins;
     byte **buffer;
     */
    byte colPins[9];
    byte levelPins[3];
    byte buffer[3][9];
    boolean bufferEnabled;
    boolean bufferInverted;
    
    void toggleBuffer (byte val);
};

#endif

