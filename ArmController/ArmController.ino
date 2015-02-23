// Serial Communication with Pololu Simple Controller
// Anh Luong
// May 6th, 2012
// Device number have to be in hex
// Pololu Protocol

#include <math.h>
#include <SPI.h>         // needed for Arduino versions later than 0018
#include <Ethernet.h>
#include <EthernetUdp.h>         // UDP library from: bjoern@cs.stanford.edu 12/30/2008
#include <Wire.h>
#include <avr/wdt.h>

#define soft_reset() do {wdt_enable(WDTO_15MS); for(;;) {}} while(0)

#define resetPin 22
#define errPin 24

// some variable IDs
#define ERROR_STATUS 0
#define LIMIT_STATUS 3
#define TARGET_SPEED 20
#define INPUT_VOLTAGE 23
#define TEMPERATURE 24
 
// some motor limit IDs
#define FORWARD_ACCELERATION 5
#define REVERSE_ACCELERATION 9
#define DECELERATION 2

#define dev1pp A8
#define dev2pp A9
#define dev3pp A10

// Joint 1 - Elbow
#define HOMEINV1 1
#define MAXPOT1 615
#define MINPOT1 115
#define SPEED1 400

// Joint 2 - Wrist
#define HOMEINV2 0
#define MAXPOT2 602
#define MINPOT2 401
#define SPEED2 3200

// Joint 3 - Shoulder
#define HOMEINV3 0
#define MAXPOT3 700
#define MINPOT3 75
#define SPEED3 400

// Joint 4 - Grip
#define HOMEINV4 0
#define MAXPOT4 1023
#define MINPOT4 0
#define SPEED4 3200

// Shoulder Sweep
#define MiddlePWM 200
#define RightPWM 135
#define LeftPWM 235
#define SSPIN 46

#define DEBUG 1

typedef struct 
{
  uint16_t maxValue;
  uint16_t minValue;
  uint16_t currentValue;
  uint8_t potPin;
  uint8_t address;
  uint8_t centerPos;
  uint8_t inverted;
  uint8_t rotSpeed;
} Joint_t;

// Inverted = + to go 0
//Joint_t elbow = {615, 115, analogRead(A8), A8, 1, 300, 1, 400};
//Joint_t wrist = {1023, 0, analogRead(A9), A9, 2, 512, 0, 3200};
//Joint_t shoulder = {};
//Joint_t grip = {};
//Joint_t shoulderSweep = {};

// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
byte mac[] = {  
  0x90, 0xA2, 0xDA, 0x00, 0xF7, 0x87 };
IPAddress ip(192, 168, 0, 177);

unsigned int localPort = 8888;      // local port to listen on

// buffers for receiving and sending data
char packetBuffer[UDP_TX_PACKET_MAX_SIZE]; //buffer to hold incoming packet,
char  ReplyBuffer[] = "acknowledged";       // a string to send back
char  ErrorReplyBuffer[] = "shit happened but bad package";       // a string to send back

// An EthernetUDP instance to let us send and receive packets over UDP
EthernetUDP Udp;

byte receivedPacket[6] ={0};

// read a serial byte (returns -1 if nothing received after the timeout expires)
int readByte()
{
  char c = Serial1.read();
  
  if(c == 0)
  {
    return -1;
  }
  
  return (byte)c;
}
 
// required to allow motors to move
// must be called when controller restarts and after any error
void exitSafeStart(byte dev)
{
  // Command
  Serial1.write(0xAA);
  
  // Device number
  Serial1.write(dev);
  
  // Data Byte define as 0x03
  Serial1.write(0x03);
}
 
// speed should be a number from -3200 to 3200
void setMotorSpeed(byte dev, int speed)
{
  // Command
  Serial1.write(0xAA);
  
  // Device number
  Serial1.write(dev);
  
  // Data byte define as 0x06(reverse) 0x05(forward)
  if (speed < 0)
  {
    Serial1.write(0x06);  // motor reverse command
    speed = -speed;  // make speed positive
  }
  else
  {
    Serial1.write(0x05);  // motor forward command
  }
  
  // Data byte 3 & 4
  Serial1.write(speed & 0x1F);
  Serial1.write(speed >> 5);
}
 
// returns the specified variable as an unsigned integer.
// if the requested variable is signed, the value returned by this function
// should be typecast as an int.
unsigned int getVariable(byte dev, unsigned char variableID)
{
  // Command
  Serial1.write(0xAA);
  
  // Device number
  Serial1.write(dev);
  
  // Data Byte define as 0x21
  Serial1.write(0x21);
  
  // Data byte 3
  Serial1.write(variableID);
  
  return readByte() + 256 * readByte();
}

