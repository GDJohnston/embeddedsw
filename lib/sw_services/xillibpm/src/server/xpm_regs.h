/******************************************************************************
*
* Copyright (C) 2018-2019 Xilinx, Inc.  All rights reserved.
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
* XILINX  BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
* WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF
* OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*
* Except as contained in this notice, the name of the Xilinx shall not be used
* in advertising or otherwise to promote the sale, use or other dealings in
* this Software without prior written authorization from Xilinx.
*
* Naming Convention: <MODULE>_<REGISTER>[_<FIELD>[_<DESC>]]
*     <MODULE>       Module name (e.g. can or usb)
*     <REGISTER>     Register within the current module
*     [_<FIELD>]     Bit field within a register
*     [_<DESC>]      Type of bit field define:
*         SHIFT:     Least significant bit for the field
*         WIDTH:     Size of field in bites
*         MASK:      A masking over a range of bits or a bit to
*                    be used for setting or clearing
*
******************************************************************************/

#ifndef XPM_REGS_H
#define XPM_REGS_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Base Addresses
 */
#define PMC_GLOBAL_BASEADDR	 0xF1110000
#define PMC_ANALOG_BASEADDR	0XF1160000U
#define PMC_IOU_SLCR_BASEADDR 	 0XF1060000
#define LPD_IOU_SLCR_BASEADDR 	 0XFF080000
#define LPD_SLCR_BASEADDR 	 0XFF410000
#define LPD_SLCR_SECURE_BASEADDR 	 0XFF510000
#define FPD_SLCR_BASEADDR 	 0XFD610000
#define CPM_SLCR_BASEADDR 	 0XFCA10000
#define CFU_APB_BASEADDR	0XF12B0000
#define CFRAME0_REG_BASEADDR	0XF12D0000
#define CPM_PCSR_BASEADDR	0xFCFF0000
#define EFUSE_CACHE_BASEADDR      0XF1250000
#define GTY_NPI_SLAVE_0_BASEADDDRESS 0xF6890000
#define GTY_NPI_SLAVE_1_BASEADDDRESS 0xF68F0000
#define GTY_NPI_SLAVE_2_BASEADDDRESS 0xF6940000
#define GTY_NPI_SLAVE_3_BASEADDDRESS 0xF6990000
#define GTY_NPI_SLAVE_4_BASEADDDRESS 0xF69E0000
#define GTY_NPI_SLAVE_5_BASEADDDRESS 0xF6A30000
#define GTY_NPI_SLAVE_6_BASEADDDRESS 0xF6A90000
#define GTY_NPI_SLAVE_7_BASEADDDRESS 0xF70E0000
#define GTY_NPI_SLAVE_8_BASEADDDRESS 0xF7150000
#define GTY_NPI_SLAVE_9_BASEADDDRESS 0xF71B0000
#define GTY_NPI_SLAVE_10_BASEADDDRESS 0xF7210000

#define CFU_APB_CFU_FGCR    ( ( CFU_APB_BASEADDR ) + 0X00000018 )
#define CFU_APB_CFU_FGCR_INIT_COMPLETE_MASK    0X00000001
#define CFU_APB_CFU_FGCR_SC_HBC_TRIGGER_MASK    0X00002000
#define CFU_APB_CFU_MASK    ( ( CFU_APB_BASEADDR ) + 0X00000028 )
#define CFU_APB_CFU_MASK_INIT_COMPLETE_ENABLE_MASK    0x00000001
#define CFU_APB_CFU_STATUS    ( ( CFU_APB_BASEADDR ) + 0X00000100 )
#define CFU_APB_CFU_STATUS_SCAN_CLEAR_PASS_MASK    0X00002000
#define CFU_APB_CFU_STATUS_SCAN_CLEAR_DONE_MASK    0X00000008
#define CFU_APB_CFU_PROTECT    ( ( CFU_APB_BASEADDR ) + 0X00000014 )

#define CPM_PCSR_MASK	( ( CPM_PCSR_BASEADDR ) + 0X00000000 )
#define CPM_PCSR_MASK_SCAN_CLEAR_TRIGGER_WEN_MASK    0x00000800
#define CPM_PCSR_PCR	( ( CPM_PCSR_BASEADDR ) + 0X00000004)
#define CPM_PCSR_PCR_SCAN_CLEAR_TRIGGER_MASK    0X00000800
#define CPM_PCSR_PCR_MEM_CLEAR_TRIGGER_MASK    0X00040000
#define CPM_PCSR_PSR	( ( CPM_PCSR_BASEADDR ) + 0X00000008 )
#define CPM_PCSR_PSR_SCAN_CLEAR_DONE_MASK    0X00000002
#define CPM_PCSR_PSR_SCAN_CLEAR_PASS_MASK    0X00000004
#define CPM_PCSR_PSR_MEM_CLEAR_DONE_MASK    0X00000040
#define CPM_PCSR_PSR_MEM_CLEAR_PASS_MASK    0X00000080
#define CPM_PCSR_LOCK	( ( CPM_PCSR_BASEADDR ) + 0X0000000C )

