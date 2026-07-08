#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"

#include <MPU6050.h>

// I2C0 on GPIO8 (SDA) and GPIO9 (SCL) running at 40KHz.
#define I2C_PORT i2c0
#define I2C_SDA 8
#define I2C_SCL 9
static void init_i2c(void);



int main(){
    stdio_init_all();
    init_i2c();

    sleep_ms(2000); // gives USB serial time to connect

    int mpu6050_init_response = mpu6050_init(I2C_PORT);
    printf("MPU Init Response: %s (%d)\n", MPU6050_STATUS_STRINGS[mpu6050_init_response], mpu6050_init_response);

    while (true) {
        uint8_t who_am_i = 0;
        int who_am_i_status = mpu6050_read_who_am_i(I2C_PORT, &who_am_i);

        if (who_am_i_status != MPU6050_OK) {
            printf("WHO_AM_I read error: %s (%d)\n", MPU6050_STATUS_STRINGS[who_am_i_status], who_am_i_status);
        } else {
            printf("WHO_AM_I = 0x%02X (%d)\n", who_am_i, who_am_i);
        }

        uint8_t smplrate = 0;
        int smplrate_status = mpu6050_read_smplrate(I2C_PORT, &smplrate);

        if (smplrate_status != MPU6050_OK) 
            printf("MPU6050 Sample Rate read error: %s (%d)\n", MPU6050_STATUS_STRINGS[smplrate_status], smplrate_status);
        else{
            printf("MPU6050 Sample Rate Setting: %d\n", smplrate);
        
        }
    
        int16_t mpu_temp = 0.0f;
        int mpu_temp_status = mpu6050_read_temp(I2C_PORT, &mpu_temp);

        if (mpu_temp_status != MPU6050_OK) 
            printf("MPU6050 Temp read error: %s (%d)\n", MPU6050_STATUS_STRINGS[mpu_temp_status], mpu_temp_status);
        else{
            printf("MPU6050 Temp: %.2f\n", mpu_temp);
        
        }

        int16_t mpu_raw_temp = 0;

        int raw_mpu_temp_status = mpu6050_read_temp_raw(I2C_PORT, &mpu_raw_temp);

        if (raw_mpu_temp_status != MPU6050_OK) 
            printf("MPU6050 Raw Temp read error: %s (%d)\n", MPU6050_STATUS_STRINGS[raw_mpu_temp_status], raw_mpu_temp_status);
        else{
            printf("MPU6050 Raw Temp: %d\n", mpu_raw_temp);
        
        }
    
        sleep_ms(1000);
    }
}



static void init_i2c(void){
    i2c_init(I2C_PORT, 40*1000);
    
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);

}