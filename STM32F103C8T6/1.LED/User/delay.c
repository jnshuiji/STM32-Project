#include "delay.h"

void delay_start(void)
{
    SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8); // SysTick时钟源为HCLK/8
    SysTick->VAL = 0x00;                                  // 清空计数器
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;             // 使能，开始计数
    while (!(SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk)) // 等待时间到达
        ;
    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk; // 关闭计数器
}

/**
 * @description: 最多延时2^24/9约等于1,864,135us
 * @param {uint32_t} us
 *      @arg 1~1864135
 * @return {*}
 */
void delay_us(u32 us)
{
    SysTick->LOAD = SystemCoreClock / 8000000 * us; // 72MHz/8/1000/1000
    delay_start();
}

/**
 * @description: 最多延时2^24/9000约等于1,864ms
 * @param {uint32_t} ms
 *      @arg 1~1864
 * @return {*}
 */
void delay_ms(u16 ms)
{
    SysTick->LOAD = SystemCoreClock / 8000 * ms; // 72MHz/8/1000
    delay_start();
}
