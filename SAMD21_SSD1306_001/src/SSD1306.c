#include <asf.h>
#include <I2C.h>
#include <SSD1306.h>

#define DATA_LENGTH 2
#define SLAVE_ADDRESS 0x3C
#define PACKET_LENGTH 17
#define TIMEOUT 1000

static uint8_t command_buffer[DATA_LENGTH] = {0x00, 0x00,};

static void send_command(uint8_t command)
{
	/* Timeout counter. */
	uint16_t timeout = 0;
	
	/* Insert command */
	command_buffer[1]=command;
	
	/* Init i2c packet. */
	struct i2c_master_packet command_packet = {
		.address     = SLAVE_ADDRESS,
		.data_length = DATA_LENGTH,
		.data        = command_buffer,
		.ten_bit_address = false,
		.high_speed      = true,
		.hs_master_code  = 0x0,
	};
	
	/* Write buffer to slave until success. */
	while (i2c_master_write_packet_wait(&i2c_master_instance, &command_packet) !=
	STATUS_OK) {
		/* Increment timeout counter and check if timed out. */
		if (timeout++ == TIMEOUT) {
			break;
		}
	}
	
}

void SSD1306_init()
{

	   
	   // Turn display off
	   send_command(SSD1306_DISPLAYOFF);

	   send_command(SSD1306_SETDISPLAYCLOCKDIV);
	   send_command(0x80);

	   send_command(SSD1306_SETMULTIPLEX);
	   send_command(0x3F);
	   
	   send_command(SSD1306_SETDISPLAYOFFSET);
	   send_command(0x00);
	   
	   send_command(SSD1306_SETSTARTLINE | 0x00);
	   
	   // We use internal charge pump
	   send_command(SSD1306_CHARGEPUMP);
	   send_command(0x14);
	   
	   // Horizontal memory mode
	   send_command(SSD1306_MEMORYMODE);
	   send_command(0x00);
	   
	   send_command(SSD1306_SEGREMAP | 0x1);

	   send_command(SSD1306_COMSCANDEC);

	   send_command(SSD1306_SETCOMPINS);
	   send_command(0x12);

	   // Max contrast
	   send_command(SSD1306_SETCONTRAST);
	   send_command(0xCF);

	   send_command(SSD1306_SETPRECHARGE);
	   send_command(0xF1);

	   send_command(SSD1306_SETVCOMDETECT);
	   send_command(0x40);

	   send_command(SSD1306_DISPLAYALLON_RESUME);

	   // inverted display
	   send_command(SSD1306_NORMALDISPLAY);

	   // Turn display back on
	   send_command(SSD1306_DISPLAYON);	
	
	
}

void SSD1306_send_buffer(uint8_t *buffer)
{
	uint8_t payload[17];
	payload[0]=0x40;
	  
	send_command(SSD1306_COLUMNADDR);
	send_command(0x00);
	send_command(0x7F);

	send_command(SSD1306_PAGEADDR);
	send_command(0x00);
	send_command(0x07);
		
	/* Timeout counter. */
	uint16_t timeout = 0;
	struct i2c_master_packet buffer_packet = {
		.address     = SLAVE_ADDRESS,
		.data_length = PACKET_LENGTH,
		.data        = payload,
		.ten_bit_address = false,
		.high_speed      = true,
		.hs_master_code  = 0x0,
	};
	// We have to send the buffer as 16 bytes packets
	// Our buffer is 1024 bytes long, 1024/16 = 64
	// We have to send 64 packets
	for (uint8_t packet = 0; packet < 64; packet++) 
	{

		for (uint8_t packet_byte = 0; packet_byte < 16; ++packet_byte) {
			payload[packet_byte+1]=buffer[packet*16+packet_byte];
		}

		buffer_packet.data=payload;

		while (i2c_master_write_packet_wait(&i2c_master_instance, &buffer_packet) != STATUS_OK) 
		{
			/* Increment timeout counter and check if timed out. */
			if (timeout++ == TIMEOUT) {
				break;
			}
		}
	}
	
}

void SSD1306_invert(uint8_t inverted)
{
  if (inverted) {
    send_command(SSD1306_INVERTDISPLAY);
  } else {
    send_command(SSD1306_NORMALDISPLAY);
  }
}

