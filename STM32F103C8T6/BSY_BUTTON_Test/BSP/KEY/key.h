#ifndef   __KEY_H_
#define   __KEY_H_

#include    "stm32f10x.h"


#define   KEY0_PORT       GPIOB
#define   KEY0_PIN        GPIO_Pin_12


#define   KEY1_PORT       GPIOB
#define   KEY1_PIN        GPIO_Pin_13


#define   KEY2_PORT       GPIOB
#define   KEY2_PIN        GPIO_Pin_14


#define   KEY3_PORT       GPIOB
#define   KEY3_PIN        GPIO_Pin_15



#define   KEY0      GPIO_ReadInputDataBit(KEY0_PORT, KEY0_PIN)

#define   KEY1      GPIO_ReadInputDataBit(KEY1_PORT, KEY1_PIN)

#define   KEY2      GPIO_ReadInputDataBit(KEY2_PORT, KEY2_PIN)

#define   KEY3      GPIO_ReadInputDataBit(KEY3_PORT, KEY3_PIN)



//创建UART运行任务
void Task_Key(void);


#endif



