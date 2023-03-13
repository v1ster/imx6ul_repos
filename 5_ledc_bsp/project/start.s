.global _start

/*
 * 描述： _start 函数，程序从此函数开始执行，此函数主要功能是设置 C
 * 运行环境。
 */

_start:

    ldr pc, =Reset_Handler          /* 复位中断 */
    ldr pc, =Undefined_Handler      /* 未定义指令中断 */
    ldr pc, =SVC_Handler            /* SVC(Supervisor)中断 */
    ldr pc, =PrefAbort_Handler      /* 预取终止中断 */
    ldr pc, =DataAbort_Handler      /* 数据终止中断 */
    ldr pc, =NotUsed_Handler        /* 未使用中断 */
    ldr pc, =IRQ_Handler            /* IRQ 中断 */
    ldr pc, =FIQ_Handler            /* FIQ(快速中断)未定义中断 */
    /* 进入 SVC 模式 */
    mrs r0, cpsr
    bic r0, r0, #0x1f       /* 将 r0 的低 5 位清零，也就是 cpsr 的 M0~M4  bic 位清除 */ 
    orr r0, r0, #0x13       /* r0 或上 0x13,表示使用 SVC 模式  orr  位或 */
    msr cpsr, r0            /* 将 r0 的数据写入到 cpsr_c 中 */
    ldr sp, =0x80200000     /* 设置栈指针 */
    b main                  /* 跳转到 main 函数 */
    
Reset_Handler:
    /* 复位中断具体处理过程 */
    cpsid i         /* 关闭全局中断 */
    /* 关闭 I,DCache 和 MMU
     * 采取读-改-写的方式。
     */
    mrc     p15, 0, r0, c1, c0, 0 /* 读取 CP15 的 C1 寄存器到 R0 中 */
    bic     r0, r0, #(0x1 << 12) /* 清除 C1 的 I 位，关闭 I Cache */
    bic     r0, r0, #(0x1 <<  2) /* 清除 C1 的 C 位，关闭 D Cache */
    bic     r0, r0, #02          /* 清除 C1 的 A 位，关闭对齐检查 */
    bic     r0, r0, #(0x1 << 11) /* 清除 C1 的 Z 位，关闭分支预测 */
    bic     r0, r0, #0x1         /* 清除 C1 的 M 位，关闭 MMU */
    mcr     p15, 0, r0, c1, c0, 0 /* 将 r0 的值写入到 CP15 的 C1 中 */

#if 0
    /* 汇编版本设置中断向量表偏移 */
    ldr r0, =0X87800000

    dsb     /* 数据同步隔离。比 DMB 严格： 仅当所有在它前面的存储器访问操作 都执行完毕后，才执行在它后面的指令（亦即任何指令都要等待存储器访 问操作——译者注）*/
    isb     /* 指令同步隔离。最严格：它会清洗流水线，以保证所有它前面的指令都执行完毕之后，才执行它后面的指令。*/
    mcr p15, 0, r0, c12, c0, 0      /* 设置向量表基地址寄存器 */
    dsb
    isb 
#endif

    /* 设置各个模式下的栈指针，
     * 注意：IMX6UL 的堆栈是向下增长的！
     * 堆栈指针地址一定要是 4 字节地址对齐的！！！
     * DDR 范围:0X80000000~0X9FFFFFFF 或者 0X8FFFFFFF
     */
     /* 进入 IRQ 模式 */
     mrs r0, cpsr
     bic r0, r0, #0x1f      /* 将 r0 的低 5 位清零，也就是 cpsr 的 M0~M4 */
     orr r0, r0, #0x12      /* r0 或上 0x12,表示使用 IRQ 模式 */
     msr cpsr, r0           /* 将 r0 的数据写入到 cpsr 中 */
     ldr sp, =0x80600000    /* IRQ 模式栈首地址为 0X80600000,大小为 2MB */

     /* 进入 SYS 模式 */
     mrs r0, cpsr
     bic r0, r0, #0x1f      /* 将 r0 的低 5 位清零，也就是 cpsr 的 M0~M4 */
     orr r0, r0, #0x1f      /* r0 或上 0x1f,表示使用 SYS 模式 */
     msr cpsr, r0           /* 将 r0 的数据写入到 cpsr 中 */
     ldr sp, =0x80400000    /* SYS 模式栈首地址为 0X80400000,大小为 2MB */

     /* 进入 SVC 模式 */
     mrs r0, cpsr
     bic r0, r0, #0x1f      /* 将 r0 的低 5 位清零，也就是 cpsr 的 M0~M4 */
     orr r0, r0, #0x13      /* r0 或上 0x13,表示使用 SVC 模式 */
     msr cpsr, r0           /* 将 r0 的数据写入到 cpsr 中 */
     ldr sp, =0X80200000    /* SVC 模式栈首地址为 0X80200000,大小为 2MB */

     cpsie i                /* 打开全局中断 */

