
#include <asf.h>
#include <I2C.h>

void configure_i2c_master(void)
{
  /* Initialize config structure and software module. */
  struct i2c_master_config config_i2c_master;
  i2c_master_get_config_defaults(&config_i2c_master);
  /* Change buffer timeout to something longer. */
  config_i2c_master.buffer_timeout = 10000;
  config_i2c_master.pinmux_pad0=PINMUX_PA16C_SERCOM1_PAD0;
  config_i2c_master.pinmux_pad1=PINMUX_PA17C_SERCOM1_PAD1;
  
  /* Initialize and enable device with config. */
  i2c_master_init(&i2c_master_instance, CONF_I2C_MASTER_MODULE, &config_i2c_master);
  i2c_master_enable(&i2c_master_instance);
}
