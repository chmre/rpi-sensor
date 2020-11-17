#include <stdio.h>
#include <wiringPi.h>
#include <chrono>
#include <thread>
#include <string.h>

#include "DhtxxSensor.h"
#include "DhtxxSensorData.h"


static std::chrono::milliseconds _20mS(20);
static std::chrono::milliseconds _100mS(100);
static std::chrono::microseconds _20uS(20);
static std::chrono::microseconds _30uS(30);
static std::chrono::microseconds _120uS(120);
static std::chrono::seconds _1S(1);
static std::chrono::seconds _5S(5);
static std::chrono::seconds _10S(10);
static std::chrono::seconds _20S(20);

using namespace dht;

void printSensorValue(dht::DhtxxSensorData *sensorData);

int main(int argc, char **argv)
{
    int pgmLoopCnt = 0;
    bool pgmRun = 1;
    
    wiringPiSetup();
    
    printf("starting ...\n");
    DhtxxSensor *pSensorList[2];
    pSensorList[0] = new DhtxxSensor(0, "DHT11", SENSOR_TYPE_DHT11);
    pSensorList[1] = new DhtxxSensor(7, "DHT22", SENSOR_TYPE_DHT22);
    
    DhtxxSensorData *sensorData;
    
    while(pgmRun)
    {
        printf("\n= = =  Sensor Readout %d  = = =\n", pgmLoopCnt);

        sensorData = pSensorList[0]->getSensorData();
        printSensorValue(sensorData);

        sensorData = pSensorList[1]->getSensorData();
        printSensorValue(sensorData);

        pgmLoopCnt++;
        std::this_thread::sleep_for(_20S);
    }
    
    printf("good by\n");
	return 0;
}

void printSensorValue(dht::DhtxxSensorData *sensorData)
{
    char date[32];

    std::strftime(date, sizeof(date), "%Y-%m-%d %H:%M:%s", sensorData->getUtcValueTimestamp());
    printf("Sensor %s: UTC: %s", sensorData->getName(), date);
    if(sensorData->getStatusCode() != DHTxx_VALID)
    {
        printf(" %s\n", sensorData->getStatusMessage());
    }
    else
    {
        printf(" -> T:%f°C  H:%f%%\n", sensorData->getTemperatur(), sensorData->getHumidity());
    }    
}