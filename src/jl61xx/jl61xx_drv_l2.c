/*
 * Copyright (c) 2014-2023 JLSemi Limited
 * All Rights Reserved
 *
 * THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE of JLSemi Limited
 * The copyright notice above does not evidence any actual or intended
 * publication of such source code.
 *
 * No part of this code may be reproduced, stored in a retrieval system,
 * or transmitted, in any form or by any means, electronic, mechanical,
 * photocopying, recording, or otherwise, without the prior written
 * permission of JLSemi Limited
 */
#include "jl61xx/jl61xx_drv_l2.h"
#include "jl61xx/jl61xx_drv_switch.h"
#include "jl61xx/jl61xx_chip.h"
#include "jl61xx/jl61xx_drv_switch.h"

/****Macro Define****/

/****Type Define****/
static jl_int32 g_l2_tbl_search_idx[JL_L2_SEL_MODE_END] = {-1, -1, -1, -1, -1};

static inline jl_uint32 __l2_hash(jl_mac_addr_t *addr, jl_uint32 FID)
{
	jl_uint64 key;

	key = (((jl_uint64) FID) << 48) | ((jl_uint64) addr->addr[5] + ((jl_uint64) addr->addr[4] << 8) +
			((jl_uint64) addr->addr[3] << 16) + ((jl_uint64) addr->addr[2] << 24) +
			((jl_uint64) addr->addr[1] << 32)  + ((jl_uint64) addr->addr[0] << 40));

	jl_uint64 hashval = key;
	jl_uint64 hash_size = L2_HASH_WIDTH;

	while (hashval &= L2_HASH_XOR_FACTOR, hash_size < L2_HASH_INPUT_LENGTH) {
		hashval = hashval ^ (key >> hash_size);
		hash_size += L2_HASH_WIDTH;
	}
	JL_DBG_MSG(JL_FLAG_SWCORE, _DBG_INFO, "hash val is %llx, size is %llx\n", hashval, hash_size);
	return hashval;
}

static jl_ret_t _jl61xx_l2_hash_check(
	jl_device_t *pdevice,
	jl_l2_tbl_t *pcfg,
	jl_uint32 hash_val,
	jl_uint32 idx)
{
	jl_ret_t ret = JL_ERR_OK;
	jl_uint32 mac_31 = 0;
	jl_uint16 mac_32_47 = 0;

	SWCORE_L2_DA_HASH_LOOKUP_TABLE_t lookupTbl;

	REG_BURST_READ(pdevice, SWCORE, L2_DA_HASH_LOOKUP_TABLE, hash_val + (idx << L2_HASH_WIDTH), INDEX_ZERO, lookupTbl.val);

	mac_31 = pcfg->mac.addr[5] + BITS_OFFSET_L(pcfg->mac.addr[4], 8, 8) +
	    BITS_OFFSET_L(pcfg->mac.addr[3], 16, 8) + BITS_OFFSET_L(pcfg->mac.addr[2], 24, 8);
	mac_32_47 = BITS_OFFSET_L(pcfg->mac.addr[1], 0, 8) + BITS_OFFSET_L(pcfg->mac.addr[0], 8, 8);

	if ((lookupTbl.BF.gid == pcfg->fid) &&
		(lookupTbl.BF.mac_addr_0_31 == mac_31) &&
		(lookupTbl.BF.mac_addr_32_47 == mac_32_47) &&
		(lookupTbl.BF.gid == pcfg->fid)
	) {
		ret = JL_ERR_EXISTS;
	} else
		ret = JL_ERR_OK;

	return ret;
}

static jl_ret_t _jl61xx_l2_hash_col_check(
	jl_device_t *pdevice,
	jl_l2_tbl_t *pcfg,
	jl_uint32 idx)
{
	jl_ret_t ret = JL_ERR_OK;
	jl_uint32 mac_31 = 0;
	jl_uint16 mac_32_47 = 0;

	SWCORE_L2_LOOKUP_COLLISION_TABLE_t colTbl;

	REG_BURST_READ(pdevice, SWCORE, L2_LOOKUP_COLLISION_TABLE, idx, INDEX_ZERO, colTbl.val);

	mac_31 = pcfg->mac.addr[5] + BITS_OFFSET_L(pcfg->mac.addr[4], 8, 8) +
	    BITS_OFFSET_L(pcfg->mac.addr[3], 16, 8) + BITS_OFFSET_L(pcfg->mac.addr[2], 24, 8);
	mac_32_47 = BITS_OFFSET_L(pcfg->mac.addr[1], 0, 8) + BITS_OFFSET_L(pcfg->mac.addr[0], 8, 8);

	if ((colTbl.BF.gid == pcfg->fid) &&
		(colTbl.BF.mac_addr_0_31 == mac_31) &&
		(colTbl.BF.mac_addr_32_47 == mac_32_47)
	) {
		ret = JL_ERR_EXISTS;
	} else
		ret = JL_ERR_OK;

	return ret;
}

static jl_ret_t _jl61xx_l2_tbl_update(
	jl_device_t *pdevice,
	jl_l2_tbl_t *pcfg,
	jl_uint32 hash_val,
	jl_uint32 idx)
{
	jl_ret_t ret = JL_ERR_OK;
	jl_uint8 i;
	jl_uint32 mac_port_val;
	SWCORE_L2_DA_HASH_LOOKUP_TABLE_t lookupTbl;
	SWCORE_L2_AGING_TABLE_t ageTbl;
	SWCORE_L2_DESTINATION_TABLE_t dstTbl;
	SWCORE_L2_MULTICAST_TABLE_t mulTbl;
	SWCORE_L2_AGING_STATUS_SHADOW_TABLE_t shadowAgeTbl;

	memset((void *)&lookupTbl, 0, sizeof(SWCORE_L2_DA_HASH_LOOKUP_TABLE_t));
	memset((void *)&ageTbl, 0, sizeof(SWCORE_L2_AGING_TABLE_t));
	memset((void *)&dstTbl, 0, sizeof(SWCORE_L2_DESTINATION_TABLE_t));
	memset((void *)&shadowAgeTbl, 0, sizeof(SWCORE_L2_AGING_STATUS_SHADOW_TABLE_t));
	memset((void *)&mulTbl, 0, sizeof(SWCORE_L2_MULTICAST_TABLE_t));

	/*over dynamic */
	ageTbl.BF.hit = pcfg->valid;
	ageTbl.BF.stat = pcfg->stat;
	ageTbl.BF.valid = pcfg->valid;
	lookupTbl.BF.gid = pcfg->fid;
	lookupTbl.BF.mac_addr_0_31 =
	    pcfg->mac.addr[5] + BITS_OFFSET_L(pcfg->mac.addr[4], 8, 8) +
	    BITS_OFFSET_L(pcfg->mac.addr[3], 16, 8) + BITS_OFFSET_L(pcfg->mac.addr[2], 24, 8);

	lookupTbl.BF.mac_addr_32_47 = BITS_OFFSET_L(pcfg->mac.addr[1], 0, 8) + BITS_OFFSET_L(pcfg->mac.addr[0], 8, 8);
	dstTbl.BF.uc = pcfg->type;
	if (dstTbl.BF.uc) {
		if (unlikely(JL_L2_MAC_DROPPORT_UC == pcfg->sportval)) {
			dstTbl.BF.pkt_drop = 1;
			dstTbl.BF.dest_port_or_mc_addr = 0;
		} else {
			JL_PORT_L2C_MACRO(pdevice, pcfg->sportval, mac_port_val);
			dstTbl.BF.dest_port_or_mc_addr = mac_port_val;
			dstTbl.BF.pkt_drop = 0;
		}
	} else {
		if (unlikely(JL_L2_MAC_DROPPORT_MC == pcfg->sportval)) {
			dstTbl.BF.pkt_drop = 1;
			dstTbl.BF.dest_port_or_mc_addr = 0;
		} else {
			JL_PORT_LMAP_TO_CMAP(pdevice, pcfg->sportval, mac_port_val);
			for (i = 0; i < SWCORE_L2_MULTICAST_TABLE_NUM; i++) {
				REG_BURST_READ(pdevice, SWCORE, L2_MULTICAST_TABLE, i, INDEX_ZERO, mulTbl.val);
				if (L2_MULTICAST_PORTMSK_DEFAULT == mulTbl.BF.mc_port_mask
				     || mulTbl.BF.mc_port_mask == mac_port_val) {
					mulTbl.BF.mc_port_mask = mac_port_val;
					dstTbl.BF.dest_port_or_mc_addr = i;
					break;
				}
			}
			if (i == SWCORE_L2_MULTICAST_TABLE_NUM) {
				JL_DBG_MSG(JL_FLAG_SWCORE, _DBG_WARNING,
					    "Table for multicast portmsk entry is full, plz clr first\n");
				return JL_ERR_FULL;
			}

			REG_BURST_WRITE(pdevice, SWCORE, L2_MULTICAST_TABLE, i, INDEX_ZERO, mulTbl.val);
			dstTbl.BF.pkt_drop = 0;
		}
	}

	/*refill to shadow tbl */
	shadowAgeTbl.BF.valid = pcfg->valid;
	REG_BURST_WRITE(pdevice, SWCORE, L2_AGING_STATUS_SHADOW_TABLE, hash_val + (idx << L2_HASH_WIDTH), INDEX_ZERO,
			 shadowAgeTbl.val);
	JL_DBG_MSG(JL_FLAG_SWCORE, _DBG_INFO, "shadow hash %x: %08x\n", hash_val + (idx << L2_HASH_WIDTH),
		    shadowAgeTbl.val[0]);

	/*send to driver */
	REG_BURST_WRITE(pdevice, SWCORE, L2_AGING_TABLE, hash_val + (idx << L2_HASH_WIDTH), INDEX_ZERO, ageTbl.val);
	REG_BURST_WRITE(pdevice, SWCORE, L2_DA_HASH_LOOKUP_TABLE, hash_val + (idx << L2_HASH_WIDTH), INDEX_ZERO,
			 lookupTbl.val);
	REG_BURST_WRITE(pdevice, SWCORE, L2_DESTINATION_TABLE, hash_val + (idx << L2_HASH_WIDTH), INDEX_ZERO,
			    dstTbl.val);

	JL_DBG_MSG(JL_FLAG_SWCORE, _DBG_INFO, "Update l2 tbl success, hash idx is %x\n",
			hash_val + (idx << L2_HASH_WIDTH));
	JL_DBG_MSG(JL_FLAG_SWCORE, _DBG_INFO,
		    "Tbl value is: valid: %d, stat: %d, fid: %d, type: %d sport: %d mac addr is %x:%x:%x:%x:%x:%x\n",
		    pcfg->valid, pcfg->stat, pcfg->fid, pcfg->type, pcfg->sportval, pcfg->mac.addr[0],
		    pcfg->mac.addr[1], pcfg->mac.addr[2], pcfg->mac.addr[3], pcfg->mac.addr[4], pcfg->mac.addr[5]);
	JL_DBG_MSG(JL_FLAG_SWCORE, _DBG_INFO, "Reg val is: lookup: %08x-%08x, age: %08x , dst: %08x\n", lookupTbl.val[0],
		    lookupTbl.val[1], ageTbl.val[0], dstTbl.val[0]);
	if (!dstTbl.BF.uc)
		JL_DBG_MSG(JL_FLAG_SWCORE, _DBG_INFO, "mul: %08x\n", mulTbl.val[0]);

	return ret;
}

