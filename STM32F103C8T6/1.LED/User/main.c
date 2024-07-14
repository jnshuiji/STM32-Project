#include <stm32f10x.h>
#include "delay.h"

int main(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); // 使能端口B的时钟

    // GPIO初始化结构体
    GPIO_InitTypeDef led_gpio;
    led_gpio.GPIO_Pin = GPIO_Pin_0;
    led_gpio.GPIO_Mode = GPIO_Mode_Out_PP; // Push-Pull推挽输出
    led_gpio.GPIO_Speed = GPIO_Speed_2MHz; // 2M时钟速度
    GPIO_Init(GPIOB, &led_gpio);
    while (1)
    {
        GPIO_ResetBits(GPIOB, GPIO_Pin_0);
        delay_ms(1000);
        GPIO_SetBits(GPIOB, GPIO_Pin_0);
        delay_ms(1000);
    }
}