unsigned char setMotorLimit(byte dev, unsigned char  limitID, unsigned int limitValue)
{
  // Command
  Serial1.write(0xAA);
  
  // Device number
  Serial1.write(dev);
  
  // Data Byte define as 0x22
  Serial1.write(0x22);
  
  // Limit ID
  Serial1.write(limitID);
  
  // Limit byte
  Serial1.write(limitValue & 0x7F);
  Serial1.write(limitValue >> 7);
  
  return readByte();
}

int getPotValue(byte dev)
{
  switch(dev)
  {
    case 1: return analogRead(dev1pp);
    case 2: return analogRead(dev2pp);
    case 3: return analogRead(dev3pp);
    //case 4: return analogRead(dev4pp);
    default: return -1;
  }
  
  return -1; 
}

int getAngleValue(byte dev)
{
  int potVal = getPotValue(dev);
  
  if(DEBUG)
    if(potVal == -1)
      Serial.println("Potentiometer is not connected!");
    
  return map(potVal, 0, 1023, 0, 270);
}

int potToAngle(int pot)
{
  return map(pot, 0, 1023, 0, 270);
}
 
int angleToPot(int angle)
{
  return map(angle, 0, 270, 0, 1023);
}

void run(byte dev, int motorSpeed, int duration)
{
  // Run motor
  setMotorSpeed(dev, motorSpeed);  // full-speed forward
  // signed variables must be cast to ints:
  //if(DEBUG)
    //Serial.println((int)getVariable(1, TARGET_SPEED));
  
  delay(duration);
  
  // Stop
  setMotorSpeed(dev, 0);
  
  if (digitalRead(errPin) == HIGH)
  {
        Serial.print("Error Status: 0x");
        Serial.println(getVariable(dev, ERROR_STATUS), HEX);
        // once all other errors have been fixed,
        // this lets the motors run again
        exitSafeStart(dev);
  }  
}

void goToAngle(byte dev, int angle)
{
  int mx, mn, inv, speed;
  
  // Get max, min and inverted
  if(dev == 1)
  {
    mx = MAXPOT1;
    mn = MINPOT1;
    inv = HOMEINV1;
    speed = SPEED1;
  }
  else if(dev == 2)
  {
    mx = MAXPOT2;
    mn = MINPOT2;
    inv = HOMEINV2;
    speed = SPEED2;
  }
  else if(dev == 3)
  {
    mx = MAXPOT3;
    mn = MINPOT3;
    inv = HOMEINV3;
    speed = SPEED3;
  }
  else if(dev == 4)
  {
    mx = MAXPOT4;
    mn = MINPOT4;
    inv = HOMEINV4;
    speed = SPEED4;
  }
  else if(dev == 5)
  {
    if(angle < 46 || angle > -46)
      if(angle < 0)
        analogWrite(SSPIN, map(angle, -45, 0, RightPWM, MiddlePWM));
      else if(angle > 0)
        analogWrite(SSPIN, map(angle, 0, 45, MiddlePWM, LeftPWM));
      else
        analogWrite(SSPIN, MiddlePWM);
    return;
  }
  else
  {
    printf("wrong device number!");
    return;
  }
  
  int potVal = getPotValue(dev) + angleToPot(angle);
    
  if(potVal > mx || potVal < mn)
  {
    Serial.println("exceed physical limits!");
    return;
  }
  
  int currentPotVal = getPotValue(dev);
  
  // Need to go forward
  // - go to 0, + go to 1023
  if(currentPotVal < potVal)
  {
    while(getPotValue(dev) < potVal)
    {
      if(inv)
      {
        run(dev, -speed, 1000);
      }
      else
      {
        run(dev, speed, 1000);
      }
    }
  }
  // Need to go backward
  // - go to 0, + go to 1023
  else if(currentPotVal > potVal)
  {
    while(getPotValue(dev) > potVal)
    {
      if(inv)
      {
        run(dev, speed, 1000);
      }
      else
      {
        run(dev, -speed, 1000);
      }
    }
  }
  else
  {
    return;
  }
}

