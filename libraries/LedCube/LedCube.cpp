/*
 LedCube.cpp - Library for controlling a LED cube
 Created by Gamaiel Zavala (gzip), October 9, 2009.
 Released into the public domain.
 */

#include "LedCube.h"

LedCube::LedCube(byte size, byte lp[], byte cp[]) : levels(size), cols(size*size), bufferEnabled(false), bufferInverted(false)
{
    randomSeed(analogRead(0));
    /*
     levelPins = new byte[levels];
     colPins = new byte[cols];
     buffer = new byte*[levels];
     */
    
    for (byte i= 0; i < levels; i++)
    {
        levelPins[i] = lp[i];
        pinMode(levelPins[i], OUTPUT);
        //buffer[i] = new byte[cols];
    }
    
    for (byte i= 0; i < cols; i++)
    {
        colPins[i] = cp[i];
        pinMode(colPins[i], OUTPUT);
    }
    
    clearBuffer();
}

byte LedCube::getCols()
{
    return cols;
}

byte LedCube::getLevels()
{
    return levels;
}

int LedCube::getLevelPin(int lv)
{
    return lv && lv <= levels ? levelPins[lv] : 0;
}

int LedCube::getColPin(int col)
{
    return col && col <= cols ? colPins[col] : 0;
}

// buffer methods
void LedCube::enableBuffer(boolean enable)
{
    bufferEnabled = enable;
}

void LedCube::invertBuffer(boolean invert)
{
    bufferInverted = invert;
}

void LedCube::clearBuffer()
{
    toggleBuffer(0);
}

void LedCube::fillBuffer()
{
    toggleBuffer(1);
}

void LedCube::toggleBuffer(byte val)
{
    for(byte lv=0; lv < levels; lv++)
        for(byte col=0; col < cols; col++)
            buffer[lv][col] = val;
}

byte LedCube::getBufferAt(byte lv, byte col)
{
    return buffer[lv][col];
}

void LedCube::drawBuffer(unsigned int wait, byte duration)
{
    byte seq[54];
    byte num = 0;
    for(byte lv=0; lv<levels; lv++) {
        for(byte col=0; col<cols; col++) {
            if(buffer[lv][col]){
                //Serial.print(buffer[r][c],DEC); Serial.print(':'); Serial.print(r,DEC); Serial.print(','); Serial.print(c,DEC); Serial.print(' ');
                seq[num] = lv;
                seq[num+1] = col;
                num+=2;
            }
        } 
    }
    //Serial.println(num,DEC);
    bufferEnabled = false;
    bufferInverted = false;
    lightSequence(seq,num,wait,duration);
    bufferEnabled = true;
}

void LedCube::lightOne(byte lv, byte col, byte val)
{
    if(lv<levels && col<cols)
    {
        if(bufferEnabled)
        {
            buffer[lv][col] = val;
            //Serial.print(val,DEC);
        } 
        else
        {
            digitalWrite(colPins[col], val);
            digitalWrite(levelPins[lv], val);
            
            for(byte y=0; y < levels; y++)
                if(y != lv)
                    digitalWrite(levelPins[y], !val);
            for(byte x=0; x < cols; x++)
                if(x != col)
                    digitalWrite(colPins[x], !val);
        }
    }
}

void LedCube::light(byte lv, byte col, byte val)
{
    if(lv<levels && col<cols){
        if(bufferEnabled){
            buffer[lv][col] = val;
            //Serial.print(val,DEC);
        } else {
            digitalWrite(colPins[col], val);
            digitalWrite(levelPins[lv], val);
        }
    }
}

void LedCube::lightOff(byte level, byte col)
{
    light(level,col,(bufferInverted ? HIGH : LOW));
}

void LedCube::lightOn(byte lv, byte col)
{
    light(lv,col,(bufferInverted ? LOW : HIGH));
}

void LedCube::lightPulse(byte lv, byte col, unsigned int wait)
{
    lightOn(lv, col);
    if(!bufferEnabled){
        delay(wait);
        lightOff(lv, col);
    }
}

void LedCube::lightSequence(byte seq[], byte length, unsigned int loops, byte duration)
{
    if(length%2){ return; }
    
    if(bufferEnabled){ loops = 1; }
    
    for (unsigned int l=0; l<loops; l++) {
        for (byte s=0; s<length; s+=2) {
            if(bufferEnabled){
                lightOn(seq[s],seq[s+1]);
            } else {
                lightPulse(seq[s],seq[s+1],duration);
            }
        } 
    }
}

void LedCube::lightLevel(byte r, unsigned int wait)
{
    if(r && r<levels){
        byte seq[2 * cols];
        for(byte c=0; c<cols; c++) {
            seq[c*2] = r;
            seq[c*2+1] = c;
        }
        lightSequence(seq,sizeof(seq),wait);
    }
}

void LedCube::lightRow(byte r, byte level, unsigned int wait)
{
    if(r && level && r<=levels && level<=cols){
        byte start = (r-1)*levels;
        byte seq[] = {level-1,start,level-1,start+1,level-1,start+2};
        lightSequence(seq,levels*2,wait);
    }
}

void LedCube::lightColumn(byte col, unsigned int wait)
{
    if(col && col<=cols){
        byte seq[] = {0,col-1,1,col-1,2,col-1};
        lightSequence(seq,levels*2,wait);
    }
}

void LedCube::lightDrop(byte col, unsigned int wait)
{
    for(byte r=0; r<levels; r++)    {
        lightPulse(r,col-1,wait);
    }
}

void LedCube::lightPerimeter(byte level, byte rotations, unsigned int wait)
{
    byte seq[] = {level,0,level,1,level,2,level,5,level,8,level,7,level,6,level,3};
    lightSequence(seq, sizeof(seq), rotations, wait);
}

void LedCube::randomLight(byte numLights, unsigned int wait)
{
    for (byte l=0; l<numLights; l++) {
        lightPulse(random(0,levels), random(0,cols), wait);
    } 
}

void LedCube::randomColumn(byte numColumns, unsigned int wait)
{
    for (byte c=0; c < numColumns; c++) {
        lightColumn(random(0,cols), wait);
    } 
}

