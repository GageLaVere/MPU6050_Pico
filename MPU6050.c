/** \file MPU6050.c
 *
 * \brief MPU6050 API source created for the RPI Pico 2w (2350)
 */
#include "MPU6050.h"
static int mpu6050_write_reg(i2c_inst_t *i2c_inst, uint8_t reg, uint8_t value);
static int mpu6050_read_reg(i2c_inst_t *i2c_inst, uint8_t reg, uint8_t *resp);

const char *MPU6050_STATUS_STRINGS[MPU6050_STATUS_COUNT] = {
    "!OK",
    "!WRITE",
    "!READ",
    "!INIT",
    "!NULL"
};


int mpu6050_init(i2c_inst_t *i2c_inst){

    int status = mpu6050_write_reg(
        i2c_inst,
        MPU6050_PWR_MGMT_1_REG,
        0x01
    );

    if (status != MPU6050_OK) {
        return MPU6050_INIT_ERROR;
    }

    return MPU6050_OK;
}


int mpu6050_read_who_am_i(i2c_inst_t *i2c_inst, uint8_t *who_am_i){

    return mpu6050_read_reg(i2c_inst, MPU6050_WHOAMI_REG, who_am_i);
}


int mpu6050_read_smplrate(i2c_inst_t *i2c_inst, uint8_t *smplrate){

    return mpu6050_read_reg(i2c_inst, MPU6050_SMPLRATE_REG, smplrate);
}


int mpu6050_read_temp_raw(i2c_inst_t *i2c_inst, int16_t *raw_temp){

    uint8_t temp_h = 0;
    uint8_t temp_l = 0;

    if (raw_temp == NULL)
        return MPU6050_NULL_PTR_ERROR;

    int status = mpu6050_read_reg(i2c_inst, MPU6050_TEMPH_REG, &temp_h);

    if (status != MPU6050_OK)
        return status;

    status = mpu6050_read_reg(i2c_inst, MPU6050_TEMPL_REG, &temp_l);

    if (status != MPU6050_OK)
        return status;

    //defined on page 31 of MPU reference doc
    *raw_temp = ((int16_t) ((temp_h) << 8 | temp_l)/340.0f)+ 36.53;

    return MPU6050_OK;
}


int mpu6050_read_temp(i2c_inst_t *i2c_inst, int16_t *mpu_temp){

    uint8_t temp_h = 0;
    uint8_t temp_l = 0;

    if (mpu_temp == NULL)
        return MPU6050_NULL_PTR_ERROR;

    int status = mpu6050_read_reg(i2c_inst, MPU6050_TEMPH_REG, &temp_h);

    if (status != MPU6050_OK)
        return status;

    status = mpu6050_read_reg(i2c_inst, MPU6050_TEMPL_REG, &temp_l);

    if (status != MPU6050_OK)
        return status;

    //no conversion math
    *mpu_temp = ((temp_h) << 8 | temp_l);

    return MPU6050_OK;
}

/*! \brief General use function to write to an MPU6050 Register
 *
 * \sa mpu6050_write_reg
 * \param i2c_inst Pico2W i2c instance
 * \param reg MPU6050 Register which is written to
 * \param value Data which is written to MPU6050 Register
*/
static int mpu6050_write_reg(i2c_inst_t *i2c_inst, uint8_t reg, uint8_t value){

    uint8_t msg[2] = {reg, value};

    int byte_count = i2c_write_blocking(
        i2c_inst,
        MPU6050_DEFAULT_I2C_ADDRESS,
        msg,
        2,
        false
    );

    if (byte_count != 2) {
        return MPU6050_WRITE_ERROR;
    }

    return MPU6050_OK;
}

/*! \brief General use function to read from an MPU6050 Register
 *
 * \sa mpu6050_read_reg
 * \param i2c_inst Pico2W i2c instance
 * \param reg MPU6050 Register which is read from
*/
static int mpu6050_read_reg(i2c_inst_t *i2c_inst, uint8_t reg, uint8_t *resp){

    if (resp == NULL)
        return MPU6050_NULL_PTR_ERROR;

    int byte_count = i2c_write_blocking(
        i2c_inst,
        MPU6050_DEFAULT_I2C_ADDRESS,
        &reg,
        1,
        true
    );

    if (byte_count != 1) {
        return MPU6050_WRITE_ERROR;
    }

    byte_count = i2c_read_blocking(
        i2c_inst,
        MPU6050_DEFAULT_I2C_ADDRESS,
        resp,
        1,
        false
    );

    if (byte_count != 1) {
        return MPU6050_READ_ERROR;
    }

    return MPU6050_OK;
}