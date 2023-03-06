#ifndef __MCIMX6Y2_H
#define __MCIMX6Y2_H
/**
 *  IOMUX 寄存器
 */
typedef struct
{
    volatile unsigned int BOOT_MODE0;
    volatile unsigned int BOOT_MODE1;
    volatile unsigned int SNVS_TAMPER0;
    volatile unsigned int SNVS_TAMPER1;

} IOMUX_SW_MUX_Tpye;

#define IOMUX_SW_MUX_BASE (0x020e0014)
#define IOMUX_SW_MUX ((IOMUX_SW_MUX_Tpye *)IOMUX_SW_MUX_BASE)
#endif