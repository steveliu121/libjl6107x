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

#include "jl_acl.h"
#include "jl_ops.h"

jl_api_ret_t jl_acl_template_set(const jl_uint32 chip_id,
									const jl_uint8 index,
									jl_acl_template_t *ptempl)
{
	jl_api_ret_t ret;

	JL_CHECK_POINTER(ptempl);
	JL_CHECK_ENUM(index, ACL_TEMPLATE_NUM);

	JL_DRV_SAFE_CALL(ret, acl_template_set, chip_id, index, ptempl);

	return ret;
}


jl_api_ret_t jl_acl_template_get(const jl_uint32 chip_id,
									const jl_uint8 index,
									jl_acl_template_t *ptempl)
{
	jl_api_ret_t ret;

	JL_CHECK_POINTER(ptempl);
	JL_CHECK_ENUM(index, ACL_TEMPLATE_NUM);

	JL_DRV_SAFE_CALL(ret, acl_template_get, chip_id, index, ptempl);

	return ret;
}

jl_api_ret_t jl_acl_field_insert(const jl_uint32 chip_id,
									jl_acl_field_data_t *pfield,
									jl_acl_rule_t *prule)
{
	jl_api_ret_t ret;

	JL_CHECK_POINTER(pfield);
	JL_CHECK_POINTER(prule);

	JL_DRV_SAFE_CALL(ret, acl_field_insert, chip_id, pfield, prule);

	return ret;
}


jl_api_ret_t jl_acl_field_clear(const jl_uint32 chip_id, jl_acl_rule_t *prule)
{
	jl_api_ret_t ret;

	JL_CHECK_POINTER(prule);

	JL_DRV_SAFE_CALL(ret, acl_field_clear, chip_id, prule);

	return ret;
}

jl_api_ret_t jl_acl_rule_set(const jl_uint32 chip_id,
									const jl_uint8 index,
									jl_acl_rule_t *prule,
									jl_acl_action_t *paction)

{
	jl_api_ret_t ret;

	JL_CHECK_POINTER(prule);
	JL_CHECK_POINTER(paction);
	JL_CHECK_ENUM(index, ACL_TBL_RULE_NUM);

	JL_DRV_SAFE_CALL(ret, acl_rule_add, chip_id, index, prule, paction);

	return ret;
}

jl_api_ret_t jl_acl_rule_get(const jl_uint32 chip_id,
									const jl_uint8 index,
									jl_acl_rule_t *prule,
									jl_acl_action_t *paction)

{
	jl_api_ret_t ret;

	JL_CHECK_POINTER(prule);
	JL_CHECK_POINTER(paction);
	JL_CHECK_ENUM(index, ACL_TBL_RULE_NUM);

	JL_DRV_SAFE_CALL(ret, acl_rule_get, chip_id, index, prule, paction);

	return ret;
}


jl_api_ret_t jl_acl_rule_del(const jl_uint32 chip_id, const jl_uint8 index)

{
	jl_api_ret_t ret;

	JL_CHECK_ENUM(index, ACL_TBL_RULE_NUM);

	JL_DRV_SAFE_CALL(ret, acl_rule_del, chip_id, index);

	return ret;
}


jl_api_ret_t jl_acl_rule_del_all(const jl_uint32 chip_id)
{
	jl_api_ret_t ret;
	jl_uint32 idx;

	for (idx = 0; idx < ACL_TBL_RULE_NUM; idx++)
	{
		ret = jl_acl_rule_del(chip_id, idx);
		if (ret)
			return ret;
	}

	return JL_ERR_OK;
}

jl_api_ret_t jl_acl_multi_hits_selection_set(const jl_uint32 chip_id, const jl_uint8 val)

{
	jl_api_ret_t ret;

	if (val > 3)
		return JL_ERR_PARAM;

	JL_DRV_SAFE_CALL(ret, acl_selection_set, chip_id, val);

	return ret;
}

jl_api_ret_t jl_acl_multi_hits_selection_get(const jl_uint32 chip_id, jl_uint8 *pval)

{
	jl_api_ret_t ret;

	JL_CHECK_POINTER(pval);

	JL_DRV_SAFE_CALL(ret, acl_selection_get, chip_id, pval);

	return ret;
}