static jl_ret_t _jl61xx_l2_col_tbl_update(jl_device_t *pdevice, jl_l2_tbl_t *pcfg, jl_uint32 idx)
{
	jl_ret_t ret = JL_ERR_OK;
	jl_uint8 i;
	jl_uint32 mac_port_val;

	SWCORE_L2_DESTINATION_TABLE_t dstTbl;
	SWCORE_L2_LOOKUP_COLLISION_TABLE_t colTbl;
	SWCORE_L2_AGING_COLLISION_TABLE_t colAgeTbl;
	SWCORE_L2_MULTICAST_TABLE_t mulTbl;

	SWCORE_L2_AGING_COLLISION_SHADOW_TABLE_t colShadowTbl;

	memset((void *)&colAgeTbl, 0, sizeof(SWCORE_L2_AGING_COLLISION_SHADOW_TABLE_t));
	memset((void *)&colTbl, 0, sizeof(SWCORE_L2_LOOKUP_COLLISION_TABLE_t));
	memset((void *)&dstTbl, 0, sizeof(SWCORE_L2_DESTINATION_TABLE_t));
	memset((void *)&mulTbl, 0, sizeof(SWCORE_L2_MULTICAST_TABLE_t));

	memset((void *)&colShadowTbl, 0, sizeof(SWCORE_L2_AGING_COLLISION_SHADOW_TABLE_t));

	colAgeTbl.BF.stat = pcfg->stat;
	colAgeTbl.BF.hit = pcfg->valid;
	colAgeTbl.BF.valid = pcfg->valid;

	colTbl.BF.gid = pcfg->fid;
	colTbl.BF.mac_addr_0_31 =
	    pcfg->mac.addr[5] + BITS_OFFSET_L(pcfg->mac.addr[4], 8, 8)
	    + BITS_OFFSET_L(pcfg->mac.addr[3], 16, 8) + BITS_OFFSET_L(pcfg->mac.addr[2], 24, 8);
	colTbl.BF.mac_addr_32_47 = BITS_OFFSET_L(pcfg->mac.addr[1], 0, 8) + BITS_OFFSET_L(pcfg->mac.addr[0], 8, 8);

	dstTbl.BF.uc = pcfg->type;

	if (dstTbl.BF.uc) {
		if (unlikely(JL_L2_MAC_DROPPORT_UC == pcfg->sportval)) {
			dstTbl.BF.pkt_drop = 1;
			dstTbl.BF.dest_port_or_mc_addr = 0;
		} else {
			JL_PORT_L2C_MACRO(pdevice, pcfg->sportval, mac_port_val);
			dstTbl.BF.dest_port_or_mc_addr = mac_port_val;
			dstTbl.BF.pkt_drop = 0;
		}
	} else {
		if (unlikely(JL_L2_MAC_DROPPORT_MC == pcfg->sportval)) {
			dstTbl.BF.pkt_drop = 1;
			dstTbl.BF.dest_port_or_mc_addr = 0;
		} else {
			JL_PORT_LMAP_TO_CMAP(pdevice, pcfg->sportval, mac_port_val);
			for (i = 0; i < SWCORE_L2_MULTICAST_TABLE_NUM; i++) {
				REG_BURST_READ(pdevice, SWCORE, L2_MULTICAST_TABLE, i, INDEX_ZERO, mulTbl.val);
				if (L2_MULTICAST_PORTMSK_DEFAULT == mulTbl.BF.mc_port_mask
				     || mulTbl.BF.mc_port_mask == mac_port_val) {
					mulTbl.BF.mc_port_mask = mac_port_val;
					dstTbl.BF.dest_port_or_mc_addr = i;
					break;
				}
			}

			if (i == SWCORE_L2_MULTICAST_TABLE_NUM) {
				JL_DBG_MSG(JL_FLAG_SWCORE, _DBG_WARNING,
					    "Table for multicast portmsk entry is full, plz clr first\n");
				return JL_ERR_FULL;
			}

			REG_BURST_WRITE(pdevice, SWCORE, L2_MULTICAST_TABLE, i, INDEX_ZERO, mulTbl.val);
			dstTbl.BF.pkt_drop = 0;
		}
	}


	/*send to driver */
	REG_BURST_WRITE(pdevice, SWCORE, L2_AGING_COLLISION_TABLE, idx, INDEX_ZERO, colAgeTbl.val);
	REG_BURST_WRITE(pdevice, SWCORE, L2_LOOKUP_COLLISION_TABLE, idx, INDEX_ZERO, colTbl.val);
	REG_BURST_WRITE(pdevice, SWCORE, L2_DESTINATION_TABLE, idx + L2_TBL_MAX_DEPTH, INDEX_ZERO, dstTbl.val);

	/*refill to shadow tbl */
	colShadowTbl.BF.hit = pcfg->valid;
	colShadowTbl.BF.stat = pcfg->stat;
	colShadowTbl.BF.valid = pcfg->valid;
	REG_BURST_WRITE(pdevice, SWCORE, L2_AGING_COLLISION_SHADOW_TABLE, idx, INDEX_ZERO, colShadowTbl.val);

	JL_DBG_MSG(JL_FLAG_SWCORE, _DBG_INFO, "Update l2 col tbl success, hash idx is %x\n", idx);
	JL_DBG_MSG(JL_FLAG_SWCORE, _DBG_INFO,
		    "Tbl value is: valid: %d, stat: %d, fid: %d, type: %d sport: %d mac addr is %x:%x:%x:%x:%x:%x\n",
		    pcfg->valid, pcfg->stat, pcfg->fid, pcfg->type, pcfg->sportval, pcfg->mac.addr[0],
		    pcfg->mac.addr[1], pcfg->mac.addr[2], pcfg->mac.addr[3], pcfg->mac.addr[4], pcfg->mac.addr[5]);
	JL_DBG_MSG(JL_FLAG_SWCORE, _DBG_INFO, "Reg val is: colTbl: %08x-%08x, colAgeTbl: %08x , dst: %08x\n",
		    colTbl.val[0], colTbl.val[1], colAgeTbl.val[0], dstTbl.val[0]);
	if (!dstTbl.BF.uc)
		JL_DBG_MSG(JL_FLAG_SWCORE, _DBG_INFO, "mul: %08x", mulTbl.val[0]);

	return ret;
}

static jl_ret_t _jl61xx_l2_col_tbl_get(
	jl_device_t *pdevice,
	jl_l2_tbl_t *pcfg,
	jl_uint32 idx)
{
	jl_ret_t ret = JL_ERR_OK;

	SWCORE_L2_DESTINATION_TABLE_t dstTbl;
	SWCORE_L2_LOOKUP_COLLISION_TABLE_MASKS_t colTbl;
	SWCORE_L2_AGING_COLLISION_TABLE_t colAgeTbl;
	SWCORE_L2_MULTICAST_TABLE_t mulTbl;

	/*read from driver */
	REG_BURST_READ(pdevice, SWCORE, L2_LOOKUP_COLLISION_TABLE, idx, INDEX_ZERO, colTbl.val);
	REG_BURST_READ(pdevice, SWCORE, L2_AGING_COLLISION_SHADOW_TABLE, idx, INDEX_ZERO, colAgeTbl.val);

	pcfg->fid = colTbl.BF.gid;
	pcfg->mac.addr[5] = BITS_OFFSET_R(colTbl.BF.mac_addr_0_31, 0, 8);
	pcfg->mac.addr[4] = BITS_OFFSET_R(colTbl.BF.mac_addr_0_31, 8, 8);
	pcfg->mac.addr[3] = BITS_OFFSET_R(colTbl.BF.mac_addr_0_31, 16, 8);
	pcfg->mac.addr[2] = BITS_OFFSET_R(colTbl.BF.mac_addr_0_31, 24, 8);
	pcfg->mac.addr[1] = BITS_OFFSET_R(colTbl.BF.mac_addr_32_47, 0, 8);
	pcfg->mac.addr[0] = BITS_OFFSET_R(colTbl.BF.mac_addr_32_47, 8, 8);

	pcfg->valid = colAgeTbl.BF.hit;
	pcfg->stat = colAgeTbl.BF.stat;

	REG_BURST_READ(pdevice, SWCORE, L2_DESTINATION_TABLE, idx + L2_TBL_MAX_DEPTH, INDEX_ZERO, dstTbl.val);
	pcfg->type = dstTbl.BF.uc;
	if (dstTbl.BF.uc) {
		JL_PORT_C2L_MACRO(pdevice, dstTbl.BF.dest_port_or_mc_addr, pcfg->sportval);
	} else {
		REG_BURST_READ(pdevice, SWCORE, L2_MULTICAST_TABLE, dstTbl.BF.dest_port_or_mc_addr, INDEX_ZERO,
				mulTbl.val);
		JL_PORT_CMAP_TO_LMAP(pdevice, mulTbl.BF.mc_port_mask, pcfg->sportval);
	}

	JL_DBG_MSG(JL_FLAG_SWCORE, _DBG_INFO, "Get l2 col tbl success, hash idx is %xs\n", idx);
	JL_DBG_MSG(JL_FLAG_SWCORE, _DBG_INFO,
		    "Tbl value is: valid: %d, stat: %d, fid: %d, type: %d sport: %d mac addr is %x:%x:%x:%x:%x:%x\n",
		    pcfg->valid, pcfg->stat, pcfg->fid, pcfg->type, pcfg->sportval, pcfg->mac.addr[0],
		    pcfg->mac.addr[1], pcfg->mac.addr[2], pcfg->mac.addr[3], pcfg->mac.addr[4], pcfg->mac.addr[5]);
	JL_DBG_MSG(JL_FLAG_SWCORE, _DBG_INFO, "Reg val is: colTbl: %08x-%08x, colAgeTbl: %08x , dst: %08x\n",
		    colTbl.val[0], colTbl.val[1], colAgeTbl.val[0], dstTbl.val[0]);

	return ret;
}

