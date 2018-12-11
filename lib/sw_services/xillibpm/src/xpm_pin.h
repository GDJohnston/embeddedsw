/******************************************************************************
*
* Copyright (C) 2018 Xilinx, Inc.  All rights reserved.
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
******************************************************************************/

#ifndef XPM_PIN_H_
#define XPM_PIN_H_

#include "xpm_node.h"
#include "xpm_pinfunc.h"
#include "xpm_device.h"
#include "xillibpm_api.h"

/* IOU_SLCR register related macros */
#define BITS_IN_REG			(32)
#define PINS_PER_BANK			(26)
#define BNK_OFFSET			(0x200U)
#define PINNUM(Id)	   		((NODEINDEX(Id) - XPM_NODEIDX_STMIC_LMIO_0) % PINS_PER_BANK)
#define SEL_SLEW			(0x00000120U)
#define EN_WK_PD			(0x00000110U)
#define EN_WK_PU			(0x00000114U)
#define EN_RX_SCHMITT_HYST		(0x0000010CU)
#define SEL_DRV0			(0x00000118U)
#define SEL_DRV1			(0x0000011CU)
#define SEL_DRV_WIDTH			(2U)
#define SEL_DRV0_MASK(PinIdx)		(0x3 << (PINNUM(PinIdx)))
#define SEL_DRV1_MASK(PinIdx)		(0x3 << (PINNUM(PinIdx) - (BITS_IN_REG / SEL_DRV_WIDTH)))
#define VMODE				(0x0000015CU)
#define VMODE_MASK			(0x1U)
#define TRI_STATE			(0x200U)

/* Pin states */
typedef enum {
	XPM_PINSTATE_UNUSED,
	XPM_PINSTATE_ASSIGNED,
} XPm_PinState;

typedef struct XPm_PinNode XPm_PinNode;

/**
 * The Pin class.
 */
struct XPm_PinNode {
	XPm_Node Node; /**< Node: Base class */
	u8 Bank; /**< Specifies the bank number */
	u8 BiasStatus; /**< BiasStatus: 0 – Disable; 1 – Enable */
	u8 PullCtrl; /**< PullCtrl: 0 – Pull Down; 1 – Pull Up */
	u8 TriState; /**< TriState: 0 – Disable; 1 – Enable */
	XPm_PinFunc *PinFunc; /**< Function that this pin is allocated to */
	u16 NumGroups; /**< Number of function groups allocated to this pin */
	u16 *Groups; /**< Array of group identifier supported by this pin */
	u32 SubsysId;  /**< SubsystemId of the owner who is using this pin */
};

extern XPm_PinNode *PmMioPins[XPM_NODEIDX_STMIC_MAX];
extern u16 PmNumPins;

/************************** Function Prototypes ******************************/
XStatus XPmPin_Init(XPm_PinNode *Pin, u32 Id, u32 BaseAddress);
XPm_PinNode *XPmPin_GetById(u32 PinId);

XStatus XPmPin_SetPinFunction(u32 PinId, u32 FuncId);
XStatus XPmPin_GetPinFunction(u32 PinId, u32 *FuncId);
XStatus XPmPin_SetPinConfig(u32 PinId, u32 Param, u32 Value);
XStatus XPmPin_GetPinConfig(u32 PinId, u32 Param, u32 *Value);
XStatus XPmPin_GetNumPins(u32 *NumPins);
XStatus XPmPin_GetPinGroups(u32 PinId, u32 Index, u16 *Groups);
XStatus XPmPin_CheckPerms(const u32 SubsystemId, const u32 PinId);
XStatus XPmPin_Release(const u32 SubsystemId, const u32 PinId);
XStatus XPmPin_Request(const u32 SubsystemId, const u32 PinId);

/** @} */
#endif /* XPM_PIN_H_ */
