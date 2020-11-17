#include "DhtxxSensor.h"

dht::DhtxxSensor::DhtxxSensor(int wiringPiPinNumber, char sensorName[20], int sensorType)
{
    mSensorType = (sensorType == SENSOR_TYPE_DHT22) ? SENSOR_TYPE_DHT22 : SENSOR_TYPE_DHT11;
    strcpy(mName, sensorName);
    mPinNbr = wiringPiPinNumber;
    mSensorData = new DhtxxSensorData(mName, mSensorType);
}

dht::DhtxxSensor::~DhtxxSensor()
{
}

// works using DHT11
int dht::DhtxxSensor::readSensor()
{
    mRawCrc = 0;
    mRawData = 0;

    int i;
    
    // step 1: host send s a start signal
    // 1.1 set the pin to output and level low  
    pinMode(mPinNbr, OUTPUT);
    digitalWrite(mPinNbr, 0);
    
    // 1.2 keep signal low for 18 mS
    delay(20);
    
    // 1.3 set output signal high
    digitalWrite(mPinNbr, 1);
    
    // 1.4 releas the bit bus
    pinMode(mPinNbr, INPUT);
    pullUpDnControl(mPinNbr, PUD_OFF);

    // step 2 wait for the sensor start signal
    // that is the sensor will issue a low (20uS) followed by a high (80uS)
    // 2.1 wait for the low signal
    delayMicroseconds(40);
    
    // if the signal is 0, the sensor responde
    if(digitalRead(mPinNbr) == 0)
    {
        // 2.2 wait for the begin of the high part of the start sequence
        // if there is no heigh signal then  return a sensor failure (timeout)
        int lowCnt = 0;
        while(digitalRead(mPinNbr) == 0)
        {
            delayMicroseconds(10);
            lowCnt++;
            if(lowCnt == 200)
            {
                pinMode(mPinNbr, OUTPUT);
                digitalWrite(mPinNbr, 1);
                mSensorData = new DhtxxSensorData(mRawCrc, mRawData, DHTxx_RESPONSE_FAIL, mName, mSensorType);
                return 0;
            }
        }
        
        // step 3: read 32 bit data
        for(i=0;i<32;i++)
        {
            // each data bit starts wit a 50uS low folowd by the value (high)
            // dhe high time is 28 uS for 0 and 70uS for 1
            
            // 3.1 wait for the low level
            int lowCnt = 0;
            while(digitalRead(mPinNbr))
            {
                lowCnt++;
                if(lowCnt == 2000)
                {
                    pinMode(mPinNbr, OUTPUT);
                    digitalWrite(mPinNbr, 1);
                    mSensorData = new DhtxxSensorData(mRawCrc, mRawData, DHTxx_RESPONSE_FAIL, mName, mSensorType);
                    return 0;
                }
            }
            
            // 3.2 wait for the high level
            lowCnt = 0;
            while(!digitalRead(mPinNbr))
            {
                lowCnt++;
                if(lowCnt == 2000)
                {
                    pinMode(mPinNbr, OUTPUT);
                    digitalWrite(mPinNbr, 1);
                    mSensorData = new DhtxxSensorData(mRawCrc, mRawData, DHTxx_RESPONSE_FAIL, mName, mSensorType);
                    return 0;
                }
            }
            
            // 3.3 wait slightly longer than the 0 value (>28uS)
            delayMicroseconds(HIGH_TIME);
            
            // shift data buffer
            mRawData*=2;
            
            // 3.4 if the level is high (1) add 1 to the data
            // if it is low (0) noting is to be done as by shifting
            // the data buffer a 0 is automatically added
            if(digitalRead(mPinNbr) == 1)
            {
                mRawData++;
            }
        }
        
        // step 4: read 8 bit CRC
        for(i=0;i<8;i++)
        {
            int lowCnt = 0;
            while(digitalRead(mPinNbr))
            {
                lowCnt++;
                if(lowCnt == 2000)
                {
                    pinMode(mPinNbr, OUTPUT);
                    digitalWrite(mPinNbr, 1);
                    mSensorData = new DhtxxSensorData(mRawCrc, mRawData, DHTxx_RESPONSE_FAIL, mName, mSensorType);
                    return 0;
                }
            }

            lowCnt = 0;
            while(!digitalRead(mPinNbr))
            {
                lowCnt++;
                if(lowCnt == 2000)
                {
                    pinMode(mPinNbr, OUTPUT);
                    digitalWrite(mPinNbr, 1);
                    mSensorData = new DhtxxSensorData(mRawCrc, mRawData, DHTxx_RESPONSE_FAIL, mName, mSensorType);
                    return 0;
                }
            }

            delayMicroseconds(HIGH_TIME);
            mRawCrc*=2;
            if(digitalRead(mPinNbr) == 1)
            {
                mRawCrc++;
            }
        }
        
        // step 5: return the read result
        mSensorData = new DhtxxSensorData(mRawCrc, mRawData, DHTxx_VALID, mName, mSensorType);
        return 1;
    }
    // else the signal is not 0, the sensor failed to answer
    else
    {
        // return the sensor response timeout
        mSensorData = new DhtxxSensorData(mRawCrc, mRawData, DHTxx_RESPONSE_TIMEOUT, mName, mSensorType);
        return 0;
    }
}

