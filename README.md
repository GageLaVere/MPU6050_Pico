# MPU6050 Raspberry Pi Pico 2 W (RP2350) Driver

## Overview

This library is a low-level C driver for the MPU6050 Inertial Measurement Unit using the Raspberry Pi Pico 2 W / RP2350.

The goal of this project is to build a clear, documented, and maintainable MPU6050 driver for embedded projects that need direct register-level control rather than relying on Arduino-focused libraries or high-level wrappers.

The MPU6050 combines a 3-axis gyroscope and a 3-axis accelerometer in the same device. It also includes an onboard Digital Motion Processor, though this driver currently focuses on direct register access, raw sensor reads, and basic device configuration.

Examples and manual tests can be found in the `examples` directory.

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

## Current Project Status

This driver is currently a work in progress.

Implemented and tested so far:

* I2C register write helper
* I2C single-register read helper
* I2C multi-register burst read helper
* Device wake/init through `PWR_MGMT_1`
* `WHO_AM_I` register read
* Sample-rate divider register read
* Raw temperature read
* Converted Celsius temperature read
* Raw accelerometer read
* Raw gyroscope read

Currently in progress:

* Accelerometer full-scale range configuration
* Gyroscope full-scale range configuration
* Config register readback helpers

Planned later:

* Scaled accel output in `g`
* Scaled gyro output in degrees/sec
* Interrupt support
* FIFO support
* Motion detection configuration
* More complete examples/tests

---

## Notes

The MPU6050 powers up in sleep mode.

`mpu6050_init()` must be called before normal sensor reads.

Driver functions return a status code.

Sensor/register values are returned through pointer arguments.

Example pattern:

```c
uint8_t who_am_i = 0;

int status = mpu6050_read_who_am_i(i2c_inst, &who_am_i);

if (status == MPU6050_OK) {
    // who_am_i is valid
}
```

If a function returns anything other than `MPU6050_OK`, the output value should not be used.

Status values are positive enum values so they can be used as indexes into the status string table.

The current status strings are intended to be short and easy to transmit later over USB, UART, or another host interface.

---

## Status Codes

| Status Code              | Value | Status String |
| ------------------------ | ----- | ------------- |
| `MPU6050_OK`             | `0`   | `!OK`         |
| `MPU6050_WRITE_ERROR`    | `1`   | `!WRITE`      |
| `MPU6050_READ_ERROR`     | `2`   | `!READ`       |
| `MPU6050_INIT_ERROR`     | `3`   | `!INIT`       |
| `MPU6050_NULL_PTR_ERROR` | `4`   | `!NULL`       |

Note: an `MPU6050_INVALID_ARG_ERROR` status may be added later for invalid range/configuration arguments.

---

## Wiring

| MPU6050 Pin | Raspberry Pi Pico 2 W Pin | Notes                                 |
| ----------- | ------------------------- | ------------------------------------- |
| VCC         | 3V3                       | Recommended for safe Pico I/O levels  |
| GND         | GND                       | Common ground                         |
| SDA         | GPIO8                     | I2C0 SDA                              |
| SCL         | GPIO9                     | I2C0 SCL                              |
| INT         | Optional GPIO             | Used for motion/data-ready interrupts |
| AD0         | GND or 3V3                | Selects I2C address                   |

### Voltage Warning

The Raspberry Pi Pico 2 W GPIO pins are not 5 V tolerant.

Many MPU6050 breakout boards include pull-up resistors on SDA/SCL. Make sure the I2C pull-ups are tied to 3.3 V, not 5 V.

The default I2C address is `0x68` when `AD0` is connected to GND.

---

## Function Table

