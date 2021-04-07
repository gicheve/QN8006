/* QN8006 Example


- QN8006 module:

Top view:
+-10--9--8--7--6-+
|  +------+  ++  |
|  | QN   |  ||  |
|  | 8006 |  ||  |
|  +------+  ++  |
+--1--2--3--4--5-+

1 ----> Arduino SDA
2 ----> Arduino SCL
3 ----> Audio in (left channel)
4 ----> Audio in (right channel)
5 ----> +5V
6 ----> GND
7 ----> Audio out (right channel)
8 ----> Audio out (left channel)
10 ---> Antenna

*/
#include <Wire.h>
#include <QN8006Radio.h>

QN8006Radio radio = QN8006Radio();

byte search_trshold = 34; //Autosearch RSSI treshold
byte stereo = 1;
byte mono = 0;

void setup()
{ 
  Wire.begin();
  radio.initReceive(search_trshold, stereo);
  radio.setFrequency(93.0); // pick your own frequency
}

void loop()
{
}