#define GTY_PCSR_LOCK_OFFSET	0x0000000C
#define GTY_PCSR_MASK_OFFSET	0x00000000
#define GTY_PCSR_CONTROL_OFFSET	0x00000004
#define GTY_PCSR_STATUS_OFFSET  0x00000008
#define GTY_PCSR_INITCTRL_MASK			0x01000000
#define GTY_PCSR_MEM_CLEAR_TRIGGER_MASK		0x00040000
#define GTY_PCSR_BISR_TRIGGER_MASK		0x20000000
#define GTY_PCSR_STATUS_MEM_CLEAR_DONE_MASK	0x00000040
#define GTY_PCSR_STATUS_MEM_CLEAR_PASS_MASK	0x00000080
#define GTY_PCSR_STATUS_BISR_DONE_MASK		0x00080000
#define GTY_PCSR_STATUS_BISR_PASS_MASK		0x00100000


#define EFUSE_CACHE_TRIM_CFRM_VGG_0    ( ( EFUSE_CACHE_BASEADDR ) + 0X000001B4 )
#define EFUSE_CACHE_TRIM_CFRM_VGG_1    ( ( EFUSE_CACHE_BASEADDR ) + 0X000001B8 )
#define EFUSE_CACHE_TRIM_CFRM_VGG_2    ( ( EFUSE_CACHE_BASEADDR ) + 0X000001BC )
#define EFUSE_CACHE_TRIM_CRAM    ( ( EFUSE_CACHE_BASEADDR ) + 0X000001C0 )
#define EFUSE_CACHE_TRIM_BRAM    ( ( EFUSE_CACHE_BASEADDR ) + 0X00000098 )
#define EFUSE_CACHE_TRIM_URAM    ( ( EFUSE_CACHE_BASEADDR ) + 0X0000009C )
#define EFUSE_CACHE_BISR_RSVD_0		( ( EFUSE_CACHE_BASEADDR ) + 0X00000300 )
#define EFUSE_CACHE_TBITS1_BISR_RSVD	( ( EFUSE_CACHE_BASEADDR ) + 0X00000400 )
#define EFUSE_CACHE_TBITS2_BISR_RSVD	( ( EFUSE_CACHE_BASEADDR ) + 0X00000800 )

#define DOMAIN_ISO_CTRL_OFFSET		(0x10000U)

#define PMC_GLOBAL_PL_STATUS		( ( PMC_GLOBAL_BASEADDR ) + 0X00000880 )
#define PMC_GLOBAL_PL_STATUS_POR_PL_B_MASK 0x00000001