| Function                            | Register(s)                           | Status      | Description                                                      |
| ----------------------------------- | ------------------------------------- | ----------- | ---------------------------------------------------------------- |
| `mpu6050_init()`                    | `PWR_MGMT_1`                          | Implemented | Clears sleep mode and selects clock source                       |
| `mpu6050_read_who_am_i()`           | `WHO_AM_I`                            | Implemented | Reads the device identity register through an output pointer     |
| `mpu6050_read_smplrate()`           | `SMPLRT_DIV`                          | Implemented | Reads the sample-rate divider register through an output pointer |
| `mpu6050_read_temp()`               | `TEMP_OUT_H`, `TEMP_OUT_L`            | Implemented | Reads raw temperature and returns converted degrees Celsius      |
| `mpu6050_read_temp_raw()`           | `TEMP_OUT_H`, `TEMP_OUT_L`            | Implemented | Reads raw signed 16-bit temperature output                       |
| `mpu6050_read_accel_raw()`          | `ACCEL_XOUT_H` through `ACCEL_ZOUT_L` | Implemented | Reads raw signed 16-bit accelerometer values                     |
| `mpu6050_read_gyro_raw()`           | `GYRO_XOUT_H` through `GYRO_ZOUT_L`   | Implemented | Reads raw signed 16-bit gyroscope values                         |
| `mpu6050_set_accel_range()`         | `ACCEL_CONFIG`                        | In Progress | Sets accelerometer full-scale range using read-modify-write      |
| `mpu6050_set_gyro_range()`          | `GYRO_CONFIG`                         | In Progress | Sets gyroscope full-scale range using read-modify-write          |
| `mpu6050_read_accel_config()`       | `ACCEL_CONFIG`                        | In Progress | Reads accelerometer configuration register                       |
| `mpu6050_read_gyro_config()`        | `GYRO_CONFIG`                         | In Progress | Reads gyroscope configuration register                           |
| `mpu6050_config_motion_interrupt()` | Motion interrupt registers            | Planned     | Configures motion detection threshold and duration               |
| `mpu6050_read_int_status()`         | `INT_STATUS`                          | Planned     | Reads interrupt flags                                            |
| `mpu6050_fifo_enable()`             | FIFO configuration registers          | Planned     | Enables selected FIFO sources                                    |
| `mpu6050_fifo_read()`               | `FIFO_COUNT`, `FIFO_R_W`              | Planned     | Reads data from the FIFO buffer                                  |

---

## Current Driver Interface Style

The current driver uses status returns and output pointers.

Example:

```c
mpu6050_vec16_t accel = {0};

int status = mpu6050_read_accel_raw(i2c_inst, &accel);

if (status == MPU6050_OK) {
    // accel.x, accel.y, and accel.z are valid
}
```

This keeps return values dedicated to driver status and avoids mixing error codes with sensor data.

---

## Current Data Types

| Type               | Description                                      |
| ------------------ | ------------------------------------------------ |
| `mpu6050_status_t` | Driver status/error code enum                    |
| `mpu6050_vec16_t`  | Signed 16-bit X/Y/Z vector for raw sensor values |

Raw accelerometer and gyroscope values are returned as signed 16-bit values.

Converted/scaled sensor values will be added later after range configuration support is finalized.

---

## Current Implemented Register Defines

| Define                     | Register |
| -------------------------- | -------- |
| `MPU6050_PWR_MGMT_1_REG`   | `0x6B`   |
| `MPU6050_WHOAMI_REG`       | `0x75`   |
| `MPU6050_SMPLRATE_REG`     | `0x19`   |
| `MPU6050_TEMP_OUT_H_REG`   | `0x41`   |
| `MPU6050_TEMP_OUT_L_REG`   | `0x42`   |
| `MPU6050_ACCEL_XOUT_H_REG` | `0x3B`   |
| `MPU6050_ACCEL_XOUT_L_REG` | `0x3C`   |
| `MPU6050_ACCEL_YOUT_H_REG` | `0x3D`   |
| `MPU6050_ACCEL_YOUT_L_REG` | `0x3E`   |
| `MPU6050_ACCEL_ZOUT_H_REG` | `0x3F`   |
| `MPU6050_ACCEL_ZOUT_L_REG` | `0x40`   |
| `MPU6050_GYRO_XOUT_H_REG`  | `0x43`   |
| `MPU6050_GYRO_XOUT_L_REG`  | `0x44`   |
| `MPU6050_GYRO_YOUT_H_REG`  | `0x45`   |
| `MPU6050_GYRO_YOUT_L_REG`  | `0x46`   |
| `MPU6050_GYRO_ZOUT_H_REG`  | `0x47`   |
| `MPU6050_GYRO_ZOUT_L_REG`  | `0x48`   |
| `MPU6050_ACCEL_CONFIG_REG` | `0x1C`   |
| `MPU6050_GYRO_CONFIG_REG`  | `0x1B`   |

