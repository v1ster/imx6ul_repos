
.global _start /*全局标号*/

_start:
ldr r0, =0x020c4068     /* 寄存器 CCGR0 */
ldr r1, =0xffffffff
str r1, [r0]

ldr r0, =0x020c405c     /* 寄存器 CCGR1 */
str r1, [r0]

ldr r0, =0X020C4070     /* 寄存器 CCGR2 */
str r1, [r0]

ldr r0, =0X020C4074     /* 寄存器 CCGR3 */
str r1, [r0]

ldr r0, =0X020C4078     /* 寄存器 CCGR4 */
str r1, [r0]

ldr r0, =0X020C407C     /* 寄存器 CCGR5 */
str r1, [r0]

ldr r0, =0X020C4080     /* 寄存器 CCGR6 */
str r1, [r0]

ldr r0, =0x020e0068
ldr r1, =0x05
str r1, [r0]

ldr r0, =0x020e02f4
ldr r1, =0x10b0
str r1, [r0]

ldr r0, =0x0209c004
ldr r1, =0x00000008
str r1, [r0]

ldr r0, =0x0209c000
ldr r1, =0
str r1, [r0]

loop:
    b loop
    