#define PMC_GLOBAL_PMC_GSW_ERR		( ( PMC_GLOBAL_BASEADDR ) + 0X00000064 )
#define PMC_GLOBAL_PMC_GSW_ERR_CR_FLAG_SHIFT	30
#define PMC_GLOBAL_DOMAIN_ISO_CNTRL_VCCAUX_VCCRAM_SHIFT   18
#define PMC_GLOBAL_DOMAIN_ISO_CNTRL_VCCAUX_VCCRAM_WIDTH   1
#define PMC_GLOBAL_DOMAIN_ISO_CNTRL_VCCAUX_VCCRAM_MASK    0X00040000
#define PMC_GLOBAL_DOMAIN_ISO_CNTRL_VCCRAM_SOC_SHIFT   17
#define PMC_GLOBAL_DOMAIN_ISO_CNTRL_VCCRAM_SOC_WIDTH   1
#define PMC_GLOBAL_DOMAIN_ISO_CNTRL_VCCRAM_SOC_MASK    0X00020000
#define PMC_GLOBAL_DOMAIN_ISO_CNTRL_VCCAUX_SOC_SHIFT   16
#define PMC_GLOBAL_DOMAIN_ISO_CNTRL_VCCAUX_SOC_WIDTH   1
#define PMC_GLOBAL_DOMAIN_ISO_CNTRL_VCCAUX_SOC_MASK    0X00010000
#define PMC_GLOBAL_DOMAIN_ISO_CNTRL_PL_SOC_SHIFT   15
#define PMC_GLOBAL_DOMAIN_ISO_CNTRL_PL_SOC_WIDTH   1
#define PMC_GLOBAL_DOMAIN_ISO_CNTRL_PL_SOC_MASK    0X00008000
#define PMC_GLOBAL_DOMAIN_ISO_CNTRL_PMC_SOC_SHIFT   14
#define PMC_GLOBAL_DOMAIN_ISO_CNTRL_PMC_SOC_WIDTH   1
#define PMC_GLOBAL_DOMAIN_ISO_CNTRL_PMC_SOC_MASK    0X00004000
#define PMC_GLOBAL_DOMAIN_ISO_CNTRL_PMC_SOC_NPI_SHIFT   13
#define PMC_GLOBAL_DOMAIN_ISO_CNTRL_PMC_SOC_NPI_WIDTH   1
#define PMC_GLOBAL_DOMAIN_ISO_CNTRL_PMC_SOC_NPI_MASK    0X00002000
#define PMC_GLOBAL_DOMAIN_ISO_CNTRL_PMC_PL_SHIFT   12
#define PMC_GLOBAL_DOMAIN_ISO_CNTRL_PMC_PL_WIDTH   1
#define PMC_GLOBAL_DOMAIN_ISO_CNTRL_PMC_PL_MASK    0X00001000
#define PMC_GLOBAL_DOMAIN_ISO_CNTRL_PMC_PL_TEST_SHIFT   11
#define PMC_GLOBAL_DOMAIN_ISO_CNTRL_PMC_PL_TEST_WIDTH   1
#define PMC_GLOBAL_DOMAIN_ISO_CNTRL_PMC_PL_TEST_MASK    0X00000800
#define PMC_GLOBAL_DOMAIN_ISO_CNTRL_PMC_PL_CFRAME_SHIFT   10
#define PMC_GLOBAL_DOMAIN_ISO_CNTRL_PMC_PL_CFRAME_WIDTH   1
#define PMC_GLOBAL_DOMAIN_ISO_CNTRL_PMC_PL_CFRAME_MASK    0X00000400
#define PMC_GLOBAL_DOMAIN_ISO_CNTRL_PMC_LPD_SHIFT   9
#define PMC_GLOBAL_DOMAIN_ISO_CNTRL_PMC_LPD_WIDTH   1
#define PMC_GLOBAL_DOMAIN_ISO_CNTRL_PMC_LPD_MASK    0X00000200
#define PMC_GLOBAL_DOMAIN_ISO_CNTRL_PMC_LPD_DFX_SHIFT   8
#define PMC_GLOBAL_DOMAIN_ISO_CNTRL_PMC_LPD_DFX_WIDTH   1
#define PMC_GLOBAL_DOMAIN_ISO_CNTRL_PMC_LPD_DFX_MASK    0X00000100
#define PMC_GLOBAL_DOMAIN_ISO_CNTRL_LPD_SOC_SHIFT   7
#define PMC_GLOBAL_DOMAIN_ISO_CNTRL_LPD_SOC_WIDTH   1
#define PMC_GLOBAL_DOMAIN_ISO_CNTRL_LPD_SOC_MASK    0X00000080
#define PMC_GLOBAL_DOMAIN_ISO_CNTRL_LPD_PL_SHIFT   6
#define PMC_GLOBAL_DOMAIN_ISO_CNTRL_LPD_PL_WIDTH   1
#define PMC_GLOBAL_DOMAIN_ISO_CNTRL_LPD_PL_MASK    0X00000040
#define PMC_GLOBAL_DOMAIN_ISO_CNTRL_LPD_PL_TEST_SHIFT   5
#define PMC_GLOBAL_DOMAIN_ISO_CNTRL_LPD_PL_TEST_WIDTH   1
#define PMC_GLOBAL_DOMAIN_ISO_CNTRL_LPD_PL_TEST_MASK    0X00000020
#define PMC_GLOBAL_DOMAIN_ISO_CNTRL_LPD_CPM_SHIFT   4
#define PMC_GLOBAL_DOMAIN_ISO_CNTRL_LPD_CPM_WIDTH   1
#define PMC_GLOBAL_DOMAIN_ISO_CNTRL_LPD_CPM_MASK    0X00000010
#define PMC_GLOBAL_DOMAIN_ISO_CNTRL_LPD_CPM_DFX_SHIFT   3
#define PMC_GLOBAL_DOMAIN_ISO_CNTRL_LPD_CPM_DFX_WIDTH   1
#define PMC_GLOBAL_DOMAIN_ISO_CNTRL_LPD_CPM_DFX_MASK    0X00000008
#define PMC_GLOBAL_DOMAIN_ISO_CNTRL_FPD_SOC_SHIFT   2
#define PMC_GLOBAL_DOMAIN_ISO_CNTRL_FPD_SOC_WIDTH   1
#define PMC_GLOBAL_DOMAIN_ISO_CNTRL_FPD_SOC_MASK    0X00000004
#define PMC_GLOBAL_DOMAIN_ISO_CNTRL_FPD_PL_SHIFT   1
#define PMC_GLOBAL_DOMAIN_ISO_CNTRL_FPD_PL_WIDTH   1
#define PMC_GLOBAL_DOMAIN_ISO_CNTRL_FPD_PL_MASK    0X00000002
#define PMC_GLOBAL_DOMAIN_ISO_CNTRL_FPD_PL_TEST_SHIFT   0
#define PMC_GLOBAL_DOMAIN_ISO_CNTRL_FPD_PL_TEST_WIDTH   1
#define PMC_GLOBAL_DOMAIN_ISO_CNTRL_FPD_PL_TEST_MASK    0X00000001

