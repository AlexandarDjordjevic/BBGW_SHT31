#ifndef I2C_H
#define I2C_H

#include <iostream>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdint.h>

using namespace std;

class I2C
{
    string bus;
    int bus_id;
    int file;
public:
    I2C(string, int);
    int Open();
    int SetSlaveAddress(int address);
    int Read(uint8_t* buffer, size_t count);
    int Write(uint8_t *buffer, size_t count);


};

#endif // I2C_H
