#include "sht3x.h"
#include <iostream>
#include <stdio.h>
#include <string.h>
using namespace std;

SHT3x::SHT3x(string bus, int bus_id, int address)
{
    this->I2CBus = new I2C(bus, bus_id);
    this->address = address;

    this->I2CBus->Open();
    this->I2CBus->SetSlaveAddress(this->address);
    cout << "SHT31 Address: " << this->address << endl;
}

SHT3x::~SHT3x()
{
    delete this->I2CBus;
}

void SHT3x::init()
{
    cout << "SHT31 Init\n";

}

uint8_t SHT3x::calculateCRC(uint8_t *data, size_t count)
{
    uint8_t CRC = 0xFF;

    for (size_t j = count; j; --j )
    {
        CRC ^= *data++;
        for ( int i = 8; i; --i )
        {
            CRC = ( CRC & 0x80 ) ? uint8_t(CRC << 1) ^ POLYNOMIAL : uint8_t(CRC << 1);
        }
    }
    return CRC;
}

bool SHT3x::checkCrc(uint8_t* data, size_t count, uint8_t checksum)
{
    return calculateCRC(data, count) == checksum;
}


void SHT3x::prepareCommand(uint8_t * buffer, Commands command)
{
    buffer[0] = uint8_t (command / 256);
    buffer[1] = uint8_t (command % 256);
}

int SHT3x::startPeriodicMeasurement(Repeatability repeatability, Frequency frequency)
{
    uint8_t command[2];
    switch (repeatability)
    {
        case REPEATABILITY_LOW:
            switch (frequency)
            {
            case FREQUENCY_HZ5:
                prepareCommand(command, CMD_PERIODIC_HALF_L);
                break;
            case FREQUENCY_1HZ:
                prepareCommand(command, CMD_PERIODIC_1_L);
                break;
            case FREQUENCY_2HZ:
                prepareCommand(command, CMD_PERIODIC_2_L);
                break;
            case FREQUENCY_4HZ:
                prepareCommand(command, CMD_PERIODIC_4_L);
                break;
            case FREQUENCY_10HZ:
                prepareCommand(command, CMD_PERIODIC_10_L);
                break;
            }
            break;
        case REPEATABILITY_MEDIUM:
            switch (frequency)
            {
            case FREQUENCY_HZ5:
                prepareCommand(command, CMD_PERIODIC_HALF_M);
                break;
            case FREQUENCY_1HZ:
                prepareCommand(command, CMD_PERIODIC_1_M);
                break;
            case FREQUENCY_2HZ:
                prepareCommand(command, CMD_PERIODIC_2_M);
                break;
            case FREQUENCY_4HZ:
                prepareCommand(command, CMD_PERIODIC_4_M);
                break;
            case FREQUENCY_10HZ:
                prepareCommand(command, CMD_PERIODIC_10_M);
                break;
            }
            break;

        case REPEATABILITY_HIGH:
            switch (frequency)
            {
            case FREQUENCY_HZ5:
                prepareCommand(command, CMD_PERIODIC_HALF_H);
                break;
            case FREQUENCY_1HZ:
                prepareCommand(command, CMD_PERIODIC_1_H);
                break;
            case FREQUENCY_2HZ:
                prepareCommand(command, CMD_PERIODIC_2_H);
                break;
            case FREQUENCY_4HZ:
                prepareCommand(command, CMD_PERIODIC_4_H);
                break;
            case FREQUENCY_10HZ:
                prepareCommand(command, CMD_PERIODIC_10_H);
                break;
            }
            break;
    }
    return this->I2CBus->Write(command, 2);
}

int SHT3x::readStatus()
{
    uint8_t data[3] = {0xF3, 0x2D, 0};

    if(this->I2CBus->Write(data, 2) < 0)
    {
        cout << "Fail to write Status comand\n";
        return -1;
    }

    if(this->I2CBus->Read(data, 3) < 0)
    {
        cout << "Fail to read Status data\n";
        return -1;
    }

    if(!checkCrc(data, 2, data[2]))
    {
        cout << "Bad CRC\n";
        return -1;
    }
    uint16_t status = uint16_t(data[0] << 8 | data[1]);
    cout << "Status: " << status << endl;
    return 0;
}

int SHT3x::clearStatus()
{
    uint8_t data[2] = {0x30, 0x41};

    if(this->I2CBus->Write(data, 2) < 0)
    {
        cout << "Fail to Clear Status Register\n";
        return -1;
    }
    return 0;
}

int SHT3x::heaterON()
{
    uint8_t data[2];

    prepareCommand(data, CMD_HEATER_ENABLE);

    if(this->I2CBus->Write(data, 2) < 0)
    {
        cout << "Fail to Turn ON Heater\n";
        return -1;
    }
    return 0;
}

int SHT3x::heaterOFF()
{
    uint8_t data[2];

    prepareCommand(data, CMD_HEATER_DISABLE);

    if(this->I2CBus->Write(data, 2) < 0)
    {
        cout << "Fail to Turn OFF Heater\n";
        return -1;
    }
    return 0;
}

int SHT3x::softReset()
{
    uint8_t data[2];

    prepareCommand(data, CMD_SOFT_RESET);

    if(this->I2CBus->Write(data, 2) < 0)
    {
        cout << "SHT3x Fail to reset\n";
        return -1;
    }
    return 0;
}

float SHT3x::calculateTemperature(uint16_t rawValue)
{
    return 175.0f * rawValue / 65535.0f - 45.0f;
}

float SHT3x::calculateHumidity(uint16_t rawValue)
{
    return 100.0f * rawValue / 65535.0f;
}

int SHT3x::periodicFetchData()
{
    uint8_t data[2];
    prepareCommand(data, CMD_FETCH_DATA);
    if(this->I2CBus->Write(data, 2) < 0)
    {
        cout << "Fail to write command: CMD_FETCH_DATA\n";
        return -1;
    }
    return readTemperatureAndHumidity();
}

int SHT3x::readTemperatureAndHumidity()
{

    uint8_t buf[4];

    uint16_t tempRaw, rhRaw;

    if (this->I2CBus->Read(buf, 4) < 0 )
    {
        cout << "Fail to read temperature and humidity\n";
        return -1;
    }

    tempRaw = buf[0] * 256 + buf[1];
    rhRaw = buf[2] * 256 + buf[3];

    this->temperature = calculateTemperature(tempRaw);
    this->humidity = calculateHumidity(rhRaw);

    return 0;
}
