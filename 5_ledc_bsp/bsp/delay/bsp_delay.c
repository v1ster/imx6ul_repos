#include "bsp_delay.h"

void delay_short(volatile unsigned int n)
{
    while (n--)
        ;
}

void delay_init(void)
{
    GPT1->CR = 0;       /* 清零 */
    GPT1->CR = 1 << 15; /* bit15 置 1 进入软复位 */
    while ((GPT1->CR >> 15) & 0X01)
        ; /*等待复位完成 */

    /*
     * GPT 的 CR 寄存器,GPT 通用设置
     * bit22:20  000 输出比较 1 的输出功能关闭，也就是对应的引脚没反应
     * bit9:0    Restart 模式,当 CNT 等于 OCR1 的时候就产生中断
     * bit8:6    001 GPT 时钟源选择 ipg_clk=66Mhz
     */
    GPT1->CR = (1 << 6);

    /*
     * GPT 的 PR 寄存器，GPT 的分频设置
     * bit11:0   设置分频值，设置为 0 表示 1 分频，
     *           以此类推，最大可以设置为 0XFFF，也就是最大 4096 分频
     */
    GPT1->PR = 65; /* 66 分频，GPT1 时钟为 66M/(65+1)=1MHz */

    /*
     * GPT 的 OCR1 寄存器，GPT 的输出比较 1 比较计数值，
     * GPT 的时钟为 1Mz，那么计数器每计一个值就是就是 1us。
     * 为了实现较大的计数，我们将比较值设置为最大的 0XFFFFFFFF,
     * 这样一次计满就是：0XFFFFFFFFus = 4294967296us = 4295s = 71.5min
     * 也就是说一次计满最多 71.5 分钟，存在溢出。
     */
    GPT1->OCR[0] = 0XFFFFFFFF;
    GPT1->CR |= 1 << 0; /* 使能 GPT1 */

    /* 以下屏蔽的代码是 GPT 定时器中断代码，
     * 如果想学习 GPT 定时器的话可以参考以下代码。
     */

#if 0
    /*
     * GPT 的 PR 寄存器，GPT 的分频设置
     * bit11:0  设置分频值，设置为 0 表示 1 分频， 以此类推，最大可以设置为 0XFFF，也就是最大 4096 分频
     */

    GPT1->PR = 65; /* 66 分频，GPT1 时钟为 66M/(65+1)=1MHz */

    /*
     *GPT 的 OCR1 寄存器，GPT 的输出比较 1 比较计数值，
     * 当 GPT 的计数值等于 OCR1 里面值时候，输出比较 1 就会发生中断
     * 这里定时 500ms 产生中断，因此就应该为 1000000 / 2 = 500000;
     */

    GPT1->OCR[0] = 500000;

    /*
     * GPT 的 IR 寄存器，使能通道 1 的比较中断
     * bit0： 0 使能输出比较中断
     */
    GPT1->IR |= 1 << 0;

    /*
     * 使能 GIC 里面相应的中断，并且注册中断处理函数
     */
    GIC_EnableIRQ(GPT1_IRQn);
    /* 使能 GIC 中对应的中断 */
    system_register_irqhandler(GPT1_IRQn, (system_irq_handler_t)gpt1_irqhandler, NULL);

#endif
}

#if 0
/* 中断处理函数 */
void gpt1_irqhandler(void)
{
    static unsigned char state = 0;

    state = !state;
    /*
     * GPT 的 SR 寄存器，状态寄存器
     * bit2： 1 输出比较 1 发生中断
     */

    if (GPT1->SR & (1 << 0)) {
        led_switch(LED2, state);
    }
    GPT1->SR |= 1 << 0; /* 清除中断标志位 */
}
#endif

void delayus(unsigned int usdelay)
{
    unsigned long oldcnt, newcnt;
    unsigned long tcntvalue = 0; /* 走过的总时间 */

    oldcnt = GPT1->CNT;
    while (1) {
        newcnt = GPT1->CNT;
        if (newcnt != oldcnt) {
            if (newcnt > oldcnt) /* GPT 是向上计数器,并且没有溢出 */
                tcntvalue += newcnt - oldcnt;
            else
                tcntvalue += 0XFFFFFFFF - oldcnt + newcnt;
            oldcnt = newcnt;
            if (tcntvalue >= usdelay) /* 延时时间到了 */
                break;
        }
    }
}
void delayms(unsigned int msdelay)
{
    int i = 0;
    for(i=0;i <msdelay;i++)
    {
        delayus(1000);
    }
}

void delay(volatile unsigned int n)
{
    while (n--) {
        delay_short(0x7ff);
    }
}