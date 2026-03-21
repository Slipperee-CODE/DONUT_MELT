#include <stdio.h>
#include "H3LIS331DL.h"

static i2c_inst_t* I2C_PORT = NULL;

static double curr_readings[3];

void write_to_register(uint8_t register_address, uint8_t register_value){
    uint8_t buffer[2] = {register_address, register_value};
    i2c_write_blocking(I2C_PORT, ACCELEROMETER_ADDRESS, buffer, 2, false);
}

void accelerometer_init(i2c_inst_t* i2c_port, uint8_t i2c_sda, uint8_t i2c_scl){
    I2C_PORT = i2c_port;

    curr_readings[0] = 0;
    curr_readings[1] = 0;
    curr_readings[2] = 0;

    i2c_init(I2C_PORT, 400*1000);

    gpio_set_function(i2c_sda, GPIO_FUNC_I2C);
    gpio_set_function(i2c_scl, GPIO_FUNC_I2C);

    gpio_pull_up(i2c_sda); //I2C pins need to be HIGH by default
    gpio_pull_up(i2c_scl);

    write_to_register(CTRL_REG1_REGISTER_ADDRESS, CTRL_REG1_REGISTER_VALUE); //Powers on accelerometer + sets output rate
    write_to_register(CTRL_REG4_REGISTER_ADDRESS, CTRL_REG4_REGISTER_VALUE); //Updates block data update type, endianness, and accel scale 
}

double* accelerometer_get_all_axis(){ //Maybe make a struct to store/access accelerometer data more easily 
    if (!is_data_ready_to_be_read()){
        return curr_readings;
    }

    uint8_t firstRegister = (1 << 7) | LSB_X_REGISTER_ADDRESS; //MSB is 1 for auto-increment during register reads
    i2c_write_blocking(I2C_PORT, ACCELEROMETER_ADDRESS, &firstRegister, 1, true); //pretty sure this function handles setting the LSB of ACCELEROMETER_ADDRESS to 1 for write bc all I2C addresses are 7 bits 
    uint8_t buffer[6]; //need enough space for MSB register and LSB register of each axis
    i2c_read_blocking(I2C_PORT, ACCELEROMETER_ADDRESS, buffer, 6, false); //same here with the read bit of the address, data stored in buffer 
    uint16_t* collapsed_buffer = (uint16_t*) buffer; //array decays into pointer to first element which I cast to a uint16_t pointer
    static double results[3];
    for (int i = 0; i < 3; ++i){
        int16_t raw = (int16_t)collapsed_buffer[i] >> 4; // CLAUDE SAID TO DO THIS PART - Cai
        results[i] = raw * ACCEL_SENSITIVITY / 1000.0; //divided by 1000.0 for gs + makes it a double
    }

    curr_readings[0] = results[0];
    curr_readings[1] = results[1];
    curr_readings[2] = results[2];

    return curr_readings;
}

// THIS FUNCTION IS PURELY FROM CLAUDE - Cai
bool is_data_ready_to_be_read()
{
    uint8_t status_reg = 0x27;
    uint8_t status;
    i2c_write_blocking(I2C_PORT, ACCELEROMETER_ADDRESS, &status_reg, 1, true);
    i2c_read_blocking(I2C_PORT, ACCELEROMETER_ADDRESS, &status, 1, false);
    if (status & 0x08) { // ZYXDA bit - all axes have new data
    // read axis data
        return true;
    }
    return false;
}

double accelerometer_get_x(){
    accelerometer_get_all_axis();
    return curr_readings[0];
}

double accelerometer_get_y(){
    accelerometer_get_all_axis();
    return curr_readings[1];
}

double accelerometer_get_z(){
    accelerometer_get_all_axis();
    return curr_readings[2];
}

void pico_H3LIS331DL_is_library_accesible(){
    printf("Accessing the pico_H3LIS331DL library is functioning properly! \n");
}