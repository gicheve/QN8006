
/*  - QN8006 module:

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

double old_frequency;
double frequency;
int search_mode = 0;
int search_direction;
unsigned long last_pressed;
unsigned char buf[5];
int signal_level;
double current_freq;
unsigned long current_millis = millis();
int inByte;
int flag=0;
byte search_trshold = 34; //Autosearch RSSI treshold
byte stereo = 1;
byte mono = 0;
 
void setup() {
  Serial.begin(57600);
  Wire.begin();
  Wire.setClock(400000);
  radio.initReceive(search_trshold, stereo);
  delay (10);
  radio.setFrequency(89.0); // set default station

}


void loop () {
 unsigned char stat[6];

  if (Serial.available()>0) {
    inByte = Serial.read();
    if (inByte == '+' ){  //accept only + from keyboard
      flag=0;
    }
  }
 
 if (radio.getStat(stat)) {
   current_freq = (stat[0] +  stat[1] * 256) * 0.05 + 76;
   signal_level = stat [2];
   Serial.print("Current freq: ");
   Serial.print(current_freq);
   Serial.print(" rssi: ");
   Serial.print(signal_level);
   Serial.print((stat[3] ==1) ? " Mono" : " Stereo");
   Serial.print(" searching: ");
   Serial.print((stat[5] ==  1 ? "Yes" : "No" ));
   Serial.print(" found: ");
   Serial.println((stat[4] ==  0 ? "Yes" : "No" ));

  if (inByte == '+') {
    last_pressed = current_millis;
    search_mode = 1;
    radio.searchUp(stat);
  }
  

 }
delay (500);
}
