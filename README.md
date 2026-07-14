# MPU6050 Raspberry Pi Pico 2 W (RP2350) Driver

## Overview

This library is a low-level C driver for the MPU6050 Inertial Measurement Unit using the Raspberry Pi Pico 2 W / RP2350.

The project provides direct register-level control of the MPU6050 without relying on Arduino-focused libraries or high-level wrappers. It supports device initialization, sensor configuration, individual raw sensor reads, and a coherent burst read containing accelerometer, temperature, and gyroscope data.

A usage example can be found in the `examples` directory.

---

## Reference Documents

Primary MPU6050 register reference:

* `MPU-6000/MPU-6050 Register Map and Descriptions`
* Document number: `RM-MPU-6000A`

Raspberry Pi Pico 2 W / RP2350 references:

* Raspberry Pi Pico 2 W Datasheet
* RP2350 Datasheet
* Raspberry Pi Pico C/C++ SDK Documentation

---

## Implemented Functionality

* I2C register write helper
* I2C single-register read helper
* I2C multi-register burst-read helper
* Device wake and clock-source configuration through `PWR_MGMT_1`
* `WHO_AM_I` register read
* Sample-rate divider read and write
* Digital low-pass filter configuration and readback
* Raw temperature read
* Converted Celsius temperature read
* Raw three-axis accelerometer read
* Raw three-axis gyroscope read
* Coherent 14-byte accelerometer, temperature, and gyroscope burst read
* Accelerometer full-scale range configuration and register readback
* Gyroscope full-scale range configuration and register readback
* Motion-threshold register read and write
* Status-code and status-string reporting
* Null-pointer and invalid-argument handling

---

## Driver Usage Notes

The MPU6050 powers up in sleep mode. `mpu6050_init()` must be called before normal sensor reads or configuration.

Driver functions return a status code. Sensor and register values are returned through output-pointer arguments.

```c
uint8_t who_am_i = 0;

int status = mpu6050_read_who_am_i(i2c_inst, &who_am_i);

if (status == MPU6050_OK) {
    // who_am_i is valid
}
```

If a function returns anything other than `MPU6050_OK`, its output value should not be used.

Status values are non-negative enum values and can be used as indexes into `MPU6050_STATUS_STRINGS`.

---

## Status Codes

| Status Code              | Value | Status String |
| ------------------------ | ----- | ------------- |
| `MPU6050_OK`             | `0`   | `!OK`         |
| `MPU6050_WRITE_ERROR`    | `1`   | `!WRITE`      |
| `MPU6050_READ_ERROR`     | `2`   | `!READ`       |
| `MPU6050_INIT_ERROR`     | `3`   | `!INIT`       |
| `MPU6050_NULL_PTR_ERROR` | `4`   | `!NULL`       |
| `MPU6050_INVALID_ARG`    | `5`   | `!ARG`        |

---

## Wiring

| MPU6050 Pin | Raspberry Pi Pico 2 W Pin | Notes                              |
| ----------- | ------------------------- | ---------------------------------- |
| VCC         | 3V3                       | Use 3.3 V for safe Pico I/O levels |
| GND         | GND                       | Common ground                      |
| SDA         | GPIO8                     | I2C0 SDA                           |
| SCL         | GPIO9                     | I2C0 SCL                           |
| AD0         | GND                       | Selects the default address `0x68` |

### Voltage Warning

The Raspberry Pi Pico 2 W GPIO pins are not 5 V tolerant.

Many MPU6050 breakout boards include pull-up resistors on SDA and SCL. Make sure the I2C pull-ups are tied to 3.3 V rather than 5 V.

---

## Function Table

