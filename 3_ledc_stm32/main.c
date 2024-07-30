#include "imx6u.h"

/* 使能外设时钟 */
void clk_enable(void)
{
    CCM->CCGR0 = 0xFFFFFFFF;
    CCM->CCGR1 = 0xFFFFFFFF;
    CCM->CCGR2 = 0xFFFFFFFF;
    CCM->CCGR3 = 0xFFFFFFFF;
    CCM->CCGR4 = 0xFFFFFFFF;
    CCM->CCGR5 = 0xFFFFFFFF;
    CCM->CCGR6 = 0xFFFFFFFF;
}

void delay_short(volatile unsigned int n)
{
    while(n--);
}

/* 1ms */
void delay(volatile unsigned int n)
{
    while (n--)
    {
        delay_short(0x7ff);
    }
    
}

void led_on(void)
{
    GPIO1->DR &= ~(1 << 3);
}

void led_off(void)
{
    GPIO1->DR |= (1 << 3);
}

/* 初始化LED */
void led_init(void)
{
    IOMUX_SW_MUX->GPIO1_IO03 = 0x5;     //复用为GPIO_IO3
    IOMUX_SW_PAD->GPIO1_IO03 = 0x10B0;  //设置电气属性

    /* GPIO初始化 */
    GPIO1->GDIR = 0x8;   //设置为输出
    GPIO1->DR   = 0x0;   //打开LED灯
}

int main(void)
{
    clk_enable();   //使能外设时钟
    led_init();
    while(1){
        led_on();
        delay(200);
        led_off();
        delay(200);
    }
    return 0;
}