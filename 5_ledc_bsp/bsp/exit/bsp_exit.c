#include "bsp_exit.h"
#include "bsp_beep.h"
#include "bsp_delay.h"
#include "bsp_gpio.h"
#include "bsp_int.h"

void exit_init(void)
{
    gpio_pin_config_t key_config;
    /* 1、设置 IO 复用 */
    IOMUXC_SetPinMux(IOMUXC_UART1_CTS_B_GPIO1_IO18, 0);
    IOMUXC_SetPinConfig(IOMUXC_UART1_CTS_B_GPIO1_IO18, 0xF080);
    /* 2、初始化 GPIO 为中断模式 */
    key_config.direction     = kGPIO_DigitalInput;
    key_config.interruptMode = kGPIO_IntFallingEdge;
    key_config.outputLogic   = 1;
    gpio_init(GPIO1, 18, &key_config);
    /* 3、使能 GIC 中断、注册中断服务函数、使能 GPIO 中断 */
    GIC_EnableIRQ(GPIO1_Combined_16_31_IRQn);
    system_register_irqhandler(GPIO1_Combined_16_31_IRQn, (system_irq_handler_t)gpio1_io18_irqhandler, NULL);
    gpio_enableint(GPIO1, 18);
}

void gpio1_io18_irqhandler(void)
{
    static unsigned char state = 0;
    /*
     *采用延时消抖，中断服务函数中禁止使用延时函数！因为中断服务需要
     *快进快出！！这里为了演示所以采用了延时函数进行消抖，后面我们会讲解
     *定时器中断消抖法！！！
     */
    delay(10);

    if(gpio_pinread(GPIO1, 18) == 0)    /* 按键按下了 */
    {
        state = !state;
        beep_switch(state);
    }

    gpio_clearintflags(GPIO1, 18);      /* 清除中断标志位 */
}