/** \file MPU6050.h
 *
 * \brief MPU6050 API header created for the RPI Pico 2w (2350)
 */

#ifndef MPU6050_H
#define MPU6050_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"


#define MPU6050_DEFAULT_I2C_ADDRESS 0x68

#define MPU6050_PWR_MGMT_1_REG 0x6B

#define MPU6050_WHOAMI_REG 0x75

#define MPU6050_SMPLRATE_REG 0x19

#define MPU6050_CONFIG_REG           0x1A
#define MPU6050_CONFIG_DLPF_MASK     0x07
#define MPU6050_CONFIG_DLPF_SHIFT    0

#define MPU6050_TEMPH_REG 0x41
#define MPU6050_TEMPL_REG 0x42

#define MPU6050_ACCEL_XOUT_H_REG 0x3B
#define MPU6050_ACCEL_XOUT_L_REG 0x3C

#define MPU6050_ACCEL_YOUT_H_REG 0x3D
#define MPU6050_ACCEL_YOUT_L_REG 0x3E

#define MPU6050_ACCEL_ZOUT_H_REG 0x3F
#define MPU6050_ACCEL_ZOUT_L_REG 0x40

#define MPU6050_GYRO_XOUT_H_REG 0x43
#define MPU6050_GYRO_XOUT_L_REG 0x44

#define MPU6050_GYRO_YOUT_H_REG 0x45
#define MPU6050_GYRO_YOUT_L_REG 0x46

#define MPU6050_GYRO_ZOUT_H_REG 0x47
#define MPU6050_GYRO_ZOUT_L_REG 0x48

#define MPU6050_ACCEL_CONFIG_REG 0x1C
#define MPU6050_GYRO_CONFIG_REG  0x1B

#define MPU6050_MOT_THR_REG 0x1F

#define MPU6050_ACCEL_RANGE_2G   0
#define MPU6050_ACCEL_RANGE_4G   1
#define MPU6050_ACCEL_RANGE_8G   2
#define MPU6050_ACCEL_RANGE_16G  3

#define MPU6050_GYRO_RANGE_250DPS   0
#define MPU6050_GYRO_RANGE_500DPS   1
#define MPU6050_GYRO_RANGE_1000DPS  2
#define MPU6050_GYRO_RANGE_2000DPS  3

#define MPU6050_ACCEL_CONFIG_AFS_SEL_SHIFT 3
#define MPU6050_ACCEL_CONFIG_AFS_SEL_MASK  0x18

#define MPU6050_GYRO_CONFIG_FS_SEL_SHIFT 3
#define MPU6050_GYRO_CONFIG_FS_SEL_MASK  0x18


/*! \brief Defines MPU6050 status codes
 * 
 * \sa mpu6050_status_t
*/
typedef enum {
    MPU6050_OK = 0,
    MPU6050_WRITE_ERROR,
    MPU6050_READ_ERROR,
    MPU6050_INIT_ERROR,
    MPU6050_NULL_PTR_ERROR,
    MPU6050_INVALID_ARG,
    MPU6050_STATUS_COUNT
} mpu6050_status_t;

typedef struct {
    int16_t x;
    int16_t y;
    int16_t z;
} mpu6050_vec16_t;


/*! \brief Reads one coherent raw accelerometer, temperature, and gyroscope sample
 *
 * \param i2c_inst Pico 2 W I2C instance
 * \param sample Output structure for the raw sensor sample
 * \return MPU6050 driver status
 */
typedef struct {
    mpu6050_vec16_t accel;
    int16_t temp;
    mpu6050_vec16_t gyro;
} mpu6050_sample_raw_t;

int mpu6050_read_sample_raw(i2c_inst_t *i2c_inst,mpu6050_sample_raw_t *sample);

/*! \brief Sets the MPU6050 gyroscope full-scale range
 *
 * Updates the FS_SEL field in the GYRO_CONFIG register while preserving
 * the other register bits.
 *
 * \param i2c_inst Pico 2 W I2C instance
 * \param range Gyroscope range selection
 *
 * \return MPU6050_OK on success
 * \return MPU6050_INVALID_ARG if range is invalid
 * \return MPU6050_READ_ERROR if GYRO_CONFIG cannot be read
 * \return MPU6050_WRITE_ERROR if GYRO_CONFIG cannot be written
 */
int mpu6050_set_gyro_range(
    i2c_inst_t *i2c_inst,
    uint8_t range
);

/*! \brief Function which reads motion detection threshold
 * \sa mpu6050_read_mot_thres
 * \param i2c_inst Pico2W i2c instance
 * \param thres_reg    Buffer to hold motion threshold setting (0-255)
*/
int mpu6050_read_mot_thres(i2c_inst_t *i2c_inst, uint8_t *thres_reg);

/*! \brief Function which sets the motion detection threshold (0-255)
 *  Intended to be used with #defined ranges
 * \sa mpu6050_set_mot_thres
 * \param i2c_inst Pico2W i2c instance
 * \param thres    Threshold to set (0-255)
*/
int mpu6050_set_mot_thres(i2c_inst_t *i2c_inst, uint8_t thres);

