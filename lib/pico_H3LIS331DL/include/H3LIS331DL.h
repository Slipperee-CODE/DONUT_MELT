#pragma once

#include <stdint.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"

#define ACCELEROMETER_ADDRESS 0b0011000

#define LSB_X_REGISTER_ADDRESS 0x28
#define MSB_X_REGISTER_ADDRESS 0x29 //Not used but helpful to know

#define LSB_Y_REGISTER_ADDRESS 0x2A //Not used but helpful to know
#define MSB_Y_REGISTER_ADDRESS 0x2B //Not used but helpful to know

#define LSB_Z_REGISTER_ADDRESS 0x2C //Not used but helpful to know
#define MSB_Z_REGISTER_ADDRESS 0x2D //Not used but helpful to know

#define CTRL_REG1_REGISTER_ADDRESS 0x20 //read this register then change just the DATA_OUTPUT PART
#define CTRL_REG4_REGISTER_ADDRESS 0x23 //read this register then change just the full scale selection or big/little endian data selection bc it defalts to little endian which is no bueno or block data update which should be set to 1 to avoid MSB and LSB being read for different g samples (READ DATASHEET FOR THIS INFO)

#define LITTLE_ENDIAN 0b00000000
#define BIG_ENDIAN    0b01000000
#define ENDIANNESS_OF_DATA_SELECTION BIG_ENDIAN

#define CONTINUOUS 0b00000000
#define NO_REGISTER_UPDATE_BETWEEN_LSB_AND_MSB_READS 0b10000000
#define BLOCK_DATA_UPDATE_TYPE NO_REGISTER_UPDATE_BETWEEN_LSB_AND_MSB_READS

#define ACCEL_SCALE_100 0b00000000 //each of these has a correlated sensitivity factor (milli-gs / LSB) in the accelerometer datasheet that needs to be accounted for in reading the data
#define ACCEL_SCALE_200 0b00010000
#define ACCEL_SCALE_400 0b00110000
#define ACCEL_SCALE ACCEL_SCALE_400

//ACCEL_SENSITIVITY in milli-gs / digit
#if ACCEL_SCALE == ACCEL_SCALE_100
    #define ACCEL_SENSITIVITY 49 
#elif ACCEL_SCALE == ACCEL_SCALE_200 
    #define ACCEL_SENSITIVITY 98
#elif ACCEL_SCALE == ACCEL_SCALE_400
    #define ACCEL_SENSITIVITY 195
#endif

#define DATA_OUTPUT_RATE_50   0b00000000
#define DATA_OUTPUT_RATE_100  0b00001000
#define DATA_OUTPUT_RATE_400  0b00010000
#define DATA_OUTPUT_RATE_1000 0b00011000
#define DATA_OUTPUT_RATE DATA_OUTPUT_RATE_1000

#define NORMAL_MODE  0b00100000
#define POWERED_DOWN 0b00000000
#define POWER_MODE NORMAL_MODE

#define CTRL_REG1_REGISTER_VALUE POWER_MODE | DATA_OUTPUT_RATE | 0b00000111 //Last or is to make sure all axis are enabled 
#define CTRL_REG4_REGISTER_VALUE BLOCK_DATA_UPDATE_TYPE | ENDIANNESS_OF_DATA_SELECTION | ACCEL_SCALE //write these values to their appropriate registers right after i2c init

void accelerometer_init(i2c_inst_t* i2c_port, uint8_t i2c_sda, uint8_t i2c_scl);

double* accelerometer_get_all_axis();

double accelerometer_get_x();

double accelerometer_get_y();

double accelerometer_get_z();

void pico_H3LIS331DL_is_library_accesible();