---

## Accelerometer Range Defines

| Define                    | Range  | AFS_SEL Value | Register Bits |
| ------------------------- | ------ | ------------- | ------------- |
| `MPU6050_ACCEL_RANGE_2G`  | `±2g`  | `0`           | `0x00`        |
| `MPU6050_ACCEL_RANGE_4G`  | `±4g`  | `1`           | `0x08`        |
| `MPU6050_ACCEL_RANGE_8G`  | `±8g`  | `2`           | `0x10`        |
| `MPU6050_ACCEL_RANGE_16G` | `±16g` | `3`           | `0x18`        |

The accelerometer full-scale range is controlled by bits `4:3` in `ACCEL_CONFIG`.

---

## Gyroscope Range Defines

| Define                       | Range         | FS_SEL Value | Register Bits |
| ---------------------------- | ------------- | ------------ | ------------- |
| `MPU6050_GYRO_RANGE_250DPS`  | `±250 deg/s`  | `0`          | `0x00`        |
| `MPU6050_GYRO_RANGE_500DPS`  | `±500 deg/s`  | `1`          | `0x08`        |
| `MPU6050_GYRO_RANGE_1000DPS` | `±1000 deg/s` | `2`          | `0x10`        |
| `MPU6050_GYRO_RANGE_2000DPS` | `±2000 deg/s` | `3`          | `0x18`        |

The gyroscope full-scale range is controlled by bits `4:3` in `GYRO_CONFIG`.

---

## Current I2C Setup Used In Example

| Setting | Value    |
| ------- | -------- |
| I2C Bus | `i2c0`   |
| SDA     | `GPIO8`  |
| SCL     | `GPIO9`  |
| Speed   | `40 kHz` |
| Address | `0x68`   |

---

## Verified Example Output

Recent manual serial output showed the following expected behavior:

```text
WHO_AM_I = 0x68
MPU6050 Temp: about 24.5 C
MPU6050 Raw Temp: about -4096
Accel raw: one axis near ±1g depending on board orientation
Gyro raw: near zero at rest, with spikes during rotation
```

Example flat-orientation accelerometer output:

```text
Accel raw: X=-272 Y=48 Z=17716
```

With the default `±2g` accelerometer range, `1g` is approximately `16384` counts. The measured value is close enough for early bring-up. Calibration has not been implemented yet.

---

## Current Limitations

* No calibration or bias compensation yet.
* No scaled accel output in `g` yet.
* No scaled gyro output in degrees/sec yet.
* I2C address is currently fixed at `0x68`.
* Range setters are still being finalized and verified.
* FIFO support is not implemented yet.
* Interrupt support is not implemented yet.
* The DMP is not supported.
* This driver is not production-certified and is currently intended for learning, bring-up, and embedded project development.

---

## Planned Development Order

1. Finish and verify accel/gyro range configuration.
2. Add config register readback tests.
3. Add scaled accel conversion.
4. Add scaled gyro conversion.
5. Add basic data-ready interrupt support.
6. Add optional motion interrupt support.
7. Add FIFO support.
8. Add cleaner examples and host-side parsing support.
