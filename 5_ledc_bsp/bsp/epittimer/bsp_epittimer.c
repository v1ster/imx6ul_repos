#include "bsp_epittimer.h"
#include "bsp_int.h"
#include "bsp_led.h"

/**
 * Tout = ((frac +1 )* value) / Tclk;
 * Tclk：EPIT1 的输入时钟频率(单位 Hz)。 66MHz
 * Tout：EPIT1 的溢出时间(单位 S)。
 */ 
void epit1_init(unsigned int frac, unsigned int value)
{
    if (frac > 0xfff)
        frac = 0xfff;
    // 清零CRj寄存器
    EPIT1->CR = 0;
    /*
    * CR 寄存器:
    * bit25:24 01 时钟源选择 Peripheral clock=66MHz
    * bit15:4 frac 分频值
    * bit3: 1 当计数器到 0 的话从 LR 重新加载数值
    * bit2: 1 比较中断使能
    * bit1: 1 初始计数值来源于 LR 寄存器值
    * bit0: 0 先关闭 EPIT1
    */

   EPIT1->CR = (1<<24 | frac << 4 | 1<<3 | 1<<2 | 1<<1);
   EPIT1->LR = value;   /* 加载寄存器值 */
   EPIT1->CMPR = 0;     /* 比较寄存器值 */

   /* 使能 GIC 中对应的中断 */
   GIC_EnableIRQ(EPIT1_IRQn);
   /* 注册中断服务函数 */
   system_register_irqhandler(EPIT1_IRQn,(system_irq_handler_t)epit1_irqhandler,NULL);
   EPIT1->CR |= 1<<0;   /* 使能 EPIT1 */
}

void epit1_irqhandler(void)
{
    static unsigned char state = 0;
    state = !state;

    if(EPIT1->SR & (1<<0))  /* 判断比较事件发生 */
    {
        led_switch(LED0, state);
    }
    EPIT1->SR |= 1<<0;      /* 清除中断标志位 */
}