static jl_ret_t _jl61xx_l2_aging_time_set(
	jl_device_t *pdevice,
	const jl_uint32 timeval,
	const jl_uint32 timestep)
{
	jl_ret_t ret = JL_ERR_OK;
	SWCORE_LEARNING_AND_AGING_ENABLE_t flag;
	SWCORE_TIME_TO_AGE_t age;

	/*disable & enable age fun outside*/
	REG_BURST_READ(pdevice, SWCORE, LEARNING_AND_AGING_ENABLE, INDEX_ZERO, INDEX_ZERO, flag.val);
	flag.BF.aging_enable = 0;
	REG_BURST_WRITE(pdevice, SWCORE, LEARNING_AND_AGING_ENABLE, INDEX_ZERO, INDEX_ZERO, flag.val);

	age.BF.tick_cnt = timeval;
	age.BF.tick = timestep;

	/*send to driver */
	REG_BURST_WRITE(pdevice, SWCORE, TIME_TO_AGE, 0, INDEX_ZERO, age.val);

	/*reset glb func*/
	flag.BF.aging_enable = 1;
	REG_BURST_WRITE(pdevice, SWCORE, LEARNING_AND_AGING_ENABLE, INDEX_ZERO, INDEX_ZERO, flag.val);

	JL_DBG_MSG(JL_FLAG_SWCORE, _DBG_INFO, "Set reg info %x-%x success, tick_cnt %d, tick %d\n", age.val[0],
		      age.val[1], age.BF.tick_cnt, age.BF.tick);
	return ret;
}

static jl_ret_t _jl61xx_l2_aging_time_get(
	 jl_device_t *pdevice,
	 jl_uint32 *ptimeval,
	 jl_uint32 *ptimestep)
{
	jl_ret_t ret = JL_ERR_OK;
	SWCORE_TIME_TO_AGE_t age;

	/*read from driver */
	REG_BURST_READ(pdevice, SWCORE, TIME_TO_AGE, 0, INDEX_ZERO, age.val);

	*ptimeval = age.BF.tick_cnt;
	*ptimestep = age.BF.tick;

	JL_DBG_MSG(JL_FLAG_SWCORE, _DBG_INFO, "Get reg tick_cnt %x, tick %x success\n", age.BF.tick_cnt, age.BF.tick);
	return ret;
}

jl_ret_t _jl61xx_l2_hash_masktable_set(
	jl_device_t *pdevice,
	const jl_uint32 idx,
	jl_mac_addr_t *pmacmask,
	const jl_uint32 fidmask)
{
	jl_ret_t ret = JL_ERR_OK;

	SWCORE_L2_LOOKUP_COLLISION_TABLE_MASKS_t coltblmask;

	coltblmask.BF.gid = fidmask;
	coltblmask.BF.mac_addr_0_31 =
	    pmacmask->addr[5] + BITS_OFFSET_L(pmacmask->addr[4], 8, 8) +
	    BITS_OFFSET_L(pmacmask->addr[3], 16, 8) + BITS_OFFSET_L(pmacmask->addr[2], 24, 8);

	coltblmask.BF.mac_addr_32_47 = pmacmask->addr[1] + BITS_OFFSET_L(pmacmask->addr[0], 8, 8);

	/*send to driver */
	REG_BURST_WRITE(pdevice, SWCORE, L2_LOOKUP_COLLISION_TABLE_MASKS, idx, INDEX_ZERO, coltblmask.val);
	JL_DBG_MSG(JL_FLAG_SWCORE, _DBG_INFO,
		      "Set hash col table idx %d success,  gid is %d, mac is %x:%x:%x:%x:%x:%x, reg val is %x-%x", idx,
		      fidmask, pmacmask->addr[0], pmacmask->addr[1], pmacmask->addr[2], pmacmask->addr[3], pmacmask->addr[4],
		      pmacmask->addr[5], coltblmask.val[0], coltblmask.val[1]);

	return ret;
}

jl_ret_t _jl61xx_l2_hash_masktable_get(
	jl_device_t *pdevice,
	const jl_uint32 idx,
	jl_mac_addr_t *pmacmask,
	jl_uint32 *pfidmask)
{
	jl_ret_t ret = JL_ERR_OK;

	SWCORE_L2_LOOKUP_COLLISION_TABLE_MASKS_t coltblmask;

	/*read from driver */
	REG_BURST_READ(pdevice, SWCORE, L2_LOOKUP_COLLISION_TABLE_MASKS, idx, INDEX_ZERO, coltblmask.val);

	*pfidmask = coltblmask.BF.gid;
	pmacmask->addr[5] = BITS_OFFSET_R(coltblmask.BF.mac_addr_0_31, 0, 8);
	pmacmask->addr[4] = BITS_OFFSET_R(coltblmask.BF.mac_addr_0_31, 8, 8);
	pmacmask->addr[3] = BITS_OFFSET_R(coltblmask.BF.mac_addr_0_31, 16, 8);
	pmacmask->addr[2] = BITS_OFFSET_R(coltblmask.BF.mac_addr_0_31, 24, 8);
	pmacmask->addr[1] = BITS_OFFSET_R(coltblmask.BF.mac_addr_32_47, 0, 8);
	pmacmask->addr[0] = BITS_OFFSET_R(coltblmask.BF.mac_addr_32_47, 8, 8);

	JL_DBG_MSG(JL_FLAG_SWCORE, _DBG_INFO,
			"Get hash col table idx %d success,  gid is %d, mac is %x:%x:%x:%x:%x:%x, reg val is %x-%x",
			idx, *pfidmask, pmacmask->addr[0], pmacmask->addr[1], pmacmask->addr[2], pmacmask->addr[3],
			pmacmask->addr[4], pmacmask->addr[5], coltblmask.val[0], coltblmask.val[1]);

	return ret;
}


/* For firmware , these case will be set below
 *   Learning And Aging Enable
 *   Source port learning/dst Port
 *   Hairpin Enable
 *   Default Learning Status
 */

jl_ret_t jl61xx_l2_init(jl_device_t *pdevice)
{
	jl_ret_t ret = JL_ERR_OK;

	if (NULL == pdevice)
		return JL_ERR_PARAM;
	/*Add init setting as follows*/

	return ret;
}

jl_ret_t jl61xx_l2_deinit(jl_device_t *pdevice)
{
	jl_ret_t ret = JL_ERR_OK;

	if (NULL == pdevice)
		return JL_ERR_PARAM;
	/*Add deinit setting as follows*/

	return ret;
}

