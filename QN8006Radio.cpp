/*
QN8006 I2C FM Radio Library

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA


Evgeniy Gichev 2021
*/

#include <Arduino.h>
#include <QN8006Radio.h>
/*
QN8006Radio::QN8006Radio(int address)
{
  _address = address;
}

*/
QN8006Radio::QN8006Radio()
{
  _address = 0x2B;
}

void QN8006Radio::initReceive(byte s_t, byte mode)
{
  byte buf[4]; // 5-byte data value.
  buf[0] = 0x00; //start subaddress SYSTEM1 Address: 00h
  buf[1] = 0b10000001; // Enter Receiving mode. CH is determined by the content in CH[9:0].
  
  if (mode) buf[2] = 0b00001011; // Set STEREO & set IDLE to infinity (never go to Standby)
  else buf[2] = 0b00011011; // Set MONO & set IDLE to infinity (never go to Standby)
  if (s_t > 31)   buf[3] = 0b10101010;  //b7 = 1: RX CCA threshold MSB. See CCA register 19h [4:0].
  else buf[3] = 0b00101010;  //b7 = 0: RX CCA threshold MSB. See CCA register 19h [4:0].
  
	Wire.beginTransmission(_address);
  Wire.write(buf,4); //send data
	Wire.endTransmission();
  
  buf[0] = 0x19; //start subaddress  CCA threshold
  buf[1] = (s_t & 0b00011111) | 0b01000000;
  Wire.beginTransmission(_address);
  Wire.write(buf,2); //send data
	Wire.endTransmission();
}

int QN8006Radio::getStat(unsigned char *stat){

  unsigned char buf[32];
  memset (stat, 0, 5);
	Wire.requestFrom (_address, 32);    //reading QN8006
//  while(Wire.available() < 1);
		for (int i = 0; i < 32; i++) {
			buf[i] = Wire.read ();
//     Serial.println(buf[i]);
		}
   stat[0] = buf[10];  //cuirrent frequensy in steps b0-b7
   stat[1] = buf [13] & 0b00000011; //cuirrent frequensy in steps b8-b9
   stat[2] = buf[30] - 40; //RSSI
   stat[3] = buf[28] & 0x01; // 1= mono ; 0 = stereo
   stat[4] = (buf[28] & 0b01000000) >> 6; // auto tuning chanel found = 0; not found = 1
   stat[5] = (buf[2] & 0b00100000) >> 5; // autoscan in progress = 1; completed = 0
  return 1;
 // }


}

void QN8006Radio::searchUp(unsigned char *stat){
  byte buf[5]; 
//    Serial.println("searching...");
  if (QN8006Radio::getStat (stat)) {
        //set start frequency to current /begining
    buf[0] = 0x09; //start subaddress
    if (stat[0] == 128 && stat[1] == 2) { //end reached, set to 88 Mhz
      buf[1] =  240;
      buf[3] = 0;
    }
    else {
      unsigned int frequencyB = stat[0] + stat[1]*256 + 2;
      byte frequencyH = frequencyB >> 8;
      byte frequencyL = frequencyB & 0XFF;
      
      buf[1] =  frequencyL;
      buf[3] = frequencyH | frequencyH << 2;
    }

    //set stop frequency to 108 Mhz
    buf[2] = 0b10000000;
    buf[3]|= 0b01100000;
    
 //       Serial.println (buf[1]);
 //       Serial.println (buf[3]);

    //send start/stop settings
    Wire.beginTransmission(_address); 
    Wire.write(buf,4); //send data
    Wire.endTransmission();
  

    // start searching
    buf[0] = 0x00; //start subaddress
    buf[1] = 0b10100000;
    Wire.beginTransmission(_address); 
    Wire.write(buf,2); //send data
    Wire.endTransmission();
  }
  
}


void QN8006Radio::mute()
{  
    // Mute
  byte buf[2]; 
  buf[0] = 0x00; //start subaddress SYSTEM1 Address: 00h
  buf[1] =0b10001000;
  Wire.beginTransmission(_address); 
  Wire.write(buf,2); //send data
	Wire.endTransmission();
//  Serial.println("mute");
  
}

void QN8006Radio::unmute()
{  
    // Mute
  byte buf[2]; 
  buf[0] = 0x00; //start subaddress SYSTEM1 Address: 00h
  buf[1] =0b10000001;
  Wire.beginTransmission(_address); 
  Wire.write(buf,2); //send data
	Wire.endTransmission();
//  Serial.println("unmute");
  
}

void QN8006Radio::setFrequency(float frequency)
{
  byte buf[2]; 
  unsigned int frequencyB = (frequency*100 - 7600)/5;
	byte frequencyH = frequencyB >> 8;
	byte frequencyL = frequencyB & 0XFF;
  buf[0] = 0x08; //start subaddress 
  buf[1] = frequencyL;
  Wire.beginTransmission(_address); 
  Wire.write(buf,2); //send data
	Wire.endTransmission();
  buf[0] = 0x0b; //start subaddress 
  buf[1] = frequencyH;
  Wire.beginTransmission(_address); 
  Wire.write(buf,2); //send data
	Wire.endTransmission();
  
}