/*! \brief Function which sets the accel range to: ±2g, ±4g, ±8g, ±16g
 *  Intended to be used with #defined ranges
 * \sa mpu6050_set_accel_range
 * \param i2c_inst Pico2W i2c instance
 * \param range    MPU6050 accel instance
*/
int mpu6050_set_accel_range(i2c_inst_t *i2c_inst, uint8_t range);

/*! \brief Function which sets the gyro range ±250, ±500, ±1000, ±2000 deg/s
 *  Intended to be used with #defined ranges
 * \sa mpu6050_set_gyro_range
 * \param i2c_inst Pico2W i2c instance
 * \param range    MPU6050 gyro range
*/
int mpu6050_set_gyro_range(i2c_inst_t *i2c_inst, uint8_t range);

/*! \brief Function which reads the accel range
 *  
 * \sa mpu6050_read_accel_config
 * \param i2c_inst Pico2W i2c instance
 * \param config    MPU6050 accel config buffer
*/
int mpu6050_read_accel_config(i2c_inst_t *i2c_inst, uint8_t *config);

/*! \brief Function which reads the gyro range
 * 
 * \sa mpu6050_read_gyro_config
 * \param i2c_inst Pico2W i2c instance
 * \param config    MPU6050 gyro buffer
*/
int mpu6050_read_gyro_config(i2c_inst_t *i2c_inst, uint8_t *config);

/*! \brief Defines MPU6050 status strings
 * 
 * \sa MPU6050_STATUS_STRINGS
*/
extern const char * const MPU6050_STATUS_STRINGS[MPU6050_STATUS_COUNT];

/*! \brief Function which reads all 3 accels on the MPU6050
 *
 * \sa mpu6050_read_accel_raw
 * \param i2c_inst Pico2W i2c instance
 * \param accel    MPU6050 accel instance
*/
int mpu6050_read_accel_raw(i2c_inst_t *i2c_inst, mpu6050_vec16_t *accel);

/*! \brief Function which reads all 3 gyros on the MPU6050
 *
 * \sa mpu6050_read_gyro_raw
 * \param i2c_inst Pico2W i2c instance
 * \param accel    MPU6050 gyro instance
*/
int mpu6050_read_gyro_raw(i2c_inst_t *i2c_inst, mpu6050_vec16_t *gyro);

/*! \brief Function which initializes the MPU6050
 *
 *  \details This must be called on startup to wake device.
 *  Writes 0x01 to power management register, and sets the 
 *  MPU clock the the x axis gyroscope's with a PLL
 * 
 * \sa mpu6050_init
 * \param i2c_inst Pico2W i2c instance
*/
int mpu6050_init(i2c_inst_t *i2c_inst);


/*! \brief Reads WHO_AM_I register 
 * 
 * \sa mpu6050_read_who_am_i
 * \param i2c_inst Pico2W i2c instance
 * \return Status returned by MPU6050 driver
*/
int mpu6050_read_who_am_i(i2c_inst_t *i2c_inst, uint8_t *who_am_i);


/*! \brief Reads temperature sensor on MPU6050, converted to degrees C
 *  \sa     mpu6050_read_temp
 *  \param i2c_inst Pico2W i2c instance
 *  \return Status returned by MPU6050 driver
 */
int mpu6050_read_temp(i2c_inst_t *i2c_inst, float *mpu_temp);

/*! \brief Reads raw temperature sensor value from MPU6050
 *  \sa     mpu6050_read_temp_raw
 *  \param i2c_inst Pico2W i2c instance
 *  \return Status returned by MPU6050 driver
 */
int mpu6050_read_temp_raw(i2c_inst_t *i2c_inst, int16_t *raw_temp);

/*! \brief Sets the MPU6050 sample-rate divider
 *
 * \param i2c_inst Pico 2 W I2C instance
 * \param divider Value written to the SMPLRT_DIV register
 * \return MPU6050 driver status
 */
int mpu6050_set_smplrate(i2c_inst_t *i2c_inst, uint8_t divider);

/*! \brief Reads Sample Rate register 
 * 
 * \details This register specifies the divider from 
 * the gyroscope output rate used to generate the Sample Rate 
 * 
 * \sa mpu6050_read_smplrate
 * \param i2c_inst Pico2W i2c instance
 * \return Status returned by MPU6050 driver
*/
int mpu6050_read_smplrate(i2c_inst_t *i2c_inst, uint8_t *smplrate);

/*! \brief Sets the MPU6050 digital low-pass filter configuration
 *
 * Updates the DLPF_CFG field in the CONFIG register while preserving
 * the other register bits.
 *
 * \param i2c_inst Pico 2 W I2C instance
 * \param dlpf_config DLPF configuration value from 0 to 7
 */
int mpu6050_set_dlpf(i2c_inst_t *i2c_inst,uint8_t dlpf_config);

/*! \brief Reads the MPU6050 digital low-pass filter configuration
 *
 * Reads the CONFIG register and extracts the DLPF_CFG field.
 *
 * \param i2c_inst Pico 2 W I2C instance
 * \param dlpf_config Output pointer for the DLPF configuration value
 */
int mpu6050_read_dlpf(i2c_inst_t *i2c_inst, uint8_t *dlpf_config);

#endif