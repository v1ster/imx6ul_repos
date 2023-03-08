#include "bsp_int.h"

/* 中断嵌套计数器 */
static unsigned int irqNesting;

/* 中断服务函数表 */
static sys_irq_handle_t irqTable[NUMBER_OF_INT_VECTORS];

void init_init(void)
{
    GIC_Init();             /* 初始化 GIC */
    system_irqtable_init(); /* 初始化中断表 */
    __set_VBAR((uint32_t)0x87800000);   /* 中断向量表偏移 */
}

void system_irqtable_init(void)
{
    unsigned int i = 0;
    irqNesting = 0;
    /* 先将所有的中断服务函数设置为默认值 */
    for(i = 0; i < NUMBER_OF_INT_VECTORS; i++)
    {
        system_register_irqhandler((IRQn_Type)i,default_irqhandler,NULL);
    }
}

void system_register_irqhandler(IRQn_Type irq,system_irq_handler_t handler,void *userParam)
{
    irqTable[irq].irqHandler = handler;
    irqTable[irq].userParam = userParam;
}

void system_irqhandler(unsigned int giccIar)
{
    uint32_t intNum = giccIar & 0x3FFUL;
    /* 检查中断号是否符合要求 */
    if ((intNum == 1020) || (intNum >= NUMBER_OF_INT_VECTORS))
    {
        return ;
    }
    irqNesting++;   /* 中断嵌套计数器加一 */

    /* 根据传递进来的中断号，在 irqTable 中调用确定的中断服务函数*/
    irqTable[intNum].irqHandler(intNum, irqTable[intNum].userParam);
    irqNesting--; /* 中断执行完成，中断嵌套寄存器减一 */
}

void default_irqhandler(unsigned int giccIar, void *userParam)
{
    while(1)
    {
        
    }
}