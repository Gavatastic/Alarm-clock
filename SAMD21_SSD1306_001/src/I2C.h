#ifndef I2C_H
#define I2C_H

#include <asf.h>

#define CONF_I2C_MASTER_MODULE SERCOM1

struct i2c_master_module i2c_master_instance;

void configure_i2c_master(void);

#endif	