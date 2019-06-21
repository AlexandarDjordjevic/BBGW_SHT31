#include "i2c.h"

I2C::I2C(string bus, int bus_id)
{
    this->bus = bus;
    this->bus_id = bus_id;
    cout << "I2C Created\n";
}

int I2C::Open()
{
    this->file = open(this->bus.c_str(), O_RDWR);
    if (file < 0) {
        cout<< "Failed to open the i2c bus\n";
        return -1;
    }
    else {
        cout << "I2C Bus opened!\n";
    }
    return 0;
}

int I2C::SetSlaveAddress(int address)
{
    if(ioctl(file, I2C_SLAVE, address) < 0)
    {
        cout << "Fail to set slave address\n";
        return -1;
    }
    return 0;
}

int I2C::Write(uint8_t *buffer, size_t count)
{
    if(size_t (write(this->file, buffer, count)) != count)
    {
        cout << "Fail to write data\n";
        return -1;
    }
    return 0;
}

int I2C::Read(uint8_t *buffer, size_t count)
{
    if(size_t (read(this->file, buffer, count)) != count)
    {
        return -1;
    }
    return 0;
}