#define PMC_GLOBAL_ERR1_STATUS_OFFSET			(0X00020000U)
#define PMC_GLOBAL_ERR1_STATUS_DDRMC_MC_NCR_MASK	0X00080000
#define PMC_GLOBAL_ERR1_STATUS_NOC_TYPE_1_NCR_MASK	0X00002000


#define PMC_ANALOG_VGG_CTRL		(( PMC_ANALOG_BASEADDR ) + 0X0000000C )
#define PMC_ANALOG_VGG_CTRL_EN_VGG_CLAMP_MASK	0x00000001
/**
 * Register: PCSR_CONTROL
 */
#define NPI_PCSR_LOCK_OFFSET				(0X0000000CU)
#define NPI_PCSR_MASK_OFFSET				(0X00000000U)
#define NPI_PCSR_CONTROL_OFFSET				(0X00000004U)
#define NPI_PCSR_CONTROL_MEM_CLEAR_TRIGGER_MASK		0X00040000
#define NPI_PCSR_STATUS_OFFSET				(0X00000008U)
#define NPI_PCSR_STATUS_MEM_CLEAR_DONE_MASK			0X00000040
#define NPI_PCSR_STATUS_MEM_CLEAR_PASS_MASK			0X00000080

#define PMC_IOU_SLCR_WPROT0 	( ( PMC_IOU_SLCR_BASEADDR ) + 0X00000828 )
#define LPD_IOU_SLCR_WPROT0 	( ( LPD_IOU_SLCR_BASEADDR ) + 0X00000728 )

#define CPM_SLCR_BISR_CACHE_DATA_0 	( ( CPM_SLCR_BASEADDR ) + 0X000003F4 )
#define CPM_SLCR_BISR_CACHE_CTRL 	( ( CPM_SLCR_BASEADDR ) + 0X000003F0 )
#define CPM_SLCR_BISR_CACHE_STATUS 	( ( CPM_SLCR_BASEADDR ) + 0X000003EC )
#define CPM_SLCR_BISR_CACHE_CTRL_TRIGGER_MASK	0x00000001
#define CPM_SLCR_BISR_CACHE_STATUS_DONE_MASK	0x00000001
#define CPM_SLCR_BISR_CACHE_STATUS_PASS_MASK	0x00000002

/**
 * Register: LPD_SLCR_BISR
 */
#define LPD_SLCR_WPROT0 	( ( LPD_SLCR_BASEADDR ) + 0X00000000 )
#define	LPD_SLCR_BISR_CACHE_DATA_0 	( ( LPD_SLCR_BASEADDR ) + 0X0000010C )
#define LPD_SLCR_BISR_CACHE_CTRL_0		( ( LPD_SLCR_BASEADDR ) + 0x00000100U )
#define LPD_SLCR_CACHE_CTRL_0_BISR_TRIGGER_MASK	0X00000001U
#define LPD_SLCR_BISR_CACHE_CTRL_1		( ( LPD_SLCR_BASEADDR ) + 0x00000104U )
#define LPD_SLCR_CACHE_CTRL_1_PGEN0_MASK	0X00000001U
#define LPD_SLCR_CACHE_CTRL_1_PGEN1_MASK	0X00000002U
#define LPD_SLCR_BISR_CACHE_STATUS		( ( LPD_SLCR_BASEADDR ) + 0x00000108U )
#define LPD_SLCR_BISR_PASS_GLOBAL_MASK		(0X8000000U)
#define LPD_SLCR_BISR_DONE_GLOBAL_MASK		(0X4000000U)
#define LPD_SLCR_BISR_PASS_1_MASK		(0X0000008U)
#define LPD_SLCR_BISR_DONE_1_MASK		(0X0000004U)
#define LPD_SLCR_BISR_PASS_0_MASK		(0X0000002U)
#define LPD_SLCR_BISR_DONE_0_MASK		(0X0000001U)

