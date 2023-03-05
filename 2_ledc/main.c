#include "main.h"

/**
 * @description : 使能 I.MX6U 所有外设时钟
 * @param       : 无
 * @return      : 无
 */
void clk_enable(void)
{
    CCM_CCGR0 = 0XFFFFFFFF;
    CCM_CCGR1 = 0XFFFFFFFF;
    CCM_CCGR2 = 0XFFFFFFFF;
    CCM_CCGR3 = 0XFFFFFFFF;
    CCM_CCGR4 = 0XFFFFFFFF;
    CCM_CCGR5 = 0XFFFFFFFF;
    CCM_CCGR6 = 0XFFFFFFFF;
}

void led_init(void)
{
    /* 1, 初始化IO复用, 复用在GPIO1_IO03 */
    SW_MUX_GPIO1_IO03 = 0X5;

    /*  2, 配置GPIO1_IO03 的IO属性
     * bit 16:0 HYS 关闭
     * bit [15:14]: 00 默认下拉
     * bit [13]: 0 kepper 功能
     * bit [12]: 1 pull/keeper 使能
     * bit [11]: 0 关闭开路输出
     * bit [7:6]: 10 速度100Mhz
     * bit [5:3]: 110 R0/6 驱动能力
     * bit [0]: 0 低转换率
     */

    SW_PAD_GPIO1_IO03 = 0x10b0;

    /* 3,初始化GPIO, GPIO1_IO03 设置为输出 */
    GPIO1_GDIR = 0x0000008;

    /* 4, 设置GPIO_IO03输出低电平, 打开 LED0 */
    GPIO1_DR = 0x0;
}

void led_on(void)
{
    GPIO1_DR &= ~(1 << 3);
}

void led_off(void)
{
    GPIO1_DR |= (1 << 3);
}

void delay_short(volatile unsigned int n)
{
    while (--n)
        ;
}

/**
 * @description : 延时函数,在395Mhz 的主频下延时时间大约为 1ms
 * @param       : 要延迟的ms数
 * @return      : 无
*/

void delay(volatile unsigned int n)
{
    while (--n) {
        delay_short(0x7ff);
    }
}

int main(void)
{
    clk_enable();
    led_init();

    while(1)
    {
        led_off();
        delay(500);

        led_on();
        delay(500);
    }
    return 0;
}