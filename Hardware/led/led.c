#include "led.h"

#include <string.h>

struct led _led;

static GPIO_TypeDef *led_port_to_type(led_port_t port)
{
	GPIO_TypeDef *type;
	
	switch (port)
	{
	case LED_PORT_GPIOA:
		type = GPIOA;
		break;
	case LED_PORT_GPIOB:
		type = GPIOB;
		break;
	case LED_PORT_GPIOF:
		type = GPIOF;
		break;
	default:
		return LED_NULL;
	}
	
	return type;
}

/**
 * @brief 将LED的状态转化成PIN的状态
 */
static GPIO_PinState led_state_led_to_pin(led_state_t state)
{
	GPIO_PinState hal_state;
	
	if (state == LED_ON) hal_state = GPIO_PIN_SET;
	else if (state == LED_OFF) hal_state = GPIO_PIN_RESET;
	else hal_state = -1;
	
	return hal_state;
}

static led_err_t led_init(led_t led, led_port_t led_port, uint32_t pin, FunctionalState state)
{
	led->gpio.Mode = GPIO_MODE_OUTPUT_PP;
	led->gpio.Speed = GPIO_SPEED_FREQ_LOW;
	led->gpio.Pin = pin;
	
	GPIO_TypeDef *gpiox = led_port_to_type(led_port);
	switch (led_port)
	{
	case LED_PORT_GPIOA:
		if (state == ENABLE) __HAL_RCC_GPIOA_CLK_ENABLE();
		else __HAL_RCC_GPIOA_CLK_DISABLE();
		break;
	case LED_PORT_GPIOB:
		if (state == ENABLE) __HAL_RCC_GPIOB_CLK_ENABLE();
		else __HAL_RCC_GPIOB_CLK_DISABLE();
		break;
	case LED_PORT_GPIOF:
		if (state == ENABLE) __HAL_RCC_GPIOF_CLK_ENABLE();
		else __HAL_RCC_GPIOF_CLK_DISABLE();
		break;
	default:
		return -LED_ERROR;
	}
	HAL_GPIO_Init(gpiox, &led->gpio);
	
	return LED_EOK;
}

static led_err_t led_open(led_t led)
{
	led->open_flag |= LED_DEV_OPENED;
	
	return LED_EOK;
}

static led_err_t led_close(led_t led)
{
	led->open_flag &= ~LED_DEV_OPENED;
	
	return LED_EOK;
}

static led_state_t led_read(led_t led)
{
	if (!(led->open_flag & LED_DEV_OPENED)) return -LED_ECLOSE;
	
	led_state_t state;
	
	GPIO_TypeDef *gpiox = led_port_to_type(led->port);
	GPIO_PinState hal_state = HAL_GPIO_ReadPin(gpiox, led->gpio.Pin);
	
	if (hal_state == GPIO_PIN_SET) state = LED_ON;
	else if (hal_state == GPIO_PIN_RESET) state = LED_OFF;
	else state = LED_STATE_ERROR;
	
	return state;
}

static led_err_t led_write(led_t led, led_state_t state)
{
	if (!(led->open_flag & LED_DEV_OPENED)) return -LED_ECLOSE;
	
	GPIO_PinState hal_state;
	
	GPIO_TypeDef *gpiox = led_port_to_type(led->port);
	hal_state = led_state_led_to_pin(state);
//	if (hal_state == -1) return -LED_EINVAL;
	HAL_GPIO_WritePin(gpiox, led->gpio.Pin, hal_state);
	
	return LED_EOK;
}

led_err_t led_dev_register(led_t led)
{
	if (led == LED_NULL) return -LED_EINVAL;
	
	memset(&led->gpio, 0, sizeof(led->gpio));
	led->open_flag	= 0X00;
	led->event_flag	= 0X00;
	
	if (!(_led.event_flag & LED_EVENT_INIT_SUCC)) {
		memcpy(_led.name, "vptr", 4);
		memset(&_led.gpio, 0, sizeof(_led.gpio));
		_led.port = 0;
		_led.pin = 0;
		
		_led.init	= led_init;
		_led.open	= led_open;
		_led.close	= led_close;
		_led.read	= led_read;
		_led.write	= led_write;
		
		_led.open_flag |= LED_DEV_OPENED;
		_led.event_flag |= LED_EVENT_INIT_SUCC;  // 已经初始化了_led
	}
	
	led->init	= _led.init;
	led->open	= _led.open;
	led->close	= _led.close;
	led->read	= _led.read;
	led->write	= _led.write;
	
	led->init(led, led->port, led->pin, ENABLE);
	
	return LED_EOK;
}
