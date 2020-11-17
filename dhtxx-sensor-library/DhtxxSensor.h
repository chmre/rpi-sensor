#ifndef DHTXXSENSOR_H
#define DHTXXSENSOR_H

#include <wiringPi.h>
#include <chrono>
#include <thread>
#include <string.h>

#include <DhtxxSensorData.h>

#define HIGH_TIME 32

#define SENSOR_TYPE_DHT11 1
#define SENSOR_TYPE_DHT22 2

namespace dht
{

    static std::chrono::milliseconds hostReq(18);
    static std::chrono::microseconds sigWait(10);
    static std::chrono::microseconds slaveAck(80);
    static std::chrono::microseconds slaveLow(50);
    static std::chrono::microseconds slaveVal0(28);
    static std::chrono::microseconds slaveVal1(70);


       
class DhtxxSensor
{

    private:
        char mName[20];
        int mSensorType;
        int mPinNbr;
        unsigned char mRawCrc;
        unsigned long mRawData;
        bool mTaskRunning = false;
        std::thread *pReadThread;
        
        int mResponseWait;
        int mStartWait;
        DhtxxSensorData *mSensorData;

        int readSensor();
        int testReadSensor();

public:
    DhtxxSensor(int wiringPiPinNumber, char sensorName[20], int sensorType);
    ~DhtxxSensor();
    DhtxxSensorData * getSensorData();

};

}

#endif // DHTXXSENSOR_H
