/** \file MPU6050.c
 *
 * \brief MPU6050 API source created for the RPI Pico 2w (2350)
 */
#include "MPU6050.h"
static int mpu6050_write_reg(i2c_inst_t *i2c_inst, uint8_t reg, uint8_t value);
static int mpu6050_read_reg(i2c_inst_t *i2c_inst, uint8_t reg, uint8_t *resp);
static int mpu6050_read_regs(i2c_inst_t *i2c_inst, uint8_t reg, uint8_t *data, size_t len);

const char * const MPU6050_STATUS_STRINGS[MPU6050_STATUS_COUNT] = {
    "!OK",
    "!WRITE",
    "!READ",
    "!INIT",
    "!NULL",
    "!ARG"
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


int mpu6050_read_temp_raw(i2c_inst_t *i2c_inst, int16_t *raw_temp)
{
    if (raw_temp == NULL)
        return MPU6050_NULL_PTR_ERROR;


    uint8_t temp_data[2] = {0};

    int status = mpu6050_read_regs(
        i2c_inst,
        MPU6050_TEMPH_REG,
        temp_data,
        sizeof(temp_data)
    );

    if (status != MPU6050_OK)
        return status;


    *raw_temp = (int16_t)(((uint16_t)temp_data[0] << 8) | temp_data[1]);

    return MPU6050_OK;
}

int mpu6050_read_temp(i2c_inst_t *i2c_inst, float *mpu_temp){

    if (mpu_temp == NULL)
        return MPU6050_NULL_PTR_ERROR;

    int16_t raw_temp = 0;

    int status = mpu6050_read_temp_raw(i2c_inst, &raw_temp);

    if (status != MPU6050_OK)
        return status;

    *mpu_temp = ((float)raw_temp / 340.0f) + 36.53f;

    return MPU6050_OK;
}

int mpu6050_read_accel_raw(i2c_inst_t *i2c_inst, mpu6050_vec16_t *accel)
{
    if (accel == NULL) {
        return MPU6050_NULL_PTR_ERROR;
    }

    uint8_t accel_data[6] = {0};

    int status = mpu6050_read_regs(
        i2c_inst,
        MPU6050_ACCEL_XOUT_H_REG,
        accel_data,
        sizeof(accel_data)
    );

    if (status != MPU6050_OK) {
        return status;
    }

    accel->x = (int16_t)(((uint16_t)accel_data[0] << 8) | accel_data[1]);
    accel->y = (int16_t)(((uint16_t)accel_data[2] << 8) | accel_data[3]);
    accel->z = (int16_t)(((uint16_t)accel_data[4] << 8) | accel_data[5]);

    return MPU6050_OK;
}

int mpu6050_set_accel_range(i2c_inst_t *i2c_inst, uint8_t range){

    if (range > MPU6050_ACCEL_RANGE_16G){
        return MPU6050_INVALID_ARG;
    }

    uint8_t config = 0;

    int status = mpu6050_read_reg(
        i2c_inst,
        MPU6050_ACCEL_CONFIG_REG,
        &config
    );

    if (status != MPU6050_OK) {
        return status;
    }

    config &= ~MPU6050_ACCEL_CONFIG_AFS_SEL_MASK;

    config |= (range << MPU6050_ACCEL_CONFIG_AFS_SEL_SHIFT);

    status = mpu6050_write_reg(
        i2c_inst,
        MPU6050_ACCEL_CONFIG_REG,
        config
    );

    if (status != MPU6050_OK) {
        return status;
    }

    return MPU6050_OK;
}

int mpu6050_set_gyro_range(i2c_inst_t *i2c_inst, uint8_t range){

    if (range > MPU6050_GYRO_RANGE_2000DPS){
        return MPU6050_INVALID_ARG;
    }

    uint8_t config = 0;

    int status = mpu6050_read_reg(
        i2c_inst,
        MPU6050_GYRO_CONFIG_REG,
        &config
    );

    if (status != MPU6050_OK) {
        return status;
    }

    config &= ~MPU6050_GYRO_CONFIG_FS_SEL_MASK;

    config |= (range << MPU6050_GYRO_CONFIG_FS_SEL_SHIFT);

    status = mpu6050_write_reg(
        i2c_inst,
        MPU6050_GYRO_CONFIG_REG,
        config
    );

    if (status != MPU6050_OK) {
        return status;
    }

    return MPU6050_OK;
}

int mpu6050_read_accel_config(i2c_inst_t *i2c_inst, uint8_t *config){

    int status = mpu6050_read_reg(
        i2c_inst,
        MPU6050_ACCEL_CONFIG_REG,
        config
    );

    if(status != MPU6050_OK){
        return status;
    }

    return MPU6050_OK;
}

int mpu6050_read_gyro_config(i2c_inst_t *i2c_inst, uint8_t *config){

    int status = mpu6050_read_reg(
        i2c_inst,
        MPU6050_GYRO_CONFIG_REG,
        config
    );

    if(status != MPU6050_OK){
        return status;
    }

    return MPU6050_OK;
}

int mpu6050_read_gyro_raw(i2c_inst_t *i2c_inst, mpu6050_vec16_t *gyro){

    if (gyro==NULL)
        return MPU6050_NULL_PTR_ERROR;

    uint8_t gyro_data[6] = {0};

    int status = mpu6050_read_regs(
        i2c_inst,
        MPU6050_GYRO_XOUT_H_REG,
        gyro_data,
        sizeof(gyro_data)
    );

    if (status != MPU6050_OK){
        return status;
    }

    gyro->x = (int16_t)(((uint16_t)gyro_data[0] << 8) | gyro_data[1]);
    gyro->y = (int16_t)(((uint16_t)gyro_data[2] << 8) | gyro_data[3]);
    gyro->z = (int16_t)(((uint16_t)gyro_data[4] << 8) | gyro_data[5]);

    return MPU6050_OK;
}

int mpu6050_set_mot_thres(i2c_inst_t *i2c_inst, uint8_t thres){

    int status = mpu6050_write_reg(
        i2c_inst,
        MPU6050_MOT_THR_REG,
        thres
    );

    if(status != MPU6050_OK){
        return status;
    }

    return MPU6050_OK;
}


int mpu6050_read_mot_thres(i2c_inst_t *i2c_inst, uint8_t *thres_reg){

    int status = mpu6050_read_reg(
        i2c_inst,
        MPU6050_MOT_THR_REG,
        thres_reg
    );
    

    if(status != MPU6050_OK){
        return status;
    }

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

    if (i2c_inst == NULL) {
        return MPU6050_NULL_PTR_ERROR;
    }

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

    if (i2c_inst == NULL) {
        return MPU6050_NULL_PTR_ERROR;
    }
    
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

/*! \brief General use function to read from an MPU6050 Register
 *
 * \sa mpu6050_read_reg
 * \param i2c_inst Pico2W i2c instance
 * \param reg MPU6050 Register which is read from
*/
static int mpu6050_read_regs(i2c_inst_t *i2c_inst, uint8_t reg, uint8_t *data, size_t len){
    
    if (i2c_inst == NULL || data == NULL || len == 0) 
        return MPU6050_NULL_PTR_ERROR;


    int status = i2c_write_blocking(
        i2c_inst,
        MPU6050_DEFAULT_I2C_ADDRESS,
        &reg,
        1,
        true
    );

    if (status != 1)
        return MPU6050_WRITE_ERROR;


    status = i2c_read_blocking(
        i2c_inst,
        MPU6050_DEFAULT_I2C_ADDRESS,
        data,
        len,
        false
    );

    if (status != (int)len)
        return MPU6050_READ_ERROR;

    return MPU6050_OK;
}