# MPU6050 Raspberry Pi Pico 2W (2350) Driver

## Overview

    This library is a low-level driver for the MPU6050 device.

    A working list of implemented functions is in the "Function Table" section.

    The goal of this project is to build a clear, documented, and maintainable MPU6050 driver for the Raspberry Pi Pico 2 W. The driver is intended to be useful for embedded projects that need direct control over device configuration rather than relying on Arduino-focused libraries or high-level wrappers.

    "MPU6050" refers to a model of Motion Processing Unit. 
    
    From the manufacturer: 
    "The devices combine a 3-axis gyroscope and a 3-axis accelerometer on the same silicon die together with an onboard Digital Motion Processor™ (DMP™) capable of processing complex 9-axis sensor fusion algorithms using the field-proven and proprietary MotionFusion™ engine."


    Examples / Tests can be found in examples directory

## Reference Documents

    Primary MPU6050 register reference:

    MPU-6000/MPU-6050 Register Map and Descriptions, RM-MPU-6000A

    Raspberry Pi Pico 2 W / RP2350 references:

    Raspberry Pi Pico 2 W Datasheet
    RP2350 Datasheet

## Wiring

| MPU6050 Pin | Raspberry Pi Pico 2 W Pin | Notes                                 |
| ----------- | ------------------------- | ------------------------------------- |
| VCC         | 3V3 or VSYS*              | Depends on breakout board regulator   |
| GND         | GND                       | Common ground                         |
| SDA         | GPIO8                     | I2C0 SDA                              |
| SCL         | GPIO9                     | I2C0 SCL                              |
| INT         | Optional GPIO             | Used for motion/data-ready interrupts |
| AD0         | GND or 3V3                | Selects I2C address                   |


## Function Table


| Function                            | Register(s)                           | Status      | Description                                                            |
| ----------------------------------- | ------------------------------------- | ----------- | ---------------------------------------------------------------------- |
| `mpu6050_init()`                    | `PWR_MGMT_1`, `WHO_AM_I`              | Planned     | Initializes I2C communication, wakes the device, and verifies identity |
| `mpu6050_wake()`                    | `PWR_MGMT_1`                          | Planned     | Clears sleep mode and selects clock source                             |
| `mpu6050_read_who_am_i()`           | `WHO_AM_I`                            | Planned     | Reads the device identity register                                     |
| `mpu6050_set_accel_range()`         | `ACCEL_CONFIG`                        | Planned     | Sets accelerometer full-scale range                                    |
| `mpu6050_set_gyro_range()`          | `GYRO_CONFIG`                         | Planned     | Sets gyroscope full-scale range                                        |
| `mpu6050_read_accel_raw()`          | `ACCEL_XOUT_H` through `ACCEL_ZOUT_L` | Planned     | Reads raw 16-bit accelerometer values                                  |
| `mpu6050_read_gyro_raw()`           | `GYRO_XOUT_H` through `GYRO_ZOUT_L`   | Planned     | Reads raw 16-bit gyroscope values                                      |
| `mpu6050_read_temp_raw()`           | `TEMP_OUT_H`, `TEMP_OUT_L`            | Planned     | Reads raw temperature output                                           |
| `mpu6050_config_motion_interrupt()` | Motion interrupt registers            | Planned     | Configures motion detection threshold and duration                     |
| `mpu6050_read_int_status()`         | `INT_STATUS`                          | Planned     | Reads interrupt flags                                                  |
| `mpu6050_fifo_enable()`             | FIFO configuration registers          | Planned     | Enables selected FIFO sources                                          |
| `mpu6050_fifo_read()`               | `FIFO_COUNT`, `FIFO_R_W`              | Planned     | Reads data from the FIFO buffer                                        |

