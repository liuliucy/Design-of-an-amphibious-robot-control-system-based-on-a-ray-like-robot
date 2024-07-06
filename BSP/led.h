#ifndef _LED_H_
#define _LED_H_

#define LED1 		GPIOE , GPIO_Pin_1
#define LED2 		GPIOE , GPIO_Pin_2
#define LED3 		GPIOE , GPIO_Pin_3

#define LED1_ON 		GPIO_ResetBits(GPIOE , GPIO_Pin_1)
#define LED2_ON 		GPIO_ResetBits(GPIOE , GPIO_Pin_2)
#define LED3_ON 		GPIO_ResetBits(GPIOE , GPIO_Pin_3)



#define LED1_OFF 		GPIO_SetBits(GPIOE , GPIO_Pin_1)
#define LED2_OFF 		GPIO_SetBits(GPIOE , GPIO_Pin_2)
#define LED3_OFF 		GPIO_SetBits(GPIOE , GPIO_Pin_3)

void LED_Configuration(void);

#endif
