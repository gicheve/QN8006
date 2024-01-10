// QN8006 I2C FM Radio Library

#include <Arduino.h>
#include <QN8006Radio.h>

QN8006Radio::QN8006Radio() {
    _address = 0x2B; //The default IC address is `0101011`
}


// ------------------------------------- receiver settings -------------------------------------

/*
 * Initiate receive mode
 */
void QN8006Radio::initReceive(byte s_t, byte mode) {
    byte buf_1[4];
    buf_1[0] = 0x00; // Register `SYSTEM1` with address `00h`
    buf_1[1] = 0b10000001; // Enter Receiving mode. CH is determined by the content in CH[9:0].
    if (mode) buf_1[2] = 0b00001011; // Set STEREO & set IDLE to infinity (never go to Standby)
    else buf_1[2] = 0b00011011; // Set MONO & set IDLE to infinity (never go to Standby)
    if (s_t > 31)   buf_1[3] = 0b10101010;  //b7 = 1: RX CCA threshold MSB. See CCA register 19h [4:0].
    else buf_1[3] = 0b00101010;  // b7 = 0: RX CCA threshold MSB. See CCA register 19h [4:0].
    sendData(buf_1);

    byte buf_2[2];
    buf_2[0] = 0x19; // Register `CCA` with address `19h` // start subaddress  CCA threshold
    buf_2[1] = (s_t & 0b00011111) | 0b01000000;
    sendData(buf_2);
}

/*
 * Mute
 */
void QN8006Radio::mute() {
    byte buf[2];
    buf[0] = 0x00; // Register `SYSTEM1` with address `00h`
    buf[1] =0b10001000;
    sendData(buf);
    //Serial.println("mute");
}

/*
 * Unmute
 */
void QN8006Radio::unmute() {
    byte buf[2];
    buf[0] = 0x00; // Register `SYSTEM1` with address `00h`
    buf[1] = 0b10000001;
    sendData(buf);
    //Serial.println("unmute");
}

void QN8006Radio::searchUp(unsigned char *stat){
    byte buf[5];
    //Serial.println("searching...");
    if (QN8006Radio::getStat (stat)) {
        // set start frequency to current /beginning
        buf[0] = 0x09; // start subaddress
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

        // set stop frequency to 108 Mhz
        buf[2] = 0b10000000;
        buf[3]|= 0b01100000;
        //Serial.println (buf[1]);
        //Serial.println (buf[3]);

        // send start/stop settings
        Wire.beginTransmission(_address);
        Wire.write(buf,4); //send data
        Wire.endTransmission();

        // start searching
        buf[0] = 0x00; // start subaddress
        buf[1] = 0b10100000;
        Wire.beginTransmission(_address);
        Wire.write(buf,2); // send data
        Wire.endTransmission();
    }
}


// ------------------------------------- transmitter settings ----------------------------------

/*
 * Initiate transmit mode
 */
void QN8006Radio::initTransmit(byte mode) {
    byte buf[3];
    buf[0] = 0x00; // Register `SYSTEM1` with address `00h`
    buf[1] = 0b01000001; // Enter Transmitting mode.
    if (mode) buf[2] = 0b00001011; // Set STEREO & set IDLE to infinity (never go to Standby)
    else buf[2] = 0b00011011; // Set MONO & set IDLE to infinity (never go to Standby)
    sendData(buf);
}

/*
 * Set transmit output power
 */
void QN8006Radio::setOutputPower(byte output_power) {
    byte buf[2];
    buf[0] = 0x0c; // Register `PAC_TARGET` with address `0ch`. Output power calibration control
    buf[1] = output_power;
    sendData(buf);
}
/*
 * Specify total TX frequency deviation: TX frequency deviation = 0.69 kHz*TX_FEDV. Default 108.
 */
void QN8006Radio::setTxFrequencyDeviation(byte tx_fedv) {
    byte buf[2];
    buf[0] = 0x0e; // Register `TX_FDEV` with address `0eh`
    buf[1] = tx_fedv;
    sendData(buf);
}


// ------------------------------------- common settings ---------------------------------------

/*
 * Reset all registers to default values.
 */
void QN8006Radio::resetDevice() {
    byte buf[2];
    buf[0] = 0x01; // Register `SYSTEM2` with address `01h`
    buf[1] = 0b10001001;
    sendData(buf);
}

/*
 * Set TX/RX Frequency
 */
void QN8006Radio::setFrequency(float frequency) {
    byte buf[2];
    unsigned int frequencyB = (frequency*100 - 7600)/5;
    byte frequencyH = frequencyB >> 8;
    byte frequencyL = frequencyB & 0XFF;
    buf[0] = 0x08; // Register `CH` with address `08h` (Lower 8 bits of 10-bit channel index.)
    buf[1] = frequencyL;
    sendData(buf);
    buf[0] = 0x0b; // Register `CH_STEP` with address `0bh` (Highest 2 bits of channel indexes.)
    buf[1] = frequencyH;
    sendData(buf);
}

/*
 * Crystal cap load setting for correct xtal frequency
 * The loading cap on each side is: 10+XCSEL*0.32 pF, i.e. it ranges from 10pF to 30pF. Default is 20 pF
 */
void QN8006Radio::setCrystalCapLoad(byte cap_load) {
    if (cap_load > 0b00111111) {
        cap_load = 0b00111111;
    }
    byte buf[2];
    buf[0] = 0x04; // Register `REG_VGA` with address `04h`
    buf[1] = cap_load + 0b01000000;
    sendData(buf);
}

/*
 * Get statistic from QN8006
 */
int QN8006Radio::getStat(unsigned char *stat) {
    unsigned char buf[32];
    memset (stat, 0, 5);
    Wire.requestFrom (_address, 32);    // reading QN8006
    //while(Wire.available() < 1);
    for (int i = 0; i < 32; i++) {
        buf[i] = Wire.read ();
        //Serial.println(buf[i]);
    }
    stat[0] = buf[10];  // current frequency in steps b0-b7
    stat[1] = buf [13] & 0b00000011; // cuirrent frequensy in steps b8-b9
    stat[2] = buf[30] - 40; //RSSI
    stat[3] = buf[28] & 0x01; // 1= mono ; 0 = stereo
    stat[4] = (buf[28] & 0b01000000) >> 6; // auto tuning chanel found = 0; not found = 1
    stat[5] = (buf[2] & 0b00100000) >> 5; // autoscan in progress = 1; completed = 0
    return 1;
}


// ------------------------------------- common private func -----------------------------------

/*
 * Send data to user control registers:
 */
void QN8006Radio::sendData(byte data[]) {
    int dataSize = sizeof(data);
    Wire.beginTransmission(_address);
    Wire.write(data, dataSize); // send data
    Wire.endTransmission();
}




















