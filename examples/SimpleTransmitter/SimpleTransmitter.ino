/*
QN8006 Transmitter Example


- QN8006 module top view:

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
9 ----> Antenna TX
10 ---> Antenna RX
*/

#include <Wire.h>
#include <QN8006Radio.h>

QN8006Radio radio = QN8006Radio();

byte output_power = 100; // range 0 ... 255
byte st_mo_tx = 0;       // tx stereo (1) or mono (0) mode selection
float frequency = 76.0;  // range 76.0 ... 108.0 MHz
byte cap_load = 25;      // range 0 ... 63, default 31, need for correct xtal frequency
byte tx_fedv = 110;      // range 0 ... 255, default 108

void setup()
{ 
  Wire.begin();
  radio.resetDevice(); //reset all registers to default value
  radio.initTransmit(st_mo_tx);
  radio.setOutputPower(output_power);
  radio.setFrequency(frequency);
  radio.setCrystalCapLoad(cap_load);
  //radio.setTxFrequencyDeviation(tx_fedv);
}

void loop()
{
  delay(86400);
}