| Function                      | Register(s)                           | Description                                                               |
| ----------------------------- | ------------------------------------- | ------------------------------------------------------------------------- |
| `mpu6050_init()`              | `PWR_MGMT_1`                          | Wakes the device and selects the X-axis gyroscope PLL as the clock source |
| `mpu6050_read_who_am_i()`     | `WHO_AM_I`                            | Reads the device identity register                                        |
| `mpu6050_set_smplrate()`      | `SMPLRT_DIV`                          | Writes the sample-rate divider                                            |
| `mpu6050_read_smplrate()`     | `SMPLRT_DIV`                          | Reads the sample-rate divider                                             |
| `mpu6050_set_dlpf()`          | `CONFIG`                              | Sets the `DLPF_CFG` field while preserving other register bits            |
| `mpu6050_read_dlpf()`         | `CONFIG`                              | Reads the `DLPF_CFG` field                                                |
| `mpu6050_read_temp()`         | `TEMP_OUT_H`, `TEMP_OUT_L`            | Reads and converts the temperature output to degrees Celsius              |
| `mpu6050_read_temp_raw()`     | `TEMP_OUT_H`, `TEMP_OUT_L`            | Reads the raw signed 16-bit temperature output                            |
| `mpu6050_read_accel_raw()`    | `ACCEL_XOUT_H` through `ACCEL_ZOUT_L` | Reads raw signed 16-bit X, Y, and Z accelerometer values                  |
| `mpu6050_read_gyro_raw()`     | `GYRO_XOUT_H` through `GYRO_ZOUT_L`   | Reads raw signed 16-bit X, Y, and Z gyroscope values                      |
| `mpu6050_read_sample_raw()`   | `ACCEL_XOUT_H` through `GYRO_ZOUT_L`  | Reads one coherent 14-byte accelerometer, temperature, and gyro sample    |
| `mpu6050_set_accel_range()`   | `ACCEL_CONFIG`                        | Sets the accelerometer full-scale range using read-modify-write           |
| `mpu6050_read_accel_config()` | `ACCEL_CONFIG`                        | Reads the accelerometer configuration register                            |
| `mpu6050_set_gyro_range()`    | `GYRO_CONFIG`                         | Sets the gyroscope full-scale range using read-modify-write               |
| `mpu6050_read_gyro_config()`  | `GYRO_CONFIG`                         | Reads the gyroscope configuration register                                |
| `mpu6050_set_mot_thres()`     | `MOT_THR`                             | Writes the motion-threshold register                                      |
| `mpu6050_read_mot_thres()`    | `MOT_THR`                             | Reads the motion-threshold register                                       |

---

## Data Types

### `mpu6050_status_t`

Driver status and error-code enum.

### `mpu6050_vec16_t`

Signed 16-bit X/Y/Z vector used for raw accelerometer and gyroscope values.

```c
typedef struct {
    int16_t x;
    int16_t y;
    int16_t z;
} mpu6050_vec16_t;
```

### `mpu6050_sample_raw_t`

Container for one coherent raw MPU6050 sample.

```c
typedef struct {
    mpu6050_vec16_t accel;
    int16_t temp;
    mpu6050_vec16_t gyro;
} mpu6050_sample_raw_t;
```

The sample is read in one continuous transaction beginning at `ACCEL_XOUT_H`.

---

## Implemented Register Defines

| Define                     | Register |
| -------------------------- | -------- |
| `MPU6050_SMPLRATE_REG`     | `0x19`   |
| `MPU6050_CONFIG_REG`       | `0x1A`   |
| `MPU6050_GYRO_CONFIG_REG`  | `0x1B`   |
| `MPU6050_ACCEL_CONFIG_REG` | `0x1C`   |
| `MPU6050_MOT_THR_REG`      | `0x1F`   |
| `MPU6050_ACCEL_XOUT_H_REG` | `0x3B`   |
| `MPU6050_ACCEL_XOUT_L_REG` | `0x3C`   |
| `MPU6050_ACCEL_YOUT_H_REG` | `0x3D`   |
| `MPU6050_ACCEL_YOUT_L_REG` | `0x3E`   |
| `MPU6050_ACCEL_ZOUT_H_REG` | `0x3F`   |
| `MPU6050_ACCEL_ZOUT_L_REG` | `0x40`   |
| `MPU6050_TEMPH_REG`        | `0x41`   |
| `MPU6050_TEMPL_REG`        | `0x42`   |
| `MPU6050_GYRO_XOUT_H_REG`  | `0x43`   |
| `MPU6050_GYRO_XOUT_L_REG`  | `0x44`   |
| `MPU6050_GYRO_YOUT_H_REG`  | `0x45`   |
| `MPU6050_GYRO_YOUT_L_REG`  | `0x46`   |
| `MPU6050_GYRO_ZOUT_H_REG`  | `0x47`   |
| `MPU6050_GYRO_ZOUT_L_REG`  | `0x48`   |
| `MPU6050_PWR_MGMT_1_REG`   | `0x6B`   |
| `MPU6050_WHOAMI_REG`       | `0x75`   |

