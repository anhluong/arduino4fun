/* We start by including the library */
#include <LedControl.h>
#include <binary.h>

/* Make a new instance of an LedControl object
* Params :
* int dataPin The pin on the Arduino where data gets shifted out (Din on MAX)
* int clockPin The pin for the clock (CLK on MAX)
* int csPin The pin for enabling the device (LD/CS on MAX)
* int numDevices The maximum number of devices that can be controlled
*/
LedControl lc1=LedControl(12,11,10,1);

void setup()
{
  
}

void loop()
{
  lc1.setColumn(0, 0, B00001111);
}
