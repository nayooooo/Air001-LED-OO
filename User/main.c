#include "main.h"

struct led led_group[3] = {
	[0] = {
		.name = "led_r",
		.port = LED_PORT_GPIOB,
		.pin = GPIO_PIN_1,
	},
	[1] = {
		.name = "led_g",
		.port = LED_PORT_GPIOB,
		.pin = GPIO_PIN_0,
	},
	[2] = {
		.name = "led_b",
		.port = LED_PORT_GPIOB,
		.pin = GPIO_PIN_3,
	},
};

int main(void)
{
	HAL_Init();
	
	led_dev_register(&led_group[0]);
	led_dev_register(&led_group[1]);
	led_dev_register(&led_group[2]);
	_led.open(&led_group[0]);
	_led.open(&led_group[1]);
	_led.open(&led_group[2]);
	
	while (1)
	{
		_led.write(&led_group[0], LED_ON);
		HAL_Delay(200);
		_led.write(&led_group[1], LED_ON);
		HAL_Delay(200);
		_led.write(&led_group[2], LED_ON);
		HAL_Delay(200);
		_led.write(&led_group[0], LED_OFF);
		HAL_Delay(200);
		_led.write(&led_group[1], LED_OFF);
		HAL_Delay(200);
		_led.write(&led_group[2], LED_OFF);
		HAL_Delay(200);
	}
}