jl_ret_t jl61xx_l2_table_set(jl_device_t *pdevice, jl_l2_tbl_t *pcfg)
{
	jl_ret_t ret = JL_ERR_OK;
	jl_uint32 check_hash;
	jl_uint32 i;

	SWCORE_L2_AGING_TABLE_t ageTbl;
	SWCORE_L2_AGING_COLLISION_TABLE_t colAgeTbl;

	/*para check */
	JL_CHECK_ENUM(pcfg->fid, (L2_FID_MAX_VALUE + 1));
	if (JL_L2_TBL_TYPE_UC == pcfg->type) {
		if (likely(JL_L2_MAC_DROPPORT_UC != pcfg->sportval))
			JL_CHECK_PORT(pdevice, pcfg->sportval);
	} else if (JL_L2_TBL_TYPE_MC == pcfg->type)
		JL_CHECK_PORTMASK(pdevice, pcfg->sportval);

	memset((void *)&ageTbl, 0, sizeof(SWCORE_L2_AGING_TABLE_t));
	memset((void *)&colAgeTbl, 0, sizeof(SWCORE_L2_AGING_COLLISION_TABLE_t));

	/*if set static, check dynamic flag to confirm rewrite */
	check_hash = __l2_hash(&(pcfg->mac), pcfg->fid);

	if (pcfg->stat) {
		/*read from driver */
		for (i = 0; i < L2_HASH_WAY_VALUE; i++) {
			REG_BURST_READ(pdevice, SWCORE, L2_AGING_TABLE, check_hash + (i << L2_HASH_WIDTH), INDEX_ZERO,
			ageTbl.val);
			ret = _jl61xx_l2_hash_check(pdevice, pcfg, check_hash, i);
			if ((JL_ERR_EXISTS == ret) && ageTbl.BF.valid) {
				ret = _jl61xx_l2_tbl_update(pdevice, pcfg, check_hash, i);
				return ret;
			}
		}
		for (i = 0; i < L2_HASH_WAY_VALUE; i++) {
			REG_BURST_READ(pdevice, SWCORE, L2_AGING_TABLE, check_hash + (i << L2_HASH_WIDTH), INDEX_ZERO,
			ageTbl.val);
			/*no same entry*/
			if ((!ageTbl.BF.valid) || (!ageTbl.BF.stat)) {
				ret = _jl61xx_l2_tbl_update(pdevice, pcfg, check_hash, i);
				return ret;
			}
		}
		/*all static, so use hash collision */
		for (i = 0; i < L2_COL_TBL_MAX_DEPTH; i++) {
			/*read from driver */
			REG_BURST_READ(pdevice, SWCORE, L2_AGING_COLLISION_TABLE, i, INDEX_ZERO, colAgeTbl.val);
			if (!colAgeTbl.BF.valid) {
				ret = _jl61xx_l2_tbl_update(pdevice, pcfg, check_hash, i);
				return ret;
			} else {
				ret = _jl61xx_l2_hash_col_check(pdevice, pcfg, i);
				if ((!colAgeTbl.BF.stat) || ret) {
					ret = _jl61xx_l2_col_tbl_update(pdevice, pcfg, i);
					return ret;
				}
			}
		}
		if (L2_COL_TBL_MAX_DEPTH == i) {
			JL_DBG_MSG(JL_FLAG_SWCORE, _DBG_WARNING, "Table for static entry is full, plz clr first\n");
			return JL_ERR_FULL;
		}
	} else {/*for dynamic */
		for (i = 0; i < L2_HASH_WAY_VALUE; i++) {
			/*read from driver */
			REG_BURST_READ(pdevice, SWCORE, L2_AGING_TABLE, check_hash + (i << L2_HASH_WIDTH), INDEX_ZERO,
					ageTbl.val);
			ret = _jl61xx_l2_hash_check(pdevice, pcfg, check_hash, i);
			if ((JL_ERR_EXISTS == ret) && ageTbl.BF.valid) {
				ret = _jl61xx_l2_tbl_update(pdevice, pcfg, check_hash, i);
				return ret;
			}
		}
		for (i = 0; i < L2_HASH_WAY_VALUE; i++) {
			/*no same entry*/
			/*read from driver */
			REG_BURST_READ(pdevice, SWCORE, L2_AGING_TABLE, check_hash + (i << L2_HASH_WIDTH), INDEX_ZERO,
					ageTbl.val);
			if (!ageTbl.BF.valid) {
				ret = _jl61xx_l2_tbl_update(pdevice, pcfg, check_hash, i);
				return ret;
			}
		}
		/* hash collision , use collision tbl */
		for (i = 0; i < L2_COL_TBL_MAX_DEPTH; i++) {
			/*read from driver */
			REG_BURST_READ(pdevice, SWCORE, L2_AGING_COLLISION_TABLE, i, INDEX_ZERO, colAgeTbl.val);
			ret = _jl61xx_l2_hash_col_check(pdevice, pcfg, i);
			if ((!colAgeTbl.BF.valid) || ret) {
				ret = _jl61xx_l2_col_tbl_update(pdevice, pcfg, i);
				return ret;
			}
		}
		if (L2_COL_TBL_MAX_DEPTH == i) {
			JL_DBG_MSG(JL_FLAG_SWCORE, _DBG_WARNING, "Table for dynamic entry is full, plz clr first\n");
			return JL_ERR_FULL;
		}
	}
	return ret;
}

