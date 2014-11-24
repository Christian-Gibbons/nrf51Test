#include "board.h"
#include "nrf_delay.h"
#include "nrf_gpio.h"
#include "nrf.h"
//#include "clock.h"
#define GPIO_TOGGLE_PIN 0

void start_timer(void)
{
  NRF_TIMER2->MODE = TIMER_MODE_MODE_Timer;  // Set the timer in Counter Mode
  NRF_TIMER2->TASKS_CLEAR = 1;               // clear the task first to be usable for later
	NRF_TIMER2->PRESCALER = 6;
	NRF_TIMER2->BITMODE = TIMER_BITMODE_BITMODE_16Bit;//0x01UL;										//Set counter to 16 bit resolution
	NRF_TIMER2->CC[0] = 10000;
	NRF_TIMER2->CC[1] = 5000;

  // Enable interrupt on Timer 2
	NRF_TIMER2->INTENSET = (TIMER_INTENSET_COMPARE0_Enabled << TIMER_INTENSET_COMPARE0_Pos) | (TIMER_INTENSET_COMPARE1_Enabled << TIMER_INTENSET_COMPARE1_Pos);
  NVIC_EnableIRQ(TIMER2_IRQn);

  NRF_TIMER2->TASKS_START = 1;               // Start timer
}

/** Timer 2 peripheral interrupt handler.
 */
void TIMER2_IRQHandler(void)
{

	if ((NRF_TIMER2->EVENTS_COMPARE[1] != 0) && ((NRF_TIMER2->INTENSET & TIMER_INTENSET_COMPARE1_Msk) != 0))
  {
		NRF_TIMER2->EVENTS_COMPARE[1] = 0;

		nrf_gpio_pin_clear(GPIO_TOGGLE_PIN);
  }

  if ((NRF_TIMER2->EVENTS_COMPARE[0] != 0) && ((NRF_TIMER2->INTENSET & TIMER_INTENSET_COMPARE0_Msk) != 0))
  {
		NRF_TIMER2->EVENTS_COMPARE[0] = 0;

		nrf_gpio_pin_set(GPIO_TOGGLE_PIN);
  }
}
int main(void)
{
//	nrf_gpio_range_cfg_output(0,7);
	nrf_gpio_port_dir_set(NRF_GPIO_PORT_SELECT_PORT0, NRF_GPIO_PORT_DIR_OUTPUT);
	start_timer();
    while(1) {
    			__WFI();
#if 0
    	nrf_gpio_port_write(NRF_GPIO_PORT_SELECT_PORT0,GPIO_OUTSET_PIN0_High << GPIO_OUTSET_PIN0_Pos);
    	nrf_delay_us(500000);
    	nrf_gpio_port_write(NRF_GPIO_PORT_SELECT_PORT0,GPIO_OUTCLR_PIN0_Low << GPIO_OUTCLR_PIN0_Pos);
    	nrf_delay_us(500000);
#endif
    }
}

