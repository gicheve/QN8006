// QN8006 I2C FM Radio Library

#include <Wire.h>

#ifndef QN8006Radio_h
#define QN8006Radio_h


class QN8006Radio
{
private:
    int _address;
    int rssi;

    void sendData(byte data[]);

public:
    QN8006Radio();

    //receiver settings
    void initReceive(byte s_t, byte mode);
    void mute();
    void unmute();
    void searchUp(unsigned char *stat);

    // transmitter settings
    void initTransmit(byte mode);
    void setOutputPower(byte output_power);
    void setTxFrequencyDeviation(byte tx_fedv);

    // common settings
    void resetDevice();
    void setFrequency(float frequency);
    void setCrystalCapLoad(byte cap_load);
    int getStat(unsigned char *stat);
};

#endif