jl_ret_t jl61xx_l2_table_search(jl_device_t *pdevice, const jl_l2_sel_mode_t mode, jl_l2_tbl_t *pcfg)
{
	jl_ret_t ret = JL_ERR_OK;
	jl_uint32 check_hash;
	jl_uint32 lportval;
	jl_uint32 i, j;

	SWCORE_L2_AGING_TABLE_t ageTbl;
	SWCORE_L2_DA_HASH_LOOKUP_TABLE_t lookupTbl;
	SWCORE_L2_DESTINATION_TABLE_t dstTbl;

	SWCORE_L2_MULTICAST_TABLE_t mulTbl;

	/*para check */
	if (JL_L2_SEL_MODE_MULPORTMSK == mode)
		JL_CHECK_PORTMASK(pdevice, pcfg->sportval);
	else if (JL_L2_SEL_MODE_SPORT == mode) {
		if (likely(JL_L2_MAC_DROPPORT_UC != pcfg->sportval))
			JL_CHECK_PORT(pdevice, pcfg->sportval);
	}

	switch (mode) {
	case JL_L2_SEL_MODE_STATIC:
		{
			for (i = g_l2_tbl_search_idx[JL_L2_SEL_MODE_STATIC] + 1; i < SWCORE_L2_AGING_TABLE_NUM; i++) {
				/*read from driver */
				REG_BURST_READ(pdevice, SWCORE, L2_AGING_TABLE, i, INDEX_ZERO, ageTbl.val);
				if (unlikely(ageTbl.BF.stat && ageTbl.BF.valid)) {
					g_l2_tbl_search_idx[JL_L2_SEL_MODE_STATIC] = i;

				    /*read lookup tbl */
				    REG_BURST_READ(pdevice, SWCORE, L2_DA_HASH_LOOKUP_TABLE, i, INDEX_ZERO,
						   lookupTbl.val);

					pcfg->fid = lookupTbl.BF.gid;
					pcfg->mac.addr[5] = BITS_OFFSET_R(lookupTbl.BF.mac_addr_0_31, 0, 8);
					pcfg->mac.addr[4] = BITS_OFFSET_R(lookupTbl.BF.mac_addr_0_31, 8, 8);
					pcfg->mac.addr[3] = BITS_OFFSET_R(lookupTbl.BF.mac_addr_0_31, 16, 8);
					pcfg->mac.addr[2] = BITS_OFFSET_R(lookupTbl.BF.mac_addr_0_31, 24, 8);
					pcfg->mac.addr[1] = BITS_OFFSET_R(lookupTbl.BF.mac_addr_32_47, 0, 8);
					pcfg->mac.addr[0] = BITS_OFFSET_R(lookupTbl.BF.mac_addr_32_47, 8, 8);

				    /*read dst tbl */
				    REG_BURST_READ(pdevice, SWCORE, L2_DESTINATION_TABLE, i, INDEX_ZERO,
						   dstTbl.val);
					pcfg->stat = ageTbl.BF.stat;
					pcfg->valid = ageTbl.BF.valid;
					pcfg->type = dstTbl.BF.uc;
					if (dstTbl.BF.uc) {
						JL_PORT_C2L_MACRO(pdevice, dstTbl.BF.dest_port_or_mc_addr, pcfg->sportval);
					} else {
						REG_BURST_READ(pdevice, SWCORE, L2_MULTICAST_TABLE,
								dstTbl.BF.dest_port_or_mc_addr, INDEX_ZERO, mulTbl.val);
						JL_PORT_CMAP_TO_LMAP(pdevice, mulTbl.BF.mc_port_mask, pcfg->sportval);
					}
					ret = JL_ERR_EXISTS;
					JL_DBG_MSG(JL_FLAG_SWCORE, _DBG_INFO,
						    "Get mode %d hash idx %d entry info mode success\n",
						    JL_L2_SEL_MODE_STATIC, i);
					break;
				}
			}
			if (SWCORE_L2_AGING_TABLE_NUM == i) {
				g_l2_tbl_search_idx[JL_L2_SEL_MODE_STATIC] = -1;
				ret = JL_ERR_OK;
			}
			break;
		}
	case JL_L2_SEL_MODE_SPORT:
		{
			for (i = g_l2_tbl_search_idx[JL_L2_SEL_MODE_SPORT] + 1; i < SWCORE_L2_AGING_TABLE_NUM; i++) {
				/*read form driver */
				REG_BURST_READ(pdevice, SWCORE, L2_DESTINATION_TABLE, i, INDEX_ZERO, dstTbl.val);
				REG_BURST_READ(pdevice, SWCORE, L2_AGING_TABLE, i, INDEX_ZERO, ageTbl.val);
				if (unlikely(JL_L2_MAC_DROPPORT_UC == pcfg->sportval)) {
					if (dstTbl.BF.pkt_drop && ageTbl.BF.valid && dstTbl.BF.uc) {
						g_l2_tbl_search_idx[JL_L2_SEL_MODE_SPORT] = i;
						REG_BURST_READ(pdevice, SWCORE, L2_DA_HASH_LOOKUP_TABLE, i, INDEX_ZERO,
								lookupTbl.val);
						pcfg->fid = lookupTbl.BF.gid;
						pcfg->mac.addr[5] = BITS_OFFSET_R(lookupTbl.BF.mac_addr_0_31, 0, 8);
						pcfg->mac.addr[4] = BITS_OFFSET_R(lookupTbl.BF.mac_addr_0_31, 8, 8);
						pcfg->mac.addr[3] = BITS_OFFSET_R(lookupTbl.BF.mac_addr_0_31, 16, 8);
						pcfg->mac.addr[2] = BITS_OFFSET_R(lookupTbl.BF.mac_addr_0_31, 24, 8);
						pcfg->mac.addr[1] = BITS_OFFSET_R(lookupTbl.BF.mac_addr_32_47, 0, 8);
						pcfg->mac.addr[0] = BITS_OFFSET_R(lookupTbl.BF.mac_addr_32_47, 8, 8);

						pcfg->type = dstTbl.BF.uc;
						pcfg->stat = ageTbl.BF.stat;
						pcfg->valid = ageTbl.BF.valid;

						ret = JL_ERR_EXISTS;
						JL_DBG_MSG(JL_FLAG_SWCORE, _DBG_INFO,
							    "Get mode %d hash idx %d entry info mode success\n",
							    JL_L2_SEL_MODE_SPORT, i);
						break;
					}
				} else {
					g_l2_tbl_search_idx[JL_L2_SEL_MODE_SPORT] = i;
					JL_PORT_C2L_MACRO(pdevice, dstTbl.BF.dest_port_or_mc_addr, lportval);
					if (pcfg->sportval == lportval && ageTbl.BF.valid && dstTbl.BF.uc) {
						REG_BURST_READ(pdevice, SWCORE, L2_DA_HASH_LOOKUP_TABLE, i, INDEX_ZERO,
								lookupTbl.val);
						pcfg->fid = lookupTbl.BF.gid;
						pcfg->mac.addr[5] = BITS_OFFSET_R(lookupTbl.BF.mac_addr_0_31, 0, 8);
						pcfg->mac.addr[4] = BITS_OFFSET_R(lookupTbl.BF.mac_addr_0_31, 8, 8);
						pcfg->mac.addr[3] = BITS_OFFSET_R(lookupTbl.BF.mac_addr_0_31, 16, 8);
						pcfg->mac.addr[2] = BITS_OFFSET_R(lookupTbl.BF.mac_addr_0_31, 24, 8);
						pcfg->mac.addr[1] = BITS_OFFSET_R(lookupTbl.BF.mac_addr_32_47, 0, 8);
						pcfg->mac.addr[0] = BITS_OFFSET_R(lookupTbl.BF.mac_addr_32_47, 8, 8);

						pcfg->type = dstTbl.BF.uc;
						pcfg->stat = ageTbl.BF.stat;
						pcfg->valid = ageTbl.BF.valid;

						ret = JL_ERR_EXISTS;
						JL_DBG_MSG(JL_FLAG_SWCORE, _DBG_INFO,
							    "Get mode %d hash idx %d entry info mode success\n",
							    JL_L2_SEL_MODE_SPORT, i);
						break;
					}
				}
			}
			if (SWCORE_L2_AGING_TABLE_NUM == i) {
				g_l2_tbl_search_idx[JL_L2_SEL_MODE_SPORT] = -1;
				ret = JL_ERR_OK;
			}
			break;
		}
	case JL_L2_SEL_MODE_FID:
		{
			JL_CHECK_ENUM(pcfg->fid, (L2_FID_MAX_VALUE + 1));
			for (i = g_l2_tbl_search_idx[JL_L2_SEL_MODE_FID] + 1; i < SWCORE_L2_AGING_TABLE_NUM; i++) {
				/*read form driver */
				REG_BURST_READ(pdevice, SWCORE, L2_DA_HASH_LOOKUP_TABLE, i, INDEX_ZERO, lookupTbl.val);
				REG_BURST_READ(pdevice, SWCORE, L2_AGING_TABLE, i, INDEX_ZERO, ageTbl.val);

				if (pcfg->fid == lookupTbl.BF.gid && ageTbl.BF.valid) {
					g_l2_tbl_search_idx[JL_L2_SEL_MODE_FID] = i;
					pcfg->mac.addr[5] = BITS_OFFSET_R(lookupTbl.BF.mac_addr_0_31, 0, 8);
					pcfg->mac.addr[4] = BITS_OFFSET_R(lookupTbl.BF.mac_addr_0_31, 8, 8);
					pcfg->mac.addr[3] = BITS_OFFSET_R(lookupTbl.BF.mac_addr_0_31, 16, 8);
					pcfg->mac.addr[2] = BITS_OFFSET_R(lookupTbl.BF.mac_addr_0_31, 24, 8);
					pcfg->mac.addr[1] = BITS_OFFSET_R(lookupTbl.BF.mac_addr_32_47, 0, 8);
					pcfg->mac.addr[0] = BITS_OFFSET_R(lookupTbl.BF.mac_addr_32_47, 8, 8);
					pcfg->fid = lookupTbl.BF.gid;

					pcfg->valid = ageTbl.BF.valid;
					pcfg->stat = ageTbl.BF.stat;

					REG_BURST_READ(pdevice, SWCORE, L2_DESTINATION_TABLE, i, INDEX_ZERO, dstTbl.val);
					pcfg->type = dstTbl.BF.uc;

					if (dstTbl.BF.uc) {
						JL_PORT_C2L_MACRO(pdevice, dstTbl.BF.dest_port_or_mc_addr, lportval);
						pcfg->sportval = lportval;
					} else {
						REG_BURST_READ(pdevice, SWCORE, L2_MULTICAST_TABLE,
								dstTbl.BF.dest_port_or_mc_addr, INDEX_ZERO, mulTbl.val);

						JL_PORT_CMAP_TO_LMAP(pdevice, mulTbl.BF.mc_port_mask, lportval);
						pcfg->sportval = lportval;
					}

					ret = JL_ERR_EXISTS;
					JL_DBG_MSG(JL_FLAG_SWCORE, _DBG_INFO,
						    "Get mode %d hash idx %d entry info mode success\n",
						    JL_L2_SEL_MODE_FID, i);
					break;
				}
			}
			if (SWCORE_L2_AGING_TABLE_NUM == i) {
				g_l2_tbl_search_idx[JL_L2_SEL_MODE_FID] = 0;
				ret = JL_ERR_OK;
			}
			break;
		}
	case JL_L2_SEL_MODE_MULPORTMSK:
		{
			if (!pcfg->sportval) {/*frame drop */
				for (i = g_l2_tbl_search_idx[JL_L2_SEL_MODE_MULPORTMSK] + 1; i < SWCORE_L2_AGING_TABLE_NUM; i++) {
					/*read form driver */
					REG_BURST_READ(pdevice, SWCORE, L2_DESTINATION_TABLE, i, INDEX_ZERO,
							   dstTbl.val);
					REG_BURST_READ(pdevice, SWCORE, L2_AGING_TABLE, i, INDEX_ZERO, ageTbl.val);
					if ((ageTbl.BF.valid) && dstTbl.BF.pkt_drop && (!dstTbl.BF.uc)) {
						g_l2_tbl_search_idx[JL_L2_SEL_MODE_MULPORTMSK] = i;
						pcfg->valid = ageTbl.BF.valid;
						pcfg->stat = ageTbl.BF.stat;

						REG_BURST_READ(pdevice, SWCORE, L2_DA_HASH_LOOKUP_TABLE, i, INDEX_ZERO,
								lookupTbl.val);
						pcfg->fid = lookupTbl.BF.gid;
						pcfg->mac.addr[5] = BITS_OFFSET_R(lookupTbl.BF.mac_addr_0_31, 0, 8);
						pcfg->mac.addr[4] = BITS_OFFSET_R(lookupTbl.BF.mac_addr_0_31, 8, 8);
						pcfg->mac.addr[3] = BITS_OFFSET_R(lookupTbl.BF.mac_addr_0_31, 16, 8);
						pcfg->mac.addr[2] = BITS_OFFSET_R(lookupTbl.BF.mac_addr_0_31, 24, 8);
						pcfg->mac.addr[1] = BITS_OFFSET_R(lookupTbl.BF.mac_addr_32_47, 0, 8);
						pcfg->mac.addr[0] = BITS_OFFSET_R(lookupTbl.BF.mac_addr_32_47, 8, 8);

						REG_BURST_READ(pdevice, SWCORE, L2_DESTINATION_TABLE, i, INDEX_ZERO,
								  dstTbl.val);
						pcfg->type = dstTbl.BF.uc;

						JL_DBG_MSG(JL_FLAG_SWCORE, _DBG_INFO,
							"Get mode %d hash idx %d entry info mode success\n",
							JL_L2_SEL_MODE_MULPORTMSK, i);
						ret = JL_ERR_EXISTS;
						break;
					}
				}
				if (SWCORE_L2_AGING_TABLE_NUM == i) {
					g_l2_tbl_search_idx[JL_L2_SEL_MODE_MULPORTMSK] = -1;
					ret = JL_ERR_OK;
				}
			} else {
				for (j = 0; j < SWCORE_L2_MULTICAST_TABLE_NUM; j++) {
					/*read form driver */
					REG_BURST_READ(pdevice, SWCORE, L2_MULTICAST_TABLE, j, INDEX_ZERO, mulTbl.val);
					JL_PORT_CMAP_TO_LMAP(pdevice, mulTbl.BF.mc_port_mask, lportval);
					if (pcfg->sportval == lportval) {
						for (i = g_l2_tbl_search_idx[JL_L2_SEL_MODE_MULPORTMSK] + 1; i < SWCORE_L2_AGING_TABLE_NUM; i++) {
							/*read form driver */
							REG_BURST_READ(pdevice, SWCORE, L2_DESTINATION_TABLE, i,
									   INDEX_ZERO, dstTbl.val);
							REG_BURST_READ(pdevice, SWCORE, L2_AGING_TABLE, i, INDEX_ZERO,
									ageTbl.val);

							if ((ageTbl.BF.valid) && (dstTbl.BF.dest_port_or_mc_addr == j) && (!dstTbl.BF.uc)) {
								g_l2_tbl_search_idx[JL_L2_SEL_MODE_MULPORTMSK] = i;

								pcfg->valid = ageTbl.BF.valid;
								pcfg->stat = ageTbl.BF.stat;
								REG_BURST_READ(pdevice, SWCORE, L2_DA_HASH_LOOKUP_TABLE, i, INDEX_ZERO, lookupTbl.val);
								pcfg->fid = lookupTbl.BF.gid;
								pcfg->mac.addr[5] =
								    BITS_OFFSET_R(lookupTbl.BF.mac_addr_0_31, 0, 8);
								pcfg->mac.addr[4] =
								    BITS_OFFSET_R(lookupTbl.BF.mac_addr_0_31, 8, 8);
								pcfg->mac.addr[3] =
								    BITS_OFFSET_R(lookupTbl.BF.mac_addr_0_31, 16, 8);
								pcfg->mac.addr[2] =
								    BITS_OFFSET_R(lookupTbl.BF.mac_addr_0_31, 24, 8);
								pcfg->mac.addr[1] =
								    BITS_OFFSET_R(lookupTbl.BF.mac_addr_32_47, 0, 8);
								pcfg->mac.addr[0] =
								    BITS_OFFSET_R(lookupTbl.BF.mac_addr_32_47, 8, 8);

								REG_BURST_READ(pdevice, SWCORE, L2_DESTINATION_TABLE, i,
										  INDEX_ZERO, dstTbl.val);
								pcfg->type = dstTbl.BF.uc;

								JL_DBG_MSG(JL_FLAG_SWCORE, _DBG_INFO,
									      "Get mode %d hash idx %d entry info mode success\n",
									      JL_L2_SEL_MODE_MULPORTMSK, i);

								ret = JL_ERR_EXISTS;
								break;
							}
						}

						if (SWCORE_L2_AGING_TABLE_NUM == i) {
							g_l2_tbl_search_idx[JL_L2_SEL_MODE_MULPORTMSK] = -1;
							ret = JL_ERR_OK;
						}
						break;
					}
				}
			}
			break;
		}
	case JL_L2_SEL_MODE_SPECIFIC:
		{
			/*cal with tbl input val */
			check_hash = __l2_hash(&(pcfg->mac), pcfg->fid);
			if (g_l2_tbl_search_idx[JL_L2_SEL_MODE_SPECIFIC] + 1 < L2_HASH_WAY_VALUE) {
				g_l2_tbl_search_idx[JL_L2_SEL_MODE_SPECIFIC]++;
				REG_BURST_READ(pdevice, SWCORE, L2_AGING_TABLE, check_hash + (g_l2_tbl_search_idx[JL_L2_SEL_MODE_SPECIFIC] << L2_HASH_WIDTH),
						INDEX_ZERO, ageTbl.val);
				REG_BURST_READ(pdevice, SWCORE, L2_DESTINATION_TABLE, check_hash + (g_l2_tbl_search_idx[JL_L2_SEL_MODE_SPECIFIC] << L2_HASH_WIDTH),
						INDEX_ZERO, dstTbl.val);
				REG_BURST_READ(pdevice, SWCORE, L2_DA_HASH_LOOKUP_TABLE, g_l2_tbl_search_idx[JL_L2_SEL_MODE_SPECIFIC], INDEX_ZERO, lookupTbl.val);

				pcfg->valid = ageTbl.BF.valid;
				pcfg->stat = ageTbl.BF.stat;
				pcfg->type = dstTbl.BF.uc;
				pcfg->fid = lookupTbl.BF.gid;

				if (dstTbl.BF.uc) {
					JL_PORT_C2L_MACRO(pdevice, dstTbl.BF.dest_port_or_mc_addr, lportval);
					pcfg->sportval = lportval;
				} else {
					REG_BURST_READ(pdevice, SWCORE, L2_MULTICAST_TABLE, dstTbl.BF.dest_port_or_mc_addr,
							INDEX_ZERO, mulTbl.val);
					JL_PORT_CMAP_TO_LMAP(pdevice, mulTbl.BF.mc_port_mask, lportval);
					pcfg->sportval = lportval;
				}

				ret = JL_ERR_EXISTS;
				JL_DBG_MSG(JL_FLAG_SWCORE, _DBG_INFO, "Get mod %d hash idx %d entry info mode success\n",
					    JL_L2_SEL_MODE_SPECIFIC, check_hash + (g_l2_tbl_search_idx[JL_L2_SEL_MODE_SPECIFIC] << L2_HASH_WIDTH));
			} else {
				g_l2_tbl_search_idx[JL_L2_SEL_MODE_SPECIFIC] = -1;
				ret = JL_ERR_OK;
			}
			break;
		}
	default:
		{
			JL_DBG_MSG(JL_FLAG_SWCORE, _DBG_ERROR, "Not support this mode yet\n");
			ret = JL_ERR_PARAM;
		}
	}
	return ret;
}