int dht::DhtxxSensor::testReadSensor()
{
    mRawCrc = 0;
    mRawData = 0;

    int i;
    
    // step 1: host send s a start signal
    // 1.1 set the pin to output and level low  
    pinMode(mPinNbr, OUTPUT);
    digitalWrite(mPinNbr, 0);
    
    // 1.2 keep signal low for 18 mS
    delay(20);
    
    // 1.3 set output signal high
    digitalWrite(mPinNbr, 1);
    
    // 1.4 releas the bit bus
    pinMode(mPinNbr, INPUT);
    pullUpDnControl(mPinNbr, PUD_OFF);

    // step 2 wait for the sensor start signal
    // that is the sensor will issue a low (20uS) followed by a high (80uS)
    // 2.1 wait for the low signal
    delayMicroseconds(40);
    
    // if the signal is 0, the sensor responde
    if(digitalRead(mPinNbr) == 0)
    {
        // 2.2 wait for the begin of the high part of the start sequence
        // if there is no heigh signal then  return a sensor failure (timeout)
        int lowCnt = 0;
        while(digitalRead(mPinNbr) == 0)
        {
            delayMicroseconds(10);
            lowCnt++;
            if(lowCnt == 200)
            {
                pinMode(mPinNbr, OUTPUT);
                digitalWrite(mPinNbr, 1);
                mSensorData = new DhtxxSensorData(mRawCrc, mRawData, DHTxx_RESPONSE_FAIL, mName, mSensorType);
                return 0;
            }
        }
        
        // step 3: read 32 bit data
        for(i=0;i<32;i++)
        {
            // each data bit starts wit a 50uS low folowd by the value (high)
            // dhe high time is 28 uS for 0 and 70uS for 1
            
            // 3.1 wait for the low level
            int lowCnt = 0;
            while(digitalRead(mPinNbr))
            {
                lowCnt++;
                if(lowCnt == 2000)
                {
                    pinMode(mPinNbr, OUTPUT);
                    digitalWrite(mPinNbr, 1);
                    mSensorData = new DhtxxSensorData(mRawCrc, mRawData, DHTxx_RESPONSE_FAIL, mName, mSensorType);
                    return 0;
                }
            }
            
            // 3.2 wait for the high level
            lowCnt = 0;
            while(!digitalRead(mPinNbr))
            {
                lowCnt++;
                if(lowCnt == 2000)
                {
                    pinMode(mPinNbr, OUTPUT);
                    digitalWrite(mPinNbr, 1);
                    mSensorData = new DhtxxSensorData(mRawCrc, mRawData, DHTxx_RESPONSE_FAIL, mName, mSensorType);
                    return 0;
                }
            }
            
            // 3.3 wait slightly longer than the 0 value (>28uS)
            delayMicroseconds(HIGH_TIME);
            
            // shift data buffer
            mRawData*=2;
            
            // 3.4 if the level is high (1) add 1 to the data
            // if it is low (0) noting is to be done as by shifting
            // the data buffer a 0 is automatically added
            if(digitalRead(mPinNbr) == 1)
            {
                mRawData++;
            }
        }
        
        // step 4: read 8 bit CRC
        for(i=0;i<8;i++)
        {
            int lowCnt = 0;
            while(digitalRead(mPinNbr))
            {
                lowCnt++;
                if(lowCnt == 2000)
                {
                    pinMode(mPinNbr, OUTPUT);
                    digitalWrite(mPinNbr, 1);
                    mSensorData = new DhtxxSensorData(mRawCrc, mRawData, DHTxx_RESPONSE_FAIL, mName, mSensorType);
                    return 0;
                }
            }

            lowCnt = 0;
            while(!digitalRead(mPinNbr))
            {
                lowCnt++;
                if(lowCnt == 2000)
                {
                    pinMode(mPinNbr, OUTPUT);
                    digitalWrite(mPinNbr, 1);
                    mSensorData = new DhtxxSensorData(mRawCrc, mRawData, DHTxx_RESPONSE_FAIL, mName, mSensorType);
                    return 0;
                }
            }

            delayMicroseconds(HIGH_TIME);
            mRawCrc*=2;
            if(digitalRead(mPinNbr) == 1)
            {
                mRawCrc++;
            }
        }
        
        // step 5: return the read result
        mSensorData = new DhtxxSensorData(mRawCrc, mRawData, DHTxx_VALID, mName, mSensorType);
        return 1;
    }
    // else the signal is not 0, the sensor failed to answer
    else
    {
        // return the sensor response timeout
        mSensorData = new DhtxxSensorData(mRawCrc, mRawData, DHTxx_RESPONSE_TIMEOUT, mName, mSensorType);
        return 0;
    }
}


dht::DhtxxSensorData * dht::DhtxxSensor::getSensorData()
{
    testReadSensor();
    return mSensorData;
}

