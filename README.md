# QN8006
QN8006 FM module simple library for ARDUINO

As I have received a QN8006 FM module instead of TEA5767 I have developed a simple library in order to use it.
It is far away from perfect but it works at least for my needs.

The pinots are as almoust the same as TEA5767 modules and can be found in the library:

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

The auto search is based on start and stop frequency. There is only one search in this lybrary - UP. From current frequency to the end of range- 108.0 Mhz. If stop frequency is reached then start frequency is set to begining - 88.0 Mhz.

I couldnt make mute to work as it is supposed to be. As a workarround mute just switch between digital and analog output.
Hope tjhis can help people who have got QN8006 and want to use it.
Have fun!