jl_ret_t jl61xx_l2_table_clear(jl_device_t *pdevice, const jl_l2_sel_mode_t mode, jl_l2_tbl_t *pcfg)
{
	jl_ret_t ret = JL_ERR_OK;
	jl_uint32 lportval;
	jl_uint32 check_hash;
	jl_uint32 i, j;

	SWCORE_L2_AGING_TABLE_t ageTbl;
	SWCORE_L2_DA_HASH_LOOKUP_TABLE_t lookupTbl;
	SWCORE_L2_DESTINATION_TABLE_t dstTbl;

	SWCORE_L2_LOOKUP_COLLISION_TABLE_t colTbl;
	SWCORE_L2_AGING_COLLISION_TABLE_t colAgeTbl;

	SWCORE_L2_MULTICAST_TABLE_t mulTbl;

	SWCORE_L2_AGING_STATUS_SHADOW_TABLE_t shadowAgeTbl;

	SWCORE_L2_AGING_COLLISION_SHADOW_TABLE_t colShadowTbl;

	memset((void *)&shadowAgeTbl, 0, sizeof(SWCORE_L2_AGING_STATUS_SHADOW_TABLE_t));
	memset((void *)&colShadowTbl, 0, sizeof(SWCORE_L2_AGING_COLLISION_SHADOW_TABLE_t));

	/*para check */
	if (JL_L2_SEL_MODE_MULPORTMSK == mode)
		JL_CHECK_PORTMASK(pdevice, pcfg->sportval);
	else if (JL_L2_SEL_MODE_SPORT == mode) {
		if (likely(JL_L2_MAC_DROPPORT_UC != pcfg->sportval))
			JL_CHECK_PORT(pdevice, pcfg->sportval);
	}

	switch (mode) {
	case JL_L2_SEL_MODE_STATIC:
		{
			for (i = 0; i < SWCORE_L2_AGING_TABLE_NUM; i++) {
				/*Read from driver */
				REG_BURST_READ(pdevice, SWCORE, L2_AGING_TABLE, i, INDEX_ZERO, ageTbl.val);
				if (ageTbl.BF.stat) {
					/*Set hit & valid to 0 */
					memset((void *)&ageTbl, 0, sizeof(SWCORE_L2_AGING_TABLE_t));

					/*Send to driver */
					REG_BURST_WRITE(pdevice, SWCORE, L2_AGING_TABLE, i, INDEX_ZERO, ageTbl.val);
					REG_BURST_WRITE(pdevice, SWCORE, L2_AGING_STATUS_SHADOW_TABLE, i, INDEX_ZERO,
							 shadowAgeTbl.val);
				}
			}
			for (i = 0; i < L2_COL_TBL_MAX_DEPTH; i++) {
				/*Read from driver */
				REG_BURST_READ(pdevice, SWCORE, L2_AGING_COLLISION_TABLE, i, INDEX_ZERO, colAgeTbl.val);
				if (colAgeTbl.BF.stat) {
					/*Set hit & valid to 0*/
					memset((void *)&colAgeTbl, 0, sizeof(SWCORE_L2_AGING_COLLISION_TABLE_t));

					/*Send to driver*/
					REG_BURST_WRITE(pdevice, SWCORE, L2_AGING_COLLISION_TABLE, i, INDEX_ZERO, colAgeTbl.val);
					REG_BURST_WRITE(pdevice, SWCORE, L2_AGING_COLLISION_SHADOW_TABLE, i, INDEX_ZERO,
							 colShadowTbl.val);
				}
			}
			JL_DBG_MSG(JL_FLAG_SWCORE, _DBG_INFO, "Clear mod %d entry info mode success\n",
				    JL_L2_SEL_MODE_STATIC);
			ret = JL_ERR_OK;
			break;
		}
	case JL_L2_SEL_MODE_SPORT:
		{
			for (i = 0; i < (SWCORE_L2_AGING_TABLE_NUM + L2_COL_TBL_MAX_DEPTH); i++) {
				/*Read from driver */
				REG_BURST_READ(pdevice, SWCORE, L2_DESTINATION_TABLE, i, INDEX_ZERO, dstTbl.val);
				if (unlikely(JL_L2_MAC_DROPPORT_UC == pcfg->sportval)) {
					if (dstTbl.BF.uc && dstTbl.BF.pkt_drop) {
						if (i < SWCORE_L2_AGING_TABLE_NUM) {
							/*Set hit & valid to 0 */
							memset((void *)&ageTbl, 0, sizeof(SWCORE_L2_AGING_TABLE_t));

							/*Send to driver */
							REG_BURST_WRITE(pdevice, SWCORE, L2_AGING_TABLE, i, INDEX_ZERO,
								    ageTbl.val);
							REG_BURST_WRITE(pdevice, SWCORE, L2_AGING_STATUS_SHADOW_TABLE, i,
									 INDEX_ZERO, shadowAgeTbl.val);
						} else {
							/*Set hit & valid to 0*/
							memset((void *)&colAgeTbl, 0, sizeof(SWCORE_L2_AGING_COLLISION_TABLE_t));

							/*Send to driver*/
							REG_BURST_WRITE(pdevice, SWCORE, L2_AGING_COLLISION_TABLE,
											(i - SWCORE_L2_AGING_TABLE_NUM), INDEX_ZERO, colAgeTbl.val);
							REG_BURST_WRITE(pdevice, SWCORE, L2_AGING_COLLISION_SHADOW_TABLE,
											(i - SWCORE_L2_AGING_TABLE_NUM), INDEX_ZERO, colShadowTbl.val);
						}
					}
				} else {
					JL_PORT_C2L_MACRO(pdevice, dstTbl.BF.dest_port_or_mc_addr, lportval);
					if (dstTbl.BF.uc && pcfg->sportval == lportval) {
						if (i < SWCORE_L2_AGING_TABLE_NUM) {
							/*Set hit & valid to 0 */
							memset((void *)&ageTbl, 0, sizeof(SWCORE_L2_AGING_TABLE_t));

							/*Send to driver */
							REG_BURST_WRITE(pdevice, SWCORE, L2_AGING_TABLE, i, INDEX_ZERO,
									    ageTbl.val);
							REG_BURST_WRITE(pdevice, SWCORE, L2_AGING_STATUS_SHADOW_TABLE, i,
									 INDEX_ZERO, shadowAgeTbl.val);
						} else {
							/*Set hit & valid to 0*/
							memset((void *)&colAgeTbl, 0, sizeof(SWCORE_L2_AGING_COLLISION_TABLE_t));

							/*Send to driver*/
							REG_BURST_WRITE(pdevice, SWCORE, L2_AGING_COLLISION_TABLE,
											(i - SWCORE_L2_AGING_TABLE_NUM), INDEX_ZERO, colAgeTbl.val);
							REG_BURST_WRITE(pdevice, SWCORE, L2_AGING_COLLISION_SHADOW_TABLE,
											(i - SWCORE_L2_AGING_TABLE_NUM), INDEX_ZERO,
									 colShadowTbl.val);
						}
					}
				}
			}
			JL_DBG_MSG(JL_FLAG_SWCORE, _DBG_INFO, "Clear mod %d entry info mode success\n",
				    JL_L2_SEL_MODE_SPORT);
			ret = JL_ERR_OK;
			break;
		}
	case JL_L2_SEL_MODE_FID:
		{
			JL_CHECK_ENUM(pcfg->fid, (L2_FID_MAX_VALUE + 1));
			for (i = 0; i < SWCORE_L2_AGING_TABLE_NUM; i++) {
				REG_BURST_READ(pdevice, SWCORE, L2_DA_HASH_LOOKUP_TABLE, i, INDEX_ZERO, lookupTbl.val);
				REG_BURST_READ(pdevice, SWCORE, L2_AGING_TABLE, i, INDEX_ZERO, ageTbl.val);
				/*for FID 0 default val, check hit*/
				if (pcfg->fid == lookupTbl.BF.gid && ageTbl.BF.valid) {
					/*Set hit & valid to 0 */
					memset((void *)&ageTbl, 0, sizeof(SWCORE_L2_AGING_TABLE_t));

					/*Send to driver */
					REG_BURST_WRITE(pdevice, SWCORE, L2_AGING_TABLE, i, INDEX_ZERO, ageTbl.val);
					REG_BURST_WRITE(pdevice, SWCORE, L2_AGING_STATUS_SHADOW_TABLE, i, INDEX_ZERO,
							 shadowAgeTbl.val);
				}
			}
			for (i = 0; i < L2_COL_TBL_MAX_DEPTH; i++) {
				REG_BURST_READ(pdevice, SWCORE, L2_LOOKUP_COLLISION_TABLE, i, INDEX_ZERO, colTbl.val);
				REG_BURST_READ(pdevice, SWCORE, L2_AGING_COLLISION_TABLE, i, INDEX_ZERO, colAgeTbl.val);
				if (pcfg->fid == colTbl.BF.gid && colAgeTbl.BF.valid) {
					/*Set hit & valid to 0*/
					memset((void *)&colTbl, 0, sizeof(SWCORE_L2_LOOKUP_COLLISION_TABLE_t));

					/*Send to driver*/
					REG_BURST_WRITE(pdevice, SWCORE, L2_AGING_COLLISION_TABLE, i, INDEX_ZERO, colAgeTbl.val);
					REG_BURST_WRITE(pdevice, SWCORE, L2_AGING_COLLISION_SHADOW_TABLE, i, INDEX_ZERO,
							 colShadowTbl.val);
				}
			}
			JL_DBG_MSG(JL_FLAG_SWCORE, _DBG_INFO, "Clear mod %d entry info mode success\n",
				    JL_L2_SEL_MODE_FID);
			ret = JL_ERR_OK;
			break;
		}
	case JL_L2_SEL_MODE_MULPORTMSK:
		{
			if (!pcfg->sportval) {
				for (i = 0; i < (SWCORE_L2_AGING_TABLE_NUM + L2_COL_TBL_MAX_DEPTH); i++) {
					REG_BURST_READ(pdevice, SWCORE, L2_DESTINATION_TABLE, i, INDEX_ZERO, dstTbl.val);
					if ((!dstTbl.BF.uc) && dstTbl.BF.pkt_drop) {
						if (i < SWCORE_L2_AGING_TABLE_NUM) {
							/*Set hit & valid to 0 */
							memset((void *)&ageTbl, 0, sizeof(SWCORE_L2_AGING_TABLE_t));

							/*Send to driver */
							REG_BURST_WRITE(pdevice, SWCORE, L2_AGING_TABLE, i, INDEX_ZERO,
								    ageTbl.val);
							REG_BURST_WRITE(pdevice, SWCORE, L2_AGING_STATUS_SHADOW_TABLE, i,
									 INDEX_ZERO, shadowAgeTbl.val);
						} else {
							/*Set hit & valid to 0*/
							memset((void *)&colAgeTbl, 0, sizeof(SWCORE_L2_AGING_COLLISION_TABLE_t));

							/*Send to driver*/
							REG_BURST_WRITE(pdevice, SWCORE, L2_AGING_COLLISION_TABLE,
											(i - SWCORE_L2_AGING_TABLE_NUM), INDEX_ZERO, colAgeTbl.val);
							REG_BURST_WRITE(pdevice, SWCORE, L2_AGING_COLLISION_SHADOW_TABLE,
											(i - SWCORE_L2_AGING_TABLE_NUM), INDEX_ZERO,
									 colShadowTbl.val);
						}
					}
				}
			} else {
				for (j = 0; j < SWCORE_L2_MULTICAST_TABLE_NUM; j++) {
					/*Read from driver */
					REG_BURST_READ(pdevice, SWCORE, L2_MULTICAST_TABLE, j, INDEX_ZERO, mulTbl.val);
					JL_PORT_CMAP_TO_LMAP(pdevice, mulTbl.BF.mc_port_mask, lportval);
					if (pcfg->sportval == lportval) {
						for (i = 0; i < (SWCORE_L2_AGING_TABLE_NUM + L2_COL_TBL_MAX_DEPTH); i++) {
							REG_BURST_READ(pdevice, SWCORE, L2_DESTINATION_TABLE, i, INDEX_ZERO, dstTbl.val);
							if ((!dstTbl.BF.uc) && (j == dstTbl.BF.dest_port_or_mc_addr)) {
								if (i < SWCORE_L2_AGING_TABLE_NUM) {
									/*Set hit & valid to 0 */
									memset((void *)&ageTbl, 0, sizeof(SWCORE_L2_AGING_TABLE_t));

									/*Send to driver */
									REG_BURST_WRITE(pdevice, SWCORE, L2_AGING_TABLE, i,
											    INDEX_ZERO, ageTbl.val);
									REG_BURST_WRITE(pdevice, SWCORE,
											 L2_AGING_STATUS_SHADOW_TABLE, i,
											 INDEX_ZERO, shadowAgeTbl.val);
								} else {
									/*Set hit & valid to 0*/
									memset((void *)&colAgeTbl, 0, sizeof(SWCORE_L2_AGING_COLLISION_TABLE_t));

									/*Send to driver*/
									REG_BURST_WRITE(pdevice, SWCORE, L2_AGING_COLLISION_TABLE,
										(i - SWCORE_L2_AGING_TABLE_NUM), INDEX_ZERO, colAgeTbl.val);
									REG_BURST_WRITE(pdevice, SWCORE, L2_AGING_COLLISION_SHADOW_TABLE,
										(i - SWCORE_L2_AGING_TABLE_NUM), INDEX_ZERO, colShadowTbl.val);
								}
							}
						}
					}
				}
			}
			JL_DBG_MSG(JL_FLAG_SWCORE, _DBG_INFO, "Clear mod %d entry info mode success\n",
				    JL_L2_SEL_MODE_MULPORTMSK);
			ret = JL_ERR_OK;
			break;
		}
	case JL_L2_SEL_MODE_SPECIFIC:
		{
			/*cal with tbl input val */
			check_hash = __l2_hash(&(pcfg->mac), pcfg->fid);
			for (i = 0; i < L2_HASH_WAY_VALUE; i++) {
				/*Set hit & valid to 0 */
				memset((void *)&ageTbl, 0, sizeof(SWCORE_L2_AGING_TABLE_t));

				/*Send to driver */
				REG_BURST_WRITE(pdevice, SWCORE, L2_AGING_TABLE, check_hash + (i << L2_HASH_WIDTH),
						    INDEX_ZERO, ageTbl.val);
				REG_BURST_WRITE(pdevice, SWCORE, L2_AGING_STATUS_SHADOW_TABLE,
						 check_hash + (i << L2_HASH_WIDTH), INDEX_ZERO, shadowAgeTbl.val);
			}
			for (i = 0; i < L2_COL_TBL_MAX_DEPTH; i++) {
				if (JL_ERR_EXISTS == _jl61xx_l2_hash_col_check(pdevice, pcfg, i)) {
					memset((void *)&colAgeTbl, 0, sizeof(SWCORE_L2_AGING_COLLISION_TABLE_t));
					/*Send to driver */
					REG_BURST_WRITE(pdevice, SWCORE, L2_AGING_COLLISION_TABLE, i, INDEX_ZERO, colAgeTbl.val);
					REG_BURST_WRITE(pdevice, SWCORE, L2_AGING_COLLISION_SHADOW_TABLE, i, INDEX_ZERO, colShadowTbl.val);
				}
			}

			JL_DBG_MSG(JL_FLAG_SWCORE, _DBG_INFO, "Clear mod %d entry info mode success\n",
				    JL_L2_SEL_MODE_SPECIFIC);

			ret = JL_ERR_OK;
			break;
		}
	default:
		{
			JL_DBG_MSG(JL_FLAG_SWCORE, _DBG_ERROR, "Not support this mode yet\n");
			ret = JL_ERR_PARAM;
		}
	}
	return ret;
}

