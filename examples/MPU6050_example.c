#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"

#include <MPU6050.h>

// I2C0 on GPIO8 (SDA) and GPIO9 (SCL) running at 40 kHz.
#define I2C_PORT i2c0
#define I2C_SDA  8
#define I2C_SCL  9

static void init_i2c(void);

int main(void) {
    stdio_init_all();
    init_i2c();

    sleep_ms(2000); // Gives USB serial time to connect.

    // START MPU INITIALIZATION

    int status = mpu6050_init(I2C_PORT);

    printf(
        "MPU Init Response: %s (%d)\n",
        MPU6050_STATUS_STRINGS[status],
        status
    );

    if (status != MPU6050_OK) {
        return 1;
    }

    // START DEVICE ID CHECK

    uint8_t who_am_i = 0;

    status = mpu6050_read_who_am_i(
        I2C_PORT,
        &who_am_i
    );

    if (status != MPU6050_OK) {
        printf(
            "WHO_AM_I read error: %s (%d)\n",
            MPU6050_STATUS_STRINGS[status],
            status
        );

        return 1;
    }

    printf(
        "WHO_AM_I = 0x%02X (%u)\n",
        (unsigned)who_am_i,
        (unsigned)who_am_i
    );

    // START ACCELEROMETER CONFIGURATION

    status = mpu6050_set_accel_range(
        I2C_PORT,
        MPU6050_ACCEL_RANGE_8G
    );

    if (status != MPU6050_OK) {
        printf(
            "MPU6050 accel range set error: %s (%d)\n",
            MPU6050_STATUS_STRINGS[status],
            status
        );

        return 1;
    }

    printf("Accelerometer range set to +/-8G\n");

    // START GYROSCOPE CONFIGURATION

    status = mpu6050_set_gyro_range(
        I2C_PORT,
        MPU6050_GYRO_RANGE_250DPS
    );

    if (status != MPU6050_OK) {
        printf(
            "MPU6050 gyro range set error: %s (%d)\n",
            MPU6050_STATUS_STRINGS[status],
            status
        );

        return 1;
    }

    printf("Gyroscope range set to +/-250 DPS\n");

    // START DLPF CONFIGURATION

    const uint8_t dlpf_config = 3;

    status = mpu6050_set_dlpf(
        I2C_PORT,
        dlpf_config
    );

    if (status != MPU6050_OK) {
        printf(
            "MPU6050 DLPF set error: %s (%d)\n",
            MPU6050_STATUS_STRINGS[status],
            status
        );

        return 1;
    }

    printf(
        "DLPF configuration set to: %u\n",
        (unsigned)dlpf_config
    );

    // START SAMPLE-RATE CONFIGURATION

    const uint8_t sample_rate_divider = 9;

    status = mpu6050_set_smplrate(
        I2C_PORT,
        sample_rate_divider
    );

    if (status != MPU6050_OK) {
        printf(
            "MPU6050 sample-rate divider set error: %s (%d)\n",
            MPU6050_STATUS_STRINGS[status],
            status
        );

        return 1;
    }

    printf(
        "Sample-rate divider set to: %u\n",
        (unsigned)sample_rate_divider
    );

    // Allow the sensor and filter output to settle.
    sleep_ms(100);

    // START SAMPLE LOOP

    while (true) {
        mpu6050_sample_raw_t sample = {0};

        status = mpu6050_read_sample_raw(
            I2C_PORT,
            &sample
        );

        if (status != MPU6050_OK) {
            printf(
                "MPU6050 sample read error: %s (%d)\n",
                MPU6050_STATUS_STRINGS[status],
                status
            );
        } else {
            float temp_c =
                ((float)sample.temp / 340.0f) + 36.53f;

            printf(
                "Accel: X=%d Y=%d Z=%d | "
                "Temp: %.2f C | "
                "Gyro: X=%d Y=%d Z=%d\n",
                sample.accel.x,
                sample.accel.y,
                sample.accel.z,
                temp_c,
                sample.gyro.x,
                sample.gyro.y,
                sample.gyro.z
            );
        }

        sleep_ms(1000);
    }
}

static void init_i2c(void) {
    i2c_init(I2C_PORT, 40 * 1000);

    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);

    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);
}