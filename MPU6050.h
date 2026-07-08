/** \file MPU6050.h
 *
 * \brief MPU6050 API header created for the RPI Pico 2w (2350)
 */
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
    MPU6050_STATUS_COUNT
} mpu6050_status_t;

typedef struct {
    int16_t x;
    int16_t y;
    int16_t z;
} mpu6050_vec16_t;

/*! \brief Defines MPU6050 status strings
 * 
 * \sa MPU6050_STATUS_STRINGS
*/
extern const char *MPU6050_STATUS_STRINGS[MPU6050_STATUS_COUNT];


/*! \brief Function which reads all 3 accels on the MPU6050
 *
 * \sa mpu6050_init
 * \param i2c_inst Pico2W i2c instance
 * \param accel    MPU6050 accel instance
*/
int mpu6050_read_accel_raw(i2c_inst_t *i2c_inst, mpu6050_vec16_t *accel);

/*! \brief Function which reads all 3 accels on the MPU6050
 *
 * \sa mpu6050_init
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