// Home Seq
void home(byte dev)
{
  int mx, mn, inv, speed;
  
  if(dev == 1)
  {
    mx = MAXPOT1;
    mn = MINPOT1;
    inv = HOMEINV1;
    speed = SPEED1;
  }
  else if(dev == 2)
  {
    mx = MAXPOT2;
    mn = MINPOT2;
    inv = HOMEINV2;
    speed = SPEED2;
  }
  else if(dev == 3)
  {
    mx = MAXPOT3;
    mn = MINPOT3;
    inv = HOMEINV3;
    speed = SPEED3;
  }
  else if(dev == 4)
  {
    return;
  }
  else if(dev == 5)
  {
    analogWrite(SSPIN, MiddlePWM);
    return;
  }
  else
  {
    printf("wrong device number!");
    return;
  }
  
  int potVal = getPotValue(dev);
  
  if(DEBUG)
  {
    Serial.print(dev, DEC);
    Serial.print(" ");
    Serial.println(potVal);
  }
   
   /**
   // Home
   // +, pot angle is current smaller than minimum, correct it
    while(potVal < mn)
    {
      if(inv)
        run(dev, speed, 1000);
      else
        run(dev, -speed, 1000);
      
      potVal = getPotValue(dev);
    }
  
    // -, pot angle is current more than minimum, correct it
    while(potVal > mn)
    {
      if(inv)
        run(dev, -speed, 1000);
      else
        run(dev, speed, 1000);
     
      potVal = getPotValue(dev);
    }
  */
  
  // - go to 1023 and + go to 0
  if(dev == 1)
  {
    while(getPotValue(dev) < mx)
    {
      run(dev, -speed, 1000);
    }
    
    while(getPotValue(dev) > mx)
    {
      run(dev, speed, 1000);
    }
  }
  // - go to 0 and + go to 1023
  else if(dev == 2)
  {
    while(getPotValue(dev) < mx)
    {
      run(dev, speed, 1000);
    }
    
    while(getPotValue(dev) > mx)
    {
      run(dev, -speed, 1000);
    }
  }
  // - go to 0 and + go to 1023
  else if(dev == 3)
  {
    while(getPotValue(dev) < mx)
    {
      run(dev, speed, 1000);
    }
    
    while(getPotValue(dev) > mx)
    {
      run(dev, -speed, 1000);
    }
  }
  else
  {
    Serial.println("No such device");
  }
  
}

void setup()
{
  // start the Ethernet and UDP:
  Ethernet.begin(mac,ip);
  Udp.begin(localPort);
  
  // Setup debugger and serial communication with the pololu chips
  Serial.begin(115200);    // for debugging (optional)
  Serial1.begin(19200);
  
  // Setup I2C
  Wire.begin();
 
  // briefly reset SMC when Arduino starts up (optional)
  pinMode(resetPin, OUTPUT);
  digitalWrite(resetPin, LOW);  // reset SMC
  delay(1);  // wait 1 ms
  pinMode(resetPin, INPUT);  // let SMC run again
 
  // must wait at least 1 ms after reset before transmitting
  delay(5);
 
  // this lets us read the state of the SMC ERR pin (optional)
  pinMode(errPin, INPUT);
 
  Serial1.write(0xAA);  // send baud-indicator byte
  setMotorLimit(1, FORWARD_ACCELERATION, 4);
  setMotorLimit(1, REVERSE_ACCELERATION, 10);
  setMotorLimit(1, DECELERATION, 20);
  
  setMotorLimit(2, FORWARD_ACCELERATION, 4);
  setMotorLimit(2, REVERSE_ACCELERATION, 10);
  setMotorLimit(2, DECELERATION, 20);
  
  setMotorLimit(3, FORWARD_ACCELERATION, 4);
  setMotorLimit(3, REVERSE_ACCELERATION, 10);
  setMotorLimit(3, DECELERATION, 20);
  
  setMotorLimit(4, FORWARD_ACCELERATION, 4);
  setMotorLimit(4, REVERSE_ACCELERATION, 10);
  setMotorLimit(4, DECELERATION, 20);
  
  // clear the safe-start violation and let the motor run
  exitSafeStart(1);
  exitSafeStart(2);
  exitSafeStart(3);
  exitSafeStart(4);
  
  // Homing
  home(1);
  //home(2);
  home(3);
  home(5);
  
  // test motors here 
  // use run() or gotoangle()
  // gotoangle is prefered.
}

