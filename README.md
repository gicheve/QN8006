# QN8006
QN8006 FM module simple library for ARDUINO

Top view:
+-10--9--8--7--6-+
|  +------+  ++  |
|  | QN   |  ||  |
|  | 8006 |  ||  |
|  +------+  ++  |
+--1--2--3--4--5-+

1  ----> Arduino SDA
2  ----> Arduino SCL
3  ----> Audio in (left channel)
4  ----> Audio in (right channel)
5  ----> +5V
6  ----> GND
7  ----> Audio out (right channel)
8  ----> Audio out (left channel)
9  ---> Antenna TX
10 ---> Antenna RX

The auto search is based on start and stop frequency. 
There is only one search in this library - UP. From current frequency to the end of range- 108.0 Mhz. 
If stop frequency is reached then start frequency is set to beginning - 88.0 Mhz.

I could not make mute to work as it is supposed to be. As a workaround mute just switch between digital and analog output.
Hope this can help people who have got QN8006 and want to use it.
Have fun!

P.S. 
Added transmitter configuration options.
