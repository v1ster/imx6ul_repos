#include "bsp_keyfilter.h"
#include "bsp_beep.h"
#include "bsp_gpio.h"
#include "bsp_int.h"
#include "bsp_key.h"

void filterkey_init(void)
{
    gpio_pin_config_t key_config;

    /* 1、初始化 IO */
    IOMUXC_SetPinMux(IOMUXC_UART1_CTS_B_GPIO1_IO18, 0);
    IOMUXC_SetPinConfig(IOMUXC_UART1_CTS_B_GPIO1_IO18, 0xF080);

    /* 2、初始化 GPIO 为中断 */
    key_config.direction     = kGPIO_DigitalInput;
    key_config.interruptMode = kGPIO_IntFallingEdge;
    key_config.outputLogic   = 1;
    gpio_init(GPIO1, 18, &key_config);

    /* 3、 使能 GPIO 中断，并且注册中断处理函数 */
    GIC_EnableIRQ(GPIO1_Combined_16_31_IRQn);
    system_register_irqhandler(GPIO1_Combined_16_31_IRQn, (system_irq_handler_t)gpio1_16_31_irqhandler, NULL);
    gpio_enableint(GPIO1, 18);        /* 使能 GPIO1_IO18 的中断功能*/
    filtertimer_init(66000000 / 100); /* 初始化定时器,10ms */
}

void filtertimer_init(unsigned int value)
{
    EPIT1->CR   = 0;
    EPIT1->CR   = (1 << 24 | 1 << 3 | 1 << 2 | 1 << 1);
    EPIT1->LR   = value;
    EPIT1->CMPR = 0;
    /* 使能 EPIT1 中断并注册中断处理函数*/
    GIC_EnableIRQ(EPIT1_IRQn);
    system_register_irqhandler(EPIT1_IRQn, (system_irq_handler_t)filtertimer_irqhandler, NULL);
}

void filtertimer_stop(void)
{
    EPIT1->CR &= ~(1 << 0);
}

void filtertimer_restart(unsigned int value)
{
    EPIT1->CR &= ~(1 << 0); /* 先关闭定时器 */
    EPIT1->LR = value;
    EPIT1->CR |= (1 << 0); /* 打开定时器 */
}

void filtertimer_irqhandler(void)
{
    static unsigned char state = OFF;
    if (EPIT1->SR & (1 << 0)) {
        filtertimer_stop();
        if (gpio_pinread(GPIO1, 18) == 0) {
            state = !state;
            beep_switch(state);
        }
    }
    EPIT1->SR |= 1 << 0;
}

void gpio1_16_31_irqhandler(void)
{
    filtertimer_restart(66000000 / 100); /* 开启定时器 */
    gpio_clearintflags(GPIO1, 18);       /* 清除中断标志位*/
}