/**
 * Register: LPD_SLCR_SECURE
 */
#define LPD_SLCR_SECURE_WPROT0 	( ( LPD_SLCR_SECURE_BASEADDR) + 0X00000000U )
#define LPD_SLCR_SECURE_ADMA_0	( ( LPD_SLCR_SECURE_BASEADDR) + 0X00000060U )

/**
 * Register: FPD_SLCR_BISR
 */
#define FPD_SLCR_WPROT0 	( ( FPD_SLCR_BASEADDR ) + 0X00000000 )
#define FPD_SLCR_BISR_CACHE_DATA_0 	( ( FPD_SLCR_BASEADDR ) + 0X0000040C )
#define FPD_SLCR_BISR_CACHE_CTRL_0		( ( FPD_SLCR_BASEADDR ) + 0x00000400U )
#define FPD_SLCR_CACHE_CTRL_0_BISR_TRIGGER_MASK	0X00000001U
#define FPD_SLCR_BISR_CACHE_CTRL_1		( ( LPD_SLCR_BASEADDR ) + 0x00000404U )
#define FPD_SLCR_CACHE_CTRL_1_PGEN0_MASK	0X00000001U
#define FPD_SLCR_CACHE_CTRL_1_PGEN1_MASK	0X00000002U
#define FPD_SLCR_CACHE_CTRL_1_PGEN2_MASK	0X00000004U
#define FPD_SLCR_CACHE_CTRL_1_PGEN3_MASK	0X00000008U
#define FPD_SLCR_BISR_CACHE_STATUS		( ( LPD_SLCR_BASEADDR ) + 0x00000408U )
#define FPD_SLCR_BISR_PASS_3_MASK		(0X0000200U)
#define FPD_SLCR_BISR_DONE_3_MASK		(0X0000100U)
#define FPD_SLCR_BISR_PASS_2_MASK		(0X0000080U)
#define FPD_SLCR_BISR_DONE_2_MASK		(0X0000040U)
#define FPD_SLCR_BISR_PASS_1_MASK		(0X0000020U)
#define FPD_SLCR_BISR_DONE_1_MASK		(0X0000010U)
#define FPD_SLCR_BISR_PASS_0_MASK		(0X0000008U)
#define FPD_SLCR_BISR_DONE_0_MASK		(0X0000004U)
#define FPD_SLCR_BISR_PASS_GLOBAL_MASK		(0X0000002U)
#define FPD_SLCR_BISR_DONE_GLOBAL_MASK		(0X0000001U)

#define APU_DUAL_RVBARADDR0L_OFFSET	0x40
#define APU_DUAL_RVBARADDR0H_OFFSET	0x44
#define APU_DUAL_RVBARADDR1L_OFFSET	0x48
#define APU_DUAL_RVBARADDR1H_OFFSET	0x4C

#define RPU_0_CFG_OFFSET		0x00000100U
#define RPU_1_CFG_OFFSET		0x00000200U
#define RPU_GLBL_CNTL_OFFSET		0x00000000U

#define CRL_PSM_RST_MODE_OFFSET		0x00000370U

#define FPD_APU_PWRCTL_OFFSET		0X00000090U

#define RPU_0_PWRDWN_OFFSET		0X00000108U
#define RPU_1_PWRDWN_OFFSET		0X00000208U

/* GIC Proxy base address */
#define PMC_GLOBAL_GIC_PROXY_BASE_OFFSET	0x30000
#define GIC_PROXY_GROUP_OFFSET(g)		(0x14U * (g))

/* GIC Proxy register offsets */
#define GIC_PROXY_IRQ_STATUS_OFFSET	0x0U
#define GIC_PROXY_IRQ_ENABLE_OFFSET	0x8U
#define GIC_PROXY_IRQ_DISABLE_OFFSET	0xCU

/**
 * Definitions required from pmc_tap.h
 */
#define PMC_TAP_BASEADDR      0XF11A0000
#define PMC_TAP_VERSION    ( ( PMC_TAP_BASEADDR ) + 0X00000004 )
#define PMC_TAP_VERSION_PLATFORM_SHIFT   24
#define PMC_TAP_VERSION_PLATFORM_MASK    0X0F000000

#ifdef __cplusplus
}
#endif

#endif /* XPM_REGS_H */
