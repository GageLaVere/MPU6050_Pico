#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"

#include <MPU6050.h>

// I2C0 on GPIO8 (SDA) and GPIO9 (SCL) running at 40KHz.
#define I2C_PORT i2c0
#define I2C_SDA 8
#define I2C_SCL 9

static void init_i2c(void);

int main(void){
    stdio_init_all();
    init_i2c();

    sleep_ms(2000); // gives USB serial time to connect

    
    //START MPU_INIT


    int mpu6050_init_response = mpu6050_init(I2C_PORT);
    printf("MPU Init Response: %s (%d)\n",\
         MPU6050_STATUS_STRINGS[mpu6050_init_response], mpu6050_init_response);

    //START CONFIG

    int accel_range_status = mpu6050_set_accel_range(
        I2C_PORT,
        MPU6050_ACCEL_RANGE_8G
    );

    if (accel_range_status != MPU6050_OK) {
        printf("MPU6050 accel range set error: %s (%d)\n",
            MPU6050_STATUS_STRINGS[accel_range_status],
            accel_range_status);
    } else {
        printf("Accelerometer range set to +/-8G\n");
    }
    
    //START MOTION DETECTION

    printf("Detecting motion...\n");

    uint8_t thres_to_set = 1;
    int thres_status = mpu6050_set_mot_thres(I2C_PORT, thres_to_set);

    if (thres_status != MPU6050_OK){
            printf("MPU6050 motion threshold set error: %s (%d)\n",
            MPU6050_STATUS_STRINGS[thres_status],
            thres_status);
    } else{
        printf("Motion threshold set to: %d\n", thres_to_set);
    }
    
    uint8_t thres_read_reg = 0;

    int read_thres_status = mpu6050_read_mot_thres(I2C_PORT, &thres_read_reg);

    if (read_thres_status != MPU6050_OK) {
        printf("MPU6050 motion threshold read error: %s (%d)\n",
            MPU6050_STATUS_STRINGS[read_thres_status],
            read_thres_status);
    } else {
        printf("Motion threshold read: %d\n", thres_read_reg);
    }
    


    //START WHILE_LOOP


    while (true) {

        //START WHO_AM_I

        uint8_t who_am_i = 0;
        int who_am_i_status = mpu6050_read_who_am_i(I2C_PORT, &who_am_i);

        if (who_am_i_status != MPU6050_OK) {
            printf("WHO_AM_I read error: %s (%d)\n",\
                 MPU6050_STATUS_STRINGS[who_am_i_status], who_am_i_status);
        } else {
            printf("WHO_AM_I = 0x%02X (%d)\n", who_am_i, who_am_i);
        }

        //START SAMPLE RATE

        uint8_t smplrate = 0;
        int smplrate_status = mpu6050_read_smplrate(I2C_PORT, &smplrate);

        if (smplrate_status != MPU6050_OK) 
            printf("MPU6050 Sample Rate read error: %s (%d)\n",\
                 MPU6050_STATUS_STRINGS[smplrate_status], smplrate_status);
        else{
            printf("MPU6050 Sample Rate Setting: %d\n", smplrate);
        
        }
    

        // START MPU RAW SAMPLE


        mpu6050_sample_raw_t sample = {0};

        int sample_status = mpu6050_read_sample_raw(
            I2C_PORT,
            &sample
        );

        if (sample_status != MPU6050_OK) {
            printf("MPU6050 sample read error: %s (%d)\n",
                MPU6050_STATUS_STRINGS[sample_status],
                sample_status);
        } else {
            float temp_c =
                ((float)sample.temp / 340.0f) + 36.53f;

            printf("Accel raw: X=%d Y=%d Z=%d\n",
                sample.accel.x,
                sample.accel.y,
                sample.accel.z);

            printf("Temp raw: %d, Temp: %.2f C\n",
                sample.temp,
                temp_c);

            printf("Gyro raw: X=%d Y=%d Z=%d\n",
                sample.gyro.x,
                sample.gyro.y,
                sample.gyro.z);
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