#if 0
    /* 使能 IRQ 中断 */
    mrs r0, cpsr            /* 读取 cpsr 寄存器值到 r0 中 */
    bic r0, r0, #0x80       /* 将 r0 寄存器中 bit7 清零，也就是 CPSR 中
                             * 的 I 位清零，表示允许 IRQ 中断
                             */
    msr cpsr, r0            /* 将 r0 重新写入到 cpsr 中 */
#endif
    b main                  /* 跳转到 main 函数 */

/* 未定义中断 */
Undefined_Handler:
    ldr r0, =Undefined_Handler
    bx r0

/* SVC 中断 */
SVC_Handler:
    ldr r0, =SVC_Handler
    bx r0

/* 预取终止中断 */
PrefAbort_Handler:
    ldr r0, =PrefAbort_Handler
    bx r0

/* 数据终止中断 */
DataAbort_Handler:
    ldr r0, =DataAbort_Handler
    bx r0

/* 未使用的中断 */
NotUsed_Handler:
    ldr r0, =NotUsed_Handler
    bx r0

/* IRQ 中断！重点！！！！！ */
IRQ_Handler:
    /* 复位中断具体处理过程  */
    push {lr}           /* 保存 lr 地址 */
    push {r0-r3, r12}   /* 保存 r0-r3，r12 寄存器 */

    mrs r0, spsr        /* 读取 spsr 寄存器 */
    push {r0}           /* 保存 spsr 寄存器 */
    mrc p15, 4, r1, c15, c0, 0 /* 将 CP15 的 C0 内的值到 R1 寄存器中
                                * 参考文档 ARM Cortex-A(armV7)编程手册 V4.0.pdf P49
                                * Cortex-A7 Technical ReferenceManua.pdf P68 P138
                                */
    add r1, r1, #0X2000     /* GIC 基地址加 0X2000，得到 CPU 接口端基地址 */
    ldr r0, [r1, #0XC]      /* CPU 接口端基地址加 0X0C 就是 GICC_IAR 寄存器，
                             * GICC_IAR 保存着当前发生中断的中断号，我们要根据
                             * 这个中断号来绝对调用哪个中断服务函数
                             */
    push {r0, r1}           /* 保存 r0,r1 */ 
    cps #0x13               /* 进入 SVC 模式，允许其他中断再次进去 */
    push {lr}               /* 保存 SVC 模式的 lr 寄存器 */
    ldr r2, =system_irqhandler  /* 加载 C 语言中断处理函数到 r2 寄存器中*/
    blx r2                  /* 运行 C 语言中断处理函数，带有一个参数 */

    pop {lr}                /* 执行完 C 语言中断服务函数，lr 出栈 */
    cps #0x12               /* 进入 IRQ 模式 */
    pop {r0, r1}            
    str r0, [r1, #0X10]     /* 中断执行完成，写 EOIR */

    pop {r0}
    msr spsr_cxsf, r0       /* 恢复 spsr */

    pop {r0-r3, r12}        /* r0-r3,r12 出栈 */
    pop {lr}                /* lr 出栈 */
    subs pc, lr, #4         /* 将 lr-4 赋给 pc */

/* FIQ 中断 */
FIQ_Handler:
    ldr r0, =FIQ_Handler
    bx r0
    