
#include <asf.h>
#include <TPIC6B595.h>

/*  Header pin layout

	Col 1 - 12V, GND, 5V
	Col 2 - Data, Bright, Latch(RCK), Clock(SRCK)

*/

const uint8_t ClockPin = PIN_PA05;
const uint8_t DataPin = PIN_PA07;
const uint8_t LatchPin = PIN_PA06;
const uint8_t BrightPin= PIN_PA04;

uint8_t digits[16] = {238,130,220,214,178,118,126,194,254,246,250,62,108,158,124,120};

struct tcc_module tcc_instance;  // required for the PWM signal on the BrightPin
#define PWM_MODULE TCC0
#define PWM_OUT_PIN PIN_PA04E_TCC0_WO0// PIN_PA04E_TC6_WO0 //PB02
#define PWM_OUT_MUX MUX_PA04E_TCC0_WO0  //MUX_PB02E_TC6_WO0
#define PWM_OUTPUT 0
#define PWM_CHANNEL 0


static void configure_tcc(void)
{
   struct tcc_config config_tcc;
   tcc_get_config_defaults(&config_tcc, PWM_MODULE);
   config_tcc.counter.period = 0xFFFF;
   config_tcc.compare.wave_generation = TCC_WAVE_GENERATION_SINGLE_SLOPE_PWM;
   config_tcc.compare.match[PWM_CHANNEL] = 0x7FFF;
   config_tcc.pins.enable_wave_out_pin[PWM_OUTPUT] = true;
   config_tcc.pins.wave_out_pin[PWM_OUTPUT]        = PWM_OUT_PIN;
   config_tcc.pins.wave_out_pin_mux[PWM_OUTPUT]    = PWM_OUT_MUX;
   tcc_init(&tcc_instance, PWM_MODULE, &config_tcc);
   tcc_enable(&tcc_instance);
}

void shftreg_init(void)
{
		struct port_config config_port_pin;

		// Outputs
		config_port_pin.direction = PORT_PIN_DIR_OUTPUT;
		port_pin_set_config(ClockPin, &config_port_pin);
		config_port_pin.direction = PORT_PIN_DIR_OUTPUT;
		port_pin_set_config(DataPin, &config_port_pin);
		config_port_pin.direction = PORT_PIN_DIR_OUTPUT;
		port_pin_set_config(LatchPin, &config_port_pin);
		config_port_pin.direction = PORT_PIN_DIR_OUTPUT;
		port_pin_set_config(BrightPin, &config_port_pin);	
		
	
		// Set latch to high
		port_pin_set_output_level(LatchPin, true);

		// Set up PWM and set BrightPin to half brightness
		configure_tcc();
		
}

static void shftreg_pulse()
{

	// pulse the shift clock
	port_pin_set_output_level(ClockPin, true);	
	delay_ms(3);	
	port_pin_set_output_level(ClockPin, false);
	delay_ms(3);
}

static void shftreg_latch()
{
	port_pin_set_output_level(LatchPin, false);
	delay_ms(3);
	port_pin_set_output_level(LatchPin, true);	
	delay_ms(3);
}


static void shtreg_write(uint8_t data)
{
	
	   //Send each 8 bits serially

	   
	   //Order is MSB first
	   for(uint8_t i=0;i<8;i++)
	   {

		   //Output the data on DS line according to the
		   //Value of MSB
		   if(data & 0b10000000)
		   {
			   //MSB is 1 so output high
			   port_pin_set_output_level(DataPin, true);	
		   }
		   else
		   {
			   //MSB is 0 so output high
			   port_pin_set_output_level(DataPin, false);
		   }

		   shftreg_pulse();  //Pulse the Clock line
		   data=data<<1;  //Now bring next bit at MSB position

	   }

	
}

void shftreg_display(uint8_t numerals[4])
{
	shtreg_write(digits[numerals[3]]);
	shtreg_write(digits[numerals[2]]);
	shtreg_write(digits[numerals[1]]);
	shtreg_write(digits[numerals[0]]);
	shftreg_latch();
	
}


void shftreg_blank(void)
{
	shtreg_write(0x00);
	shtreg_write(0x00);
	shtreg_write(0x00);
	shtreg_write(0x00);
	shftreg_latch();
	
	
}


void shftreg_bright(uint8_t brightness)
{
	tcc_set_compare_value(&tcc_instance, PWM_CHANNEL,(uint32_t)(brightness<<8)+0xFF);
	
}