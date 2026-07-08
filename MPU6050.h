/** \file MPU6050.h
 *
 * \brief MPU6050 API header created for the RPI Pico 2w (2350)
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"


#define MPU6050_DEFAULT_I2C_ADDRESS 0x68

#define MPU6050_PWR_MGMT_1_REG 0x6B

#define MPU6050_WHOAMI_REG 0x75

#define MPU6050_TEMPH_REG 0x41
#define MPU6050_TEMPL_REG 0x42

#define MPU6050_SMPLRATE_REG 0x19

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

/*! \brief Defines MPU6050 status strings
 * 
 * \sa MPU6050_STATUS_STRINGS
*/
extern const char *MPU6050_STATUS_STRINGS[MPU6050_STATUS_COUNT];

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
int mpu6050_read_temp(i2c_inst_t *i2c_inst, int16_t *mpu_temp);

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