jl_ret_t jl61xx_l2_dynamic_fast_clear(jl_device_t *pdevice)
{
	jl_ret_t ret = JL_ERR_OK;
	jl_uint32 cur_tick_cnt = 0;
	jl_uint32 cur_tick_index = 0;

	const jl_uint32 tmp_tick_cnt = 1;
	const jl_uint32 tmp_tick_index = 0;

	jl_uint16 delayms = 1000;

	/*get age time setting */

	ret = _jl61xx_l2_aging_time_get(pdevice, &cur_tick_cnt, &cur_tick_index);

	ret = _jl61xx_l2_aging_time_set(pdevice, tmp_tick_cnt, tmp_tick_index);

	do {
		    /* wait for timeout */
	} while (delayms--);

	ret = _jl61xx_l2_aging_time_set(pdevice, cur_tick_cnt, cur_tick_index);

	JL_DBG_MSG(JL_FLAG_SWCORE, _DBG_INFO, "Clear dynamic entry info mode success\n");
	return ret;
}

jl_ret_t jl61xx_l2_hash_col_table_set(jl_device_t *pdevice, const jl_uint32 idx, jl_l2_tbl_t *pcfg)
{
	jl_ret_t ret = JL_ERR_OK;

	/*para check */
	JL_CHECK_ENUM(pcfg->fid, (L2_FID_MAX_VALUE + 1));
	JL_CHECK_ENUM(idx, L2_COL_TBL_MAX_DEPTH);

	ret = _jl61xx_l2_col_tbl_update(pdevice, pcfg, idx);

	JL_DBG_MSG(JL_FLAG_SWCORE, _DBG_INFO, "Set hash table entry info mode success\n");
	return ret;
}

