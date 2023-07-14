#ifndef __LED_H__
#define __LED_H__

/*==================================================
	includes
==================================================*/

/* air MCU ---------------------------------------*/

#include "air001xx_hal.h"

/*==================================================
	micros
==================================================*/

/* led -------------------------------------------*/

#ifndef LED_NULL
#	define LED_NULL		(0)
#endif  // LED_NULL

#define LED_NAME_MAX_LEN		(10)

// open_flag
#define LED_DEV_OPENED			(0X01)

// event_flag
#define LED_EVENT_INIT_SUCC		(0X80)

/* user ------------------------------------------*/

#define LED_PORT			LED_PORT_GPIOB
#define LED_R_PIN			GPIO_PIN_1
#define LED_G_PIN			GPIO_PIN_0
#define LED_B_PIN			GPIO_PIN_3

/*==================================================
	typedefs
==================================================*/

/* led -------------------------------------------*/

#define		LED_EOK			0
#define		LED_ERROR		1
#define		LED_EBUSY		2
#define		LED_EINVAL		3
#define		LED_ECLOSE		4
typedef int led_err_t;

typedef enum{
	LED_PORT_GPIOA = 0,
	LED_PORT_GPIOB,
	LED_PORT_GPIOF,
} led_port_t;

typedef enum{
	LED_ON			=  1,
	LED_OFF			=  0,
	LED_STATE_ERROR	= -1,
} led_state_t;

struct led{
	char name[LED_NAME_MAX_LEN];
	
	GPIO_InitTypeDef gpio;
	led_port_t port;
	uint32_t pin;
	
	uint8_t open_flag;
	uint8_t event_flag;
	
	led_err_t		(*init)			(struct led*, led_port_t, uint32_t, FunctionalState);
	led_err_t		(*open)			(struct led*);
	led_err_t		(*close)		(struct led*);
	led_state_t		(*read)			(struct led*);
	led_err_t		(*write)		(struct led*, led_state_t);
};
typedef struct led * led_t;

/*==================================================
	externs
==================================================*/

extern struct led _led;

led_err_t led_dev_register(led_t led);

#endif  /* __LED_H__ */
