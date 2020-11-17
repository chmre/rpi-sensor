#include <ctime>

#include "DhtxxSensorData.h"
#include "DhtxxSensor.h"

dht::DhtxxSensorData::DhtxxSensorData(char sensorName[20], int sensorType)
{
    mValueTimestamp = std::time(nullptr);
    mRawCrc = 0;
    mRawData = 0;
    strcpy(mName, sensorName);    
    mSensorType = (sensorType == SENSOR_TYPE_DHT22) ? SENSOR_TYPE_DHT22 : SENSOR_TYPE_DHT11;
    mHumidity = 0;
    mTemperatur = 0;
    mStatusCode = DHTxx_RESPONSE_TIMEOUT;
    strcpy(mStatusMsg, "No readout done");
}

dht::DhtxxSensorData::DhtxxSensorData(unsigned char rawCrc, unsigned long  rawData, int receiveStatus, char sensorName[20], int sensorType)
{
    mValueTimestamp = std::time(nullptr);

    mRawCrc = rawCrc;
    mRawData = rawData;

    strcpy(mName, sensorName);
    mSensorType = (sensorType == SENSOR_TYPE_DHT22) ? SENSOR_TYPE_DHT22 : SENSOR_TYPE_DHT11;

    if(receiveStatus == DHTxx_VALID)
    {
        unsigned char hi = (mRawData >> 24) & 0xff;
        unsigned char hf = (mRawData >> 16) & 0xff;
        unsigned char ti = (mRawData >> 8) & 0xff;
        unsigned char tf = mRawData & 0xff;
        unsigned char myCrc = (hi + hf + ti + tf) & 0xff;
        
        if(myCrc == mRawCrc) {
            mStatusCode = DHTxx_VALID;
            if(mSensorType == SENSOR_TYPE_DHT11)
            {
                mHumidity =  float(hi * 10 + hf) / 10;
                mTemperatur = float(ti * 10 +tf) / 10;
            }
            else
            {
                mHumidity =  float((hi<<8) + hf) / 10;
                mTemperatur = float((ti<<8) +tf) / 10;
            }
        }
        else
        {
            mStatusCode = DHTxx_CRC_ERROR;
            mHumidity = 0;
            mTemperatur = 0;
        }
    }
    else
    {
        mStatusCode = receiveStatus;
    }

    switch(mStatusCode)
    {
        case DHTxx_VALID:
            strcpy(mStatusMsg, "Sensor data valid");
            break;
        case DHTxx_CRC_ERROR:
            strcpy(mStatusMsg, "Sensor data CRC error");
            break;
        case DHTxx_RESPONSE_FAIL:
            strcpy(mStatusMsg, "Sensor did not send start bit");
            break;
        case DHTxx_RESPONSE_TIMEOUT:
            strcpy(mStatusMsg, "Sensor response timeout");
            break;
        default:
            strcpy(mStatusMsg, "Sensor unknown error");
            break;
    }
}

dht::DhtxxSensorData::~DhtxxSensorData()
{
}


float dht::DhtxxSensorData::getHumidity()
{
    return mHumidity;
}

float dht::DhtxxSensorData::getTemperatur()
{
    return mTemperatur;
}


string dht::DhtxxSensorData::toString()
{
    string str = "Sensor ";
    str.append(mName);
    str.append(": ");
    
    if(mStatusCode != DHTxx_VALID)
    {
        str.append(mStatusMsg);
    }
    else
    {
        str.append(" -> ");
        str.append(" T: ");
        str += to_string(mTemperatur);
        str.append("°C H: ");
        str += to_string(mHumidity);
        str.append("%");
    }
    return str;
}


void dht::DhtxxSensorData::printRawValue()
{
    char date[32];
    std::tm *ptm =std::gmtime(&mValueTimestamp);
    std::strftime(date, sizeof(date), "%Y-%m-%d %H:%M:%s", ptm);
    unsigned char hi = (mRawData >> 24) & 0xff;
    unsigned char hf = (mRawData >> 16) & 0xff;
    unsigned char ti = (mRawData >> 8) & 0xff;
    unsigned char tf = mRawData & 0xff;
    unsigned char ck = (hi + hf + ti + tf) & 0xff;
    
    printf("Sensor %s raw values\n", mName);
    printf("Value time: %s\n", date);
    printf("  Hi: %x, (%d)\n", hi, hi);
    printf("  Hf: %x, (%d)\n", hf, hf);
    printf("  Ti: %x, (%d)\n", ti, ti);
    printf("  Tf: %x, (%d)\n", tf, tf);
    printf("  CRC: r:%x, c:%x\n", mRawCrc, ck);
    
    fflush(stdout);
}

int dht::DhtxxSensorData::getStatusCode()
{
    return mStatusCode;
}

char * dht::DhtxxSensorData::getStatusMessage()
{
    return mStatusMsg;
}


char* dht::DhtxxSensorData::getName()
{
    return mName;
}

std::tm * dht::DhtxxSensorData::getUtcValueTimestamp()
{
    return std::gmtime(&mValueTimestamp);
}
