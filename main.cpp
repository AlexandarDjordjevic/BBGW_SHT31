#include <iostream>
#include <chrono>
#include <thread>
#include "sht3x.h"

using namespace std;

int main()
{
    SHT3x SHT31("/dev/i2c-2", 2, 0x45);

    SHT31.init();
    SHT31.readStatus();
    SHT31.startPeriodicMeasurement(SHT31.REPEATABILITY_HIGH, SHT31.FREQUENCY_1HZ);

    for(int i = 0; i < 100; i++)
    {
        sleep(30);
        SHT31.periodicFetchData();
        cout << "**********************************************\nMeasurement: " << i << endl;
        cout << "Temperature: " << SHT31.temperature << endl;
        cout << "Humidity: " << SHT31.humidity << endl;
    }
    return 0;
}