jl_api_ret_t jl_acl_stat_read(const jl_uint32 chip_id,
									const jl_uint8 counter,
									jl_uint32 *ppackets)
{
	jl_api_ret_t ret;

	JL_CHECK_POINTER(ppackets);
	JL_CHECK_ENUM(counter, ACL_MATCH_COUNTER);

	JL_DRV_SAFE_CALL(ret, acl_stat_read, chip_id, counter, ppackets);

	return ret;
}

jl_api_ret_t jl_acl_stat_clear(const jl_uint32 chip_id, const jl_uint8 counter)
{
	jl_api_ret_t ret;

	JL_CHECK_ENUM(counter, ACL_MATCH_COUNTER);

	JL_DRV_SAFE_CALL(ret, acl_stat_write, chip_id, counter, 0);

	return ret;
}


jl_api_ret_t jl_acl_stat_enable(const jl_uint32 chip_id,
										const jl_uint8 index,
										jl_uint8 counter)
{
	jl_api_ret_t ret;

	JL_CHECK_ENUM(index, ACL_TBL_RULE_NUM);
	JL_CHECK_ENUM(counter, ACL_MATCH_COUNTER);

	JL_DRV_SAFE_CALL(ret, acl_stat_enable, chip_id, index, counter);

	return ret;
}

jl_api_ret_t jl_acl_stat_disable(const jl_uint32 chip_id, const jl_uint8 index)
{
	jl_api_ret_t ret;

	JL_CHECK_ENUM(index, ACL_TBL_RULE_NUM);

	JL_DRV_SAFE_CALL(ret, acl_stat_disable, chip_id, index);

	return ret;
}

jl_api_ret_t jl_acl_stat_counter_get(const jl_uint32 chip_id, jl_uint8 *pcounter)
{
	jl_api_ret_t ret;

	JL_CHECK_POINTER(pcounter);

	JL_DRV_SAFE_CALL(ret, acl_stat_counter_get, chip_id, pcounter);

	return ret;
}


jl_api_ret_t jl_acl_macrange_action_set(const jl_uint32 chip_id,
										const jl_uint8 index,
										jl_acl_mac_range_t *pmac,
										jl_acl_mac_action_t *paction)
{
	jl_api_ret_t ret;

	JL_CHECK_POINTER(pmac);
	JL_CHECK_POINTER(paction);

	JL_CHECK_ENUM(index, ACL_MAC_RULE_NUM);

	JL_DRV_SAFE_CALL(ret, acl_mac_rule_add, chip_id, index, pmac, paction);

	return ret;
}


jl_api_ret_t jl_acl_macrange_action_get(const jl_uint32 chip_id,
										const jl_uint8 index,
										const jl_uint8 mac_type,
										jl_acl_mac_range_t *pmac,
										jl_acl_mac_action_t *paction)

{
	jl_api_ret_t ret;

	JL_CHECK_POINTER(pmac);
	JL_CHECK_POINTER(paction);

	JL_CHECK_ENUM(index, ACL_MAC_RULE_NUM);
	JL_CHECK_ENUM(mac_type, ACL_MAC_TYPE_NUM);

	JL_DRV_SAFE_CALL(ret, acl_mac_rule_get, chip_id, index, mac_type, pmac, paction);

	return ret;
}

jl_api_ret_t jl_acl_macrange_action_del(const jl_uint32 chip_id,
										const jl_uint8 index,
										const jl_uint8 mac_type)
{
	jl_api_ret_t ret;

	JL_CHECK_ENUM(index, ACL_MAC_RULE_NUM);
	JL_CHECK_ENUM(mac_type, ACL_MAC_TYPE_NUM);

	JL_DRV_SAFE_CALL(ret, acl_mac_rule_del, chip_id, index, mac_type);

	return ret;
}


jl_api_ret_t jl_acl_port_set(const jl_uint32 chip_id,
								const jl_uint8 inport,
								jl_acl_port_t *pport)
{
	jl_api_ret_t ret;

	JL_CHECK_POINTER(pport);

	JL_DRV_SAFE_CALL(ret, acl_source_port_set, chip_id, inport, pport);

	return ret;
}

jl_api_ret_t jl_acl_port_get(const jl_uint32 chip_id,
								const jl_uint8 inport,
								jl_acl_port_t *pport)
{
	jl_api_ret_t ret;

	JL_CHECK_POINTER(pport);

	JL_DRV_SAFE_CALL(ret, acl_source_port_get, chip_id, inport, pport);

	return ret;
}