jl_ret_t jl61xx_l2_hash_col_table_get(jl_device_t *pdevice, const jl_uint32 idx, jl_l2_tbl_t *pcfg)
{
	jl_ret_t ret = JL_ERR_OK;

	/*para check */
	JL_CHECK_ENUM(idx, L2_COL_TBL_MAX_DEPTH);

	ret = _jl61xx_l2_col_tbl_get(pdevice, pcfg, idx);

	JL_DBG_MSG(JL_FLAG_SWCORE, _DBG_INFO, "Get hash table entry info mode success\n");
	return ret;
}


/*  age time = sys_tick_mode * tick_cnt
 *   This will auto cal in driver
 *   Suggest do not change sys clk , so age time will have deviation (round up)
 *   For shadow table, suggest sys age time = (2/3) age time setting
 *   sys clk default is 0x19(1Mhz), step is 0xa, so clk level is 1us, 10us, 100us, 1ms, 10ms
 */
jl_ret_t jl61xx_l2_set_agetime(jl_device_t *pdevice, const jl_uint32 time)
{
	jl_ret_t ret = JL_ERR_OK;
	jl_uint32 cur_time;

	cur_time = time * L2_AGE_TIME_BASE / L2_HW_TIME_BASE;

	if (unlikely(jl61xx_calc_tick_freq(pdevice) == JL_ERR_OK))
		cur_time = cur_time * 1000 / g_tick_freq_list[L2_AGE_TIME_DEFAULT_MS_LEVEL];

	ret = _jl61xx_l2_aging_time_set(pdevice, cur_time, L2_AGE_TIME_DEFAULT_MS_LEVEL);

	JL_DBG_MSG(JL_FLAG_SWCORE, _DBG_INFO, "Set age time %d hw time %d info mode success\n", time, cur_time);

	return ret;
}

jl_ret_t jl61xx_l2_get_agetime(jl_device_t *pdevice, jl_uint32 *ptime)
{
	jl_ret_t ret = JL_ERR_OK;
	jl_uint32 cur_time;
	jl_uint32 cur_level;

	ret = _jl61xx_l2_aging_time_get(pdevice, &cur_time, &cur_level);

	if (ret)
		return ret;

	if (unlikely(jl61xx_calc_tick_freq(pdevice) == JL_ERR_OK))
		cur_time = cur_time * g_tick_freq_list[L2_AGE_TIME_DEFAULT_MS_LEVEL] / 1000;

	*ptime = cur_time * L2_HW_TIME_BASE / L2_AGE_TIME_BASE;

	JL_DBG_MSG(JL_FLAG_SWCORE, _DBG_INFO, "Get age time %d hw time %d info mode success\n", *ptime, cur_time);
	return JL_ERR_OK;
}

jl_ret_t jl61xx_l2_mc_handle_set(jl_device_t *pdevice, jl_l2_mc_handle_t mcHandle, jl_bool enable)
{
	jl_ret_t ret = JL_ERR_OK;
	SWCORE_L2_MULTICAST_HANDLING_t cfg;

	/*read from driver */
	REG_BURST_READ(pdevice, SWCORE, L2_MULTICAST_HANDLING, INDEX_ZERO, INDEX_ZERO, cfg.val);

	switch (mcHandle) {
	case JL_L2_MC_HANDLE_EXC_IPV4:
		{
			if (enable)
				cfg.BF.excl_ipv4_mc = TRUE;
			else
				cfg.BF.excl_ipv4_mc = FALSE;
			break;
		}
	case JL_L2_MC_HANDLE_EXC_IPV6:
		{
			if (enable)
				cfg.BF.excl_ipv6_mc = TRUE;
			else
				cfg.BF.excl_ipv6_mc = FALSE;
			break;
		}
	case JL_L2_MC_HANDLE_INC_L2MC:
		{
			if (enable)
				cfg.BF.incl_l2_mc_lut = TRUE;
			else
				cfg.BF.incl_l2_mc_lut = FALSE;
			break;
		}
	case JL_L2_MC_HANDLE_INC_MULTIPORT:
		{
			if (enable)
				cfg.BF.incl_multi_ports = TRUE;
			else
				cfg.BF.incl_multi_ports = FALSE;
			break;
		}
	default:
		return JL_ERR_PARAM;
	}

	/*send to driver */
	REG_BURST_WRITE(pdevice, SWCORE, L2_MULTICAST_HANDLING, INDEX_ZERO, INDEX_ZERO, cfg.val);

	JL_DBG_MSG(JL_FLAG_SWCORE, _DBG_INFO, "Set mc_handle age reg %x success\n", cfg.val[0]);
	return ret;
}

jl_ret_t jl61xx_l2_mc_table_set(jl_device_t *pdevice, const jl_uint32 idx, const jl_uint32 portMask)
{
	jl_ret_t ret = JL_ERR_OK;
	jl_uint32 mac_portmsk;

	SWCORE_L2_MULTICAST_TABLE_t cfg;

	/*para check */
	JL_CHECK_ENUM(idx, SWCORE_L2_MULTICAST_TABLE_NUM);
	JL_CHECK_PORTMASK(pdevice, portMask);

	JL_PORT_LMAP_TO_CMAP(pdevice, portMask, mac_portmsk);
	cfg.BF.mc_port_mask = mac_portmsk;

	/*send to driver */
	REG_BURST_WRITE(pdevice, SWCORE, L2_MULTICAST_TABLE, idx, INDEX_ZERO, cfg.val);

	JL_DBG_MSG(JL_FLAG_SWCORE, _DBG_INFO, "Set mc table idx %d port msk %x success, age reg %x\n", idx, portMask,
		      cfg.val[0]);
	return ret;
}

jl_ret_t jl61xx_l2_mc_table_get(jl_device_t *pdevice, const jl_uint32 idx, jl_uint32 *pPortMask)
{
	jl_ret_t ret = JL_ERR_OK;

	SWCORE_L2_MULTICAST_TABLE_t cfg;

	/*para check */
	JL_CHECK_ENUM(idx, SWCORE_L2_MULTICAST_TABLE_NUM);

	/*read from driver */
	REG_BURST_READ(pdevice, SWCORE, L2_MULTICAST_TABLE, idx, INDEX_ZERO, cfg.val);

	JL_PORT_CMAP_TO_LMAP(pdevice, cfg.BF.mc_port_mask, *pPortMask);

	JL_DBG_MSG(JL_FLAG_SWCORE, _DBG_INFO, "Get mc table idx %d port msk %x success, age reg %x\n", idx, *pPortMask,
		      cfg.val[0]);
	return ret;
}


/*This func is for specific hash operation*/
jl_ret_t jl61xx_l2_set_spec_hash_tbl(
	jl_device_t *pdevice,
	const jl_uint32 hash_idx,
	const L2_HASH_TABLE_MODE_T mode,
	void *data)
{
	jl_ret_t ret = JL_ERR_OK;

	SWCORE_L2_DA_HASH_LOOKUP_TABLE_t *pLookupTbl;
	SWCORE_L2_AGING_TABLE_t *pAgeTbl;
	SWCORE_L2_DESTINATION_TABLE_t *pDstTbl;

	switch (mode) {
	case L2_HASH_TABLE_MODE_LOOKUP:
		{
			pLookupTbl = (SWCORE_L2_DA_HASH_LOOKUP_TABLE_t *) data;

			/*send to driver */
			REG_BURST_WRITE(pdevice, SWCORE, L2_DA_HASH_LOOKUP_TABLE, hash_idx, INDEX_ZERO,
				    pLookupTbl->val);
			break;
		}
	case L2_HASH_TABLE_MODE_AGE:
		{
			pAgeTbl = (SWCORE_L2_AGING_TABLE_t *) data;

			/*send to driver */
			REG_BURST_WRITE(pdevice, SWCORE, L2_AGING_TABLE, hash_idx, INDEX_ZERO, pAgeTbl->val);
			break;
		}
	case L2_HASH_TABLE_MODE_DESTINATION:
		{
			pDstTbl = (SWCORE_L2_DESTINATION_TABLE_t *) data;

			/*send to driver */
			REG_BURST_WRITE(pdevice, SWCORE, L2_DESTINATION_TABLE, hash_idx, INDEX_ZERO, pDstTbl->val);
			break;
		}
	default:
		{
			JL_DBG_MSG(JL_FLAG_SWCORE, _DBG_ERROR, "Not support this mode yet\n");
			ret = JL_ERR_PARAM;
		}
	}

	JL_DBG_MSG(JL_FLAG_SWCORE, _DBG_INFO, "Set specific mode %d hash idx %d table success\n", mode, hash_idx);
	return ret;
}


