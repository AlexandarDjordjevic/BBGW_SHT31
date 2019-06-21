#ifndef SHT3X_H
#define SHT3X_H

#include "i2c.h"
#include <stdint.h>


#define POLYNOMIAL  0x31 // Polynomial: 0x31 (x8 + x5 + x4 + 1)

/**************************************************************************
    CONVERSION DELAY (in mS)
**************************************************************************/
    #define SHT31_CONVERSIONDELAY               (100)

/**************************************************************************
    COMMAND DESCRIPTION
**************************************************************************/
    #define SHT31_MEAS_HIGHREP_STRETCH_EN       (0x2C06)    // High repeatability measurement with clock stretching enabled
    #define SHT31_MEAS_MEDREP_STRETCH_EN        (0x2C0D)    // Medium repeatability measurement with clock stretching enabled
    #define SHT31_MEAS_LOWREP_STRETCH_EN        (0x2C10)    // Low repeatability measurement with clock stretching enabled
    #define SHT31_MEAS_HIGHREP_STRETCH_DIS      (0x2400)    // High repeatability measurement with clock stretching disabled
    #define SHT31_MEAS_MEDREP_STRETCH_DIS       (0x240B)    // Medium repeatability measurement with clock stretching disabled
    #define SHT31_MEAS_LOWREP_STRETCH_DIS       (0x2416)    // Low repeatability measurement with clock stretching disabled
    #define SHT31_CMD_READSTATUS                (0xF32D)    // Command to read out the status register
    #define SHT31_CMD_CLEARSTATUS               (0x3041)    // Command to clear the status register
    #define SHT31_CMD_SOFTRESET                 (0x30A2)    // Soft reset command
    #define SHT31_CMD_HEATERENABLE              (0x306D)    // Heater enable command
    #define SHT31_CMD_HEATERDISABLE             (0x3066)    // Heater disable command

class SHT3x
{
public:
    typedef enum {
        CMD_READ_SERIAL_NUMBER = 0x3780,

        CMD_READ_STATUS = 0xF32D,
        CMD_CLEAR_STATUS = 0x3041,

        CMD_HEATER_ENABLE = 0x306D,
        CMD_HEATER_DISABLE = 0x3066,

        CMD_SOFT_RESET = 0x30A2,

        CMD_CLOCK_STRETCH_H = 0x2C06,
        CMD_CLOCK_STRETCH_M = 0x2C0D,
        CMD_CLOCK_STRETCH_L = 0x2C10,

        CMD_POLLING_H = 0x2400,
        CMD_POLLING_M = 0x240B,
        CMD_POLLING_L = 0x2416,

        CMD_ART = 0x2B32,

        CMD_PERIODIC_HALF_H = 0x2032,
        CMD_PERIODIC_HALF_M = 0x2024,
        CMD_PERIODIC_HALF_L = 0x202F,
        CMD_PERIODIC_1_H = 0x2130,
        CMD_PERIODIC_1_M = 0x2126,
        CMD_PERIODIC_1_L = 0x212D,
        CMD_PERIODIC_2_H = 0x2236,
        CMD_PERIODIC_2_M = 0x2220,
        CMD_PERIODIC_2_L = 0x222B,
        CMD_PERIODIC_4_H = 0x2334,
        CMD_PERIODIC_4_M = 0x2322,
        CMD_PERIODIC_4_L = 0x2329,
        CMD_PERIODIC_10_H = 0x2737,
        CMD_PERIODIC_10_M = 0x2721,
        CMD_PERIODIC_10_L = 0x272A,

        CMD_FETCH_DATA = 0xE000,
        CMD_STOP_PERIODIC = 0x3093,

        CMD_READ_ALR_LIMIT_LS = 0xE102,
        CMD_READ_ALR_LIMIT_LC = 0xE109,
        CMD_READ_ALR_LIMIT_HS = 0xE11F,
        CMD_READ_ALR_LIMIT_HC = 0xE114,

        CMD_WRITE_ALR_LIMIT_HS = 0x611D,
        CMD_WRITE_ALR_LIMIT_HC = 0x6116,
        CMD_WRITE_ALR_LIMIT_LC = 0x610B,
        CMD_WRITE_ALR_LIMIT_LS = 0x6100,

        CMD_NO_SLEEP = 0x303E
    }Commands;

    typedef enum {
        REPEATABILITY_HIGH,
        REPEATABILITY_MEDIUM,
        REPEATABILITY_LOW
    }Repeatability;

    typedef enum {
        MODE_CLOCK_STRETCH,
        MODE_POLLING
    }Mode;

    typedef enum {
        FREQUENCY_HZ5,
        FREQUENCY_1HZ,
        FREQUENCY_2HZ,
        FREQUENCY_4HZ,
        FREQUENCY_10HZ
    }Frequency;


    SHT3x(string bus, int bus_id, int address);
    ~SHT3x();
    void        init();
    int         readStatus();
    int         clearStatus();
    int         heaterON();
    int         heaterOFF();
    int         softReset();
    int         startPeriodicMeasurement(Repeatability repeatability, Frequency frequency);
    int         periodicFetchData();
    float       temperature;
    float       humidity;

private:
    int         address;
    I2C*        I2CBus;
    uint16_t    status;


    int         readTemperatureAndHumidity();
    uint8_t     calculateCRC(uint8_t* data, size_t count);
    bool        checkCrc(uint8_t* data, size_t count, uint8_t checksum);
    void        prepareCommand(uint8_t * buffer, Commands comand);
    float       calculateTemperature(uint16_t);
    float       calculateHumidity(uint16_t);

};

#endif // SHT3X_H
