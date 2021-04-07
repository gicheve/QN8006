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

#include <Wire.h>

#ifndef QN8006Radio_h
#define QN8006Radio_h


class QN8006Radio
{
private:
	int _address;
  int rssi;

public:
  QN8006Radio();
  void setFrequency(float frequency);
  void initReceive(byte s_t, byte mode);
  void mute();
  void unmute();
  void searchUp(unsigned char *stat);
  int getStat(unsigned char *stat);
  
};


#endif