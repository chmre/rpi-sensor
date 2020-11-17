
#ifndef DHTXXSENSORDATA_H
#define DHTXXSENSORDATA_H

#include <chrono>
#include <string.h>
#include <iostream>

#define DHTxx_VALID 0
#define DHTxx_RESPONSE_TIMEOUT -1
#define DHTxx_RESPONSE_FAIL -2
#define DHTxx_CRC_ERROR -3

using namespace std;
namespace dht

{

class DhtxxSensorData
{
private:
    unsigned char mRawCrc;
    unsigned long mRawData;

    char mName[20];
    int mSensorType;
    float mHumidity;
    float mTemperatur;
    time_t mValueTimestamp;

    int mStatusCode;
    char mStatusMsg[50];
    
public:
    DhtxxSensorData(char sensorName[20], int sensorType);
    DhtxxSensorData(unsigned char rawCrc, unsigned long rawData, int receiveStatus, char sensorName[20], int sensorType);
    ~DhtxxSensorData();
    float getHumidity();
    float getTemperatur();
    int getStatusCode();
    char * getName();
    char * getStatusMessage();
    string toString();
    void printRawValue();
    std::tm * getUtcValueTimestamp();
};

}

#endif // DHTXXSENSORDATA_H