---

## Accelerometer Range Defines

| Define                    | Range  | `AFS_SEL` Value | Register Bits |
| ------------------------- | ------ | --------------- | ------------- |
| `MPU6050_ACCEL_RANGE_2G`  | `±2g`  | `0`             | `0x00`        |
| `MPU6050_ACCEL_RANGE_4G`  | `±4g`  | `1`             | `0x08`        |
| `MPU6050_ACCEL_RANGE_8G`  | `±8g`  | `2`             | `0x10`        |
| `MPU6050_ACCEL_RANGE_16G` | `±16g` | `3`             | `0x18`        |

The accelerometer full-scale range is controlled by bits `4:3` in `ACCEL_CONFIG`.

### Accelerometer Sensitivity

| Range  | Sensitivity   |
| ------ | ------------- |
| `±2g`  | `16384 LSB/g` |
| `±4g`  | `8192 LSB/g`  |
| `±8g`  | `4096 LSB/g`  |
| `±16g` | `2048 LSB/g`  |

---

## Gyroscope Range Defines

| Define                       | Range         | `FS_SEL` Value | Register Bits |
| ---------------------------- | ------------- | -------------- | ------------- |
| `MPU6050_GYRO_RANGE_250DPS`  | `±250 deg/s`  | `0`            | `0x00`        |
| `MPU6050_GYRO_RANGE_500DPS`  | `±500 deg/s`  | `1`            | `0x08`        |
| `MPU6050_GYRO_RANGE_1000DPS` | `±1000 deg/s` | `2`            | `0x10`        |
| `MPU6050_GYRO_RANGE_2000DPS` | `±2000 deg/s` | `3`            | `0x18`        |

The gyroscope full-scale range is controlled by bits `4:3` in `GYRO_CONFIG`.

### Gyroscope Sensitivity

| Range         | Sensitivity        |
| ------------- | ------------------ |
| `±250 deg/s`  | `131 LSB/(deg/s)`  |
| `±500 deg/s`  | `65.5 LSB/(deg/s)` |
| `±1000 deg/s` | `32.8 LSB/(deg/s)` |
| `±2000 deg/s` | `16.4 LSB/(deg/s)` |

---

## Temperature Conversion

The raw temperature value is converted to degrees Celsius with:

```c
float temp_c = ((float)raw_temp / 340.0f) + 36.53f;
```

---

## Sample-Rate Configuration

The sample-rate divider is stored in `SMPLRT_DIV`.

```text
Sample Rate = Gyroscope Output Rate / (1 + SMPLRT_DIV)
```

The example configures:

```text
DLPF_CFG    = 3
SMPLRT_DIV  = 9
Sample Rate = 1000 Hz / (1 + 9) = 100 Hz
```

The delay used by the example for serial printing is independent of the MPU6050 internal sample rate.

---

## Example Configuration

The included example configures the sensor with:

| Setting              | Value        |
| -------------------- | ------------ |
| I2C Bus              | `i2c0`       |
| SDA                  | `GPIO8`      |
| SCL                  | `GPIO9`      |
| I2C Speed            | `40 kHz`     |
| I2C Address          | `0x68`       |
| Accelerometer Range  | `±8g`        |
| Gyroscope Range      | `±250 deg/s` |
| DLPF Configuration   | `3`          |
| Sample-Rate Divider  | `9`          |
| Internal Sample Rate | `100 Hz`     |

---