void loop()
{
  // if there's data available, read a packet
  int packetSize = Udp.parsePacket();
  if(packetSize)
  {
    int device = 0; //0 is the arm, 1 is the drive system
    
    Serial.print("Received packet of size ");
    Serial.println(packetSize);
    Serial.print("From ");
    IPAddress remote = Udp.remoteIP();
    for (int i = 0; i < 4; i++)
    {
      Serial.print(remote[i], DEC);
      if (i < 3)
      {
        Serial.print(".");
      }
    }
    Serial.print(", port ");
    Serial.println(Udp.remotePort());

    // read the packet into packetBufffer
    Udp.read(packetBuffer,UDP_TX_PACKET_MAX_SIZE);
    Serial.println("Contents:");
    /**
    char output[30];
    
    if(packetBuffer[0] < 0) {
      output[0] = '-';
      packetBuffer[0]*=-1;
    }
    else
      output[0] = '+';
    output[1] = packetBuffer[0]/100+0x30;
    output[2] = packetBuffer[0]/10%10+0x30;
    output[3] = packetBuffer[0]%10+0x30;
    output[4] = ' ';
    
    if(packetBuffer[1] < 0) {
      output[5] = '-';
      packetBuffer[1]*=-1;
    }
    else
      output[5] = '+';
    output[6] = packetBuffer[1]/100+0x30;
    output[7] = packetBuffer[1]/10%10+0x30;
    output[8] = packetBuffer[1]%10+0x30;
    output[9] = ' ';
    
    if(packetBuffer[2] < 0) {
      output[10] = '-';
      packetBuffer[2]*=-1;
    }
    else
      output[10] = '+';
    output[11] = packetBuffer[2]/100+0x30;
    output[12] = packetBuffer[2]/10%10+0x30;
    output[13] = packetBuffer[2]%10+0x30;
    output[14] = ' ';
    
    if(packetBuffer[3] < 0) {
      output[15] = '-';
      packetBuffer[3]*=-1;
    }
    else
      output[15] = '+';
    output[16] = packetBuffer[3]/100+0x30;
    output[17] = packetBuffer[3]/10%10+0x30;
    output[18] = packetBuffer[3]%10+0x30;
    output[19] = ' ';
    
    if(packetBuffer[4] < 0) {
      output[20] = '-';
      packetBuffer[4]*=-1;
    }
    else
      output[20] = '+';
    output[21] = packetBuffer[4]/100+0x30;
    output[22] = packetBuffer[4]/10%10+0x30;
    output[23] = packetBuffer[4]%10+0x30;
    output[24] = ' ';
    
    if(packetBuffer[5] < 0) {
      output[25] = '-';
      packetBuffer[5]*=-1;
    }
    else
      output[25] = '+';
    output[26] = packetBuffer[5]/100+0x30;
    output[27] = packetBuffer[5]/10%10+0x30;
    output[28] = packetBuffer[5]%10+0x30;
    output[29] = 0;
    Serial.println(output);
    */
    
    Serial.print((byte)packetBuffer[0], DEC);
    Serial.print(" ");
    Serial.print((byte)packetBuffer[1], DEC);
    Serial.print(" ");
    Serial.print((byte)packetBuffer[2], DEC);
    Serial.print(" ");
    Serial.print((byte)packetBuffer[3], DEC);
    Serial.print(" ");
    Serial.print((byte)packetBuffer[4], DEC);
    Serial.print(" ");
    Serial.println((byte)packetBuffer[5], DEC);
    Serial.println(packetBuffer);

    // send a reply, to the IP address and port that sent us the packet we received
    Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
    Udp.write(ReplyBuffer);
    Udp.endPacket();
    
    device = (byte)packetBuffer[0];

    // Send commands  
    if(device == 0)
    {
      soft_reset();
    }
    else if(device == 1)
    {
      // Transfer Commands
      Wire.beginTransmission(0xa0); // transmit to slave device 0
      Wire.write((unsigned byte)packetBuffer[1]); // Brake
      Wire.write((unsigned byte)packetBuffer[2]); // Speed
      Wire.write((unsigned byte)packetBuffer[3]); // Angle
      Wire.endTransmission();    // stop transmitting
      delay(500);
    }
    else if(device == 0x10)
    {
      if(packetSize == 2)
        goToAngle(1, (int)packetBuffer[1]);
    }
    else if(device == 0x11)
    {
      if(packetSize == 3)
        run(1, map((byte)packetBuffer[1], 0, 255, -SPEED1, SPEED1), (int)packetBuffer[2]*100);
    }
    else if(device == 0x20)
    {
      if(packetSize == 2)
        goToAngle(2, (int)packetBuffer[1]);
    }
    else if(device == 0x21)
    {
      if(packetSize == 3)
        run(2, map((byte)packetBuffer[1], 0, 255, -SPEED2, SPEED2), (int)packetBuffer[2]*100);
    }
    else if(device == 0x30)
    {
      if(packetSize == 2)
        goToAngle(3, (int)packetBuffer[1]);
    }
    else if(device == 0x31)
    {
      if(packetSize == 3)
        run(3, map((byte)packetBuffer[1], 0, 255, -SPEED3, SPEED3), (int)packetBuffer[2]*100);
    }
    else if(device == 0x40)
    {
      if(packetSize == 2)
        goToAngle(4, (int)packetBuffer[1]);
    }
    else if(device == 0x41)
    {
      run(4, map((byte)packetBuffer[1], 0, 255, -SPEED4, SPEED4), (int)packetBuffer[2]*100);
    }
    else if(device == 0x50)
    {
      goToAngle(5, (int)packetBuffer[1]);
    }
    else
    {
      // send a reply, to the IP address and port that sent us the packet we received
      Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
      Udp.write(ErrorReplyBuffer);
      Udp.endPacket();
    }
  }
  else
  {
    // Transfer Commands
      Wire.beginTransmission(0xa0); // transmit to slave device 0
      Wire.write((byte)0); // Brake
      Wire.write((byte)10); // Speed
      Wire.write((byte)0); // Angle
      Wire.endTransmission();    // stop transmitting
      delay(500);
  }
  
  delay(10);
}

