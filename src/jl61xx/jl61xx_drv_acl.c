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

#include "jl61xx/jl61xx_drv_acl.h"
#include "jl61xx/jl61xx_drv_switch.h"
#include "jl61xx/jl61xx_chip.h"

CONST_T jl_uint8 jl61xx_acl_field_bit_size[ACL_FIELD_NUM] = {
	0, 4, 48, 48, 12, 1, 1, 1, 3, 1,
	12, 3, 1, 16, 2, 32, 32, 128, 128, 20,
	8, 8, 8, 3, 16, 16, 128, 8, 8, 8,
	8, 8, 32, 3
};

static jl61xx_acl_template_t *gp_acl_template[JL_MAX_CHIP_NUM] = {0};
static jl61xx_acl_rule_info_t *gp_acl_rule_info[JL_MAX_CHIP_NUM] = {0};

static jl_ret_t jl61xx_acl_l2v(void*, jl_uint8, jl61xx_acl_val128_t*);
static jl_ret_t jl61xx_acl_v2l(jl61xx_acl_val128_t* , jl_uint8, void *);
static jl_ret_t jl61xx_acl_field_chk(jl_device_t*, jl_acl_rule_t*, jl_acl_field_data_t*);
static jl_ret_t jl61xx_acl_clear_tcam(jl_device_t*, jl_uint8);
static jl_ret_t jl61xx_acl_clear_hash_large(jl_device_t*, jl_uint8);
static jl_ret_t jl61xx_acl_clear_hash_small(jl_device_t*, jl_uint8);
static jl_ret_t jl61xx_acl_split_get_list(jl61xx_acl_split_val_t*, jl61xx_acl_split_data_t*);
static jl_ret_t jl61xx_acl_split_add_data(jl_uint32, jl_uint32, jl61xx_acl_split_data_t*);
static void jl61xx_acl_split_clear_data(jl61xx_acl_split_data_t*);
static void jl61xx_acl_split_get_data_width(jl_uint32, jl_uint8 *);


static jl_ret_t jl61xx_acl_templ_info_set(jl_device_t *dev,
										jl_uint8 index,
										jl_acl_template_t *acl_templ)
{
	jl_uint8 field_idx = 0;
	jl_uint8 field_type = 0;
	jl_uint8 with_pointer = 0;

	JL_CHECK_POINTER(dev);
	JL_CHECK_POINTER(acl_templ);

	gp_acl_template[dev->id]->templ_mask[index] = 0;

	for (; field_idx < ACL_RULE_FIELD_NUM; field_idx++)
	{
		gp_acl_template[dev->id]->acl_templ[index][field_idx] = ACL_FIELD_NO_FIELD;

		field_type = acl_templ->field[field_idx].field_type;
		gp_acl_template[dev->id]->acl_templ[index][field_idx] = field_type;

		if (field_type != ACL_FIELD_NO_FIELD)
		{
			gp_acl_template[dev->id]->templ_mask[index] |= BIT(field_idx);
		}
		if (field_type == ACL_FIELD_RULE_POINTER)
			with_pointer = 1;
	}

	if (with_pointer)
		SET_BIT(gp_acl_template[dev->id]->rule_pointer_mask, index);
	else
		CLR_BIT(gp_acl_template[dev->id]->rule_pointer_mask, index);

	return JL_ERR_OK;
}


jl_ret_t jl61xx_acl_template_set(jl_device_t *dev,
							jl_uint8 index,
							jl_acl_template_t *acl_templ)
{
	jl_ret_t ret = JL_ERR_OK;
	jl_uint8 idx = 0;
	jl_uint8 field_idx = 0;
	jl_uint8 field_type = 0;
	jl_uint8 field_size = 0;
	jl_uint8 size_all = 0;

	jl_acl_field_t acl_field;
	jl_acl_template_t acl_template;
	SWCORE_INGRESS_CONFIGURABLE_ACL_RULES_SETUP_t field_rule;

	JL_CHECK_POINTER(dev);
	JL_CHECK_POINTER(acl_templ);

	memset((void*)&acl_template,0,sizeof(jl_acl_template_t));
	memset((void*)&field_rule,0,sizeof(SWCORE_INGRESS_CONFIGURABLE_ACL_RULES_SETUP_t));

	for (field_idx = 0; field_idx < ACL_RULE_FIELD_NUM; field_idx++)
	{
		field_type = acl_templ->field[field_idx].field_type;

		if (field_type >= ACL_FIELD_NUM)
			return JL_ERR_PARAM;

		field_size = jl61xx_acl_field_bit_size[field_type];

		acl_templ->field[field_idx].field_size = field_size;
		size_all += field_size;

		if (field_type != ACL_FIELD_NO_FIELD)
		{
			acl_template.field[idx] = acl_templ->field[field_idx];

			if (field_type == ACL_FIELD_RULE_POINTER && idx != 0)   //RULE_POINTER must be the first
			{
				acl_field = acl_template.field[0];
				acl_template.field[0] = acl_template.field[idx];
				acl_template.field[idx] = acl_field;
			}
			idx++;
		}
	}

	if (size_all > 202)
		return JL_ERR_CONFIG;

	for (field_idx = 0; field_idx < ACL_RULE_FIELD_NUM; field_idx++)
	{
		field_type = acl_template.field[field_idx].field_type;
		field_size = acl_template.field[field_idx].field_size;

		field_rule.BF.field_select = field_type &
			BITS_WIDTH(SWCORE_INGRESS_CONFIGURABLE_ACL_RULES_SETUP_OFFSET_FIELD_SELECT_WIDTH);
		field_rule.BF.bits_selected = field_size &
			BITS_WIDTH(SWCORE_INGRESS_CONFIGURABLE_ACL_RULES_SETUP_OFFSET_BITS_SELECTED_WIDTH);

		idx = (index * ACL_RULE_FIELD_NUM + field_idx);

		REGISTER_WRITE(dev, SWCORE, INGRESS_CONFIGURABLE_ACL_RULES_SETUP, field_rule, idx, 0);
	}

	ret = jl61xx_acl_templ_info_set(dev, index, &acl_template);

	return ret;
}


static jl_ret_t jl61xx_acl_templ_field_get(jl_device_t *dev,
						jl_uint8 index,
						jl_acl_field_t *acl_filed)

{
	jl_ret_t ret = JL_ERR_OK;

	JL_CHECK_POINTER(dev);
	JL_CHECK_POINTER(acl_filed);

	REGISTER_READ(dev, SWCORE, INGRESS_CONFIGURABLE_ACL_RULES_SETUP, field_rule, index, 0);

	acl_filed->field_type = field_rule.BF.field_select;
	acl_filed->field_size = field_rule.BF.bits_selected;

	return ret;
}

jl_ret_t jl61xx_acl_template_get(jl_device_t *dev,
				jl_uint8 index,
				jl_acl_template_t *acl_templ)
{
	jl_ret_t ret = JL_ERR_OK;
	jl_uint8 idx = 0;
	jl_uint8 field_idx = 0;
	jl_acl_field_t acl_filed;

	JL_CHECK_POINTER(dev);
	JL_CHECK_POINTER(acl_templ);

	for (idx = 0; idx < ACL_RULE_FIELD_NUM; idx++)
	{
		field_idx = (index * ACL_RULE_FIELD_NUM + idx);

		ret = jl61xx_acl_templ_field_get(dev, field_idx, &acl_filed);
		if (ret)
			return ret;

		acl_templ->field[idx] = acl_filed;
	}

	return JL_ERR_OK;
}

static jl_uint8 jl61xx_acl_calc_hash_idx(jl61xx_acl_hash_key_t* key, jl_uint8 is_large)
{
	jl_uint8 i,j;
	jl_uint8 hash_val = 0;
	jl_uint32 value = 0;

	JL_CHECK_POINTER(key);

	if (is_large)
	{
		//calc 4bit hash
		hash_val = key->value[0] & 0xf;
		hash_val ^= (key->value[0] >> 4);
		hash_val &= 0xf;

		for (i = 1; i < 26; i++)
		{
			hash_val ^= key->value[i];
			hash_val &= 0xf;
			hash_val ^= (key->value[i] >> 4);
			hash_val &= 0xf;
		}
	}
	else
	{
		//calc 3bit hash
		value = *(jl_uint32*)&key->value[0];
		value &= 0xffffff;
		hash_val = value & 0x7;

		for (i = 1; i < 8; i++)
		{
			value >>= 3;
			hash_val ^= value;
			hash_val &= 0x7;
		}

		for (i = 1; i < 8; i++)
		{
			value = *(jl_uint32*)&key->value[3*i];
			value &= 0xffffff;

			for (j = 0; j < 8; j++)
			{
				hash_val ^= value;
				hash_val &= 0x7;
				value >>= 3;
			}
		}

		value = *(jl_uint16*)&key->value[24];
		value &= 0xffff;

		for (j = 0; j < 6; j++)
		{
			hash_val ^= value;
			hash_val &= 0x7;
			value >>= 3;
		}
	}

	return hash_val;
}

static jl_ret_t jl61xx_acl_add_key_to_tcam(jl61xx_acl_hash_key_t* key , jl_uint8* tcam_tbl)
{
	jl_uint8 idx = 0;

	JL_CHECK_POINTER(key);
	JL_CHECK_POINTER(tcam_tbl);

	for (; idx < 26; idx++)
	{
		key->value[idx] &= key->mask[idx];

		jl_write_bits(tcam_tbl, key->mask[idx], (8 * idx) + 1, 8);
		jl_write_bits(tcam_tbl + 26, key->value[idx], (8 * idx) + 1, 8);  //begin from 209bit
	}

	return JL_ERR_OK;
}

static jl_ret_t jl61xx_acl_add_key_to_hash(jl61xx_acl_hash_key_t* key, jl_uint8* hash_tbl)
{
	jl_uint32 idx = 0;

	JL_CHECK_POINTER(key);
	JL_CHECK_POINTER(hash_tbl);

	for (; idx < 26; idx++)
	{
		jl_write_bits(hash_tbl, key->value[idx], (8 * idx) + 1, 8);
	}

	return JL_ERR_OK;
}

static jl_ret_t jl61xx_acl_add_action_to_tcam(jl_device_t *dev,
										jl_acl_action_t *acl_action,
										SWCORE_CONFIGURABLE_ACL_TCAM_ANSWER_t* tcam_tbl)
{
	jl_port_t cport;

	JL_CHECK_POINTER(dev);
	JL_CHECK_POINTER(acl_action);
	JL_CHECK_POINTER(tcam_tbl);

	tcam_tbl->BF.drop_enable = acl_action->drop_enable;
	tcam_tbl->BF.send_to_cpu = acl_action->send2cpu_enable;

	tcam_tbl->BF.send_to_port = acl_action->send2port_enable;
	if (tcam_tbl->BF.send_to_port)
	{
		cport = jlxx_port_l2c(dev, acl_action->dest_port);
		tcam_tbl->BF.dest_port = cport;
	}

	tcam_tbl->BF.force_vid_valid = acl_action->force_vid_enable;
	if (tcam_tbl->BF.force_vid_valid)
		tcam_tbl->BF.force_vid = acl_action->force_vid;

	tcam_tbl->BF.force_queue = acl_action->force_queue_enable;
	if (tcam_tbl->BF.force_queue)
		tcam_tbl->BF.e_queue = acl_action->e_queue;

	tcam_tbl->BF.force_color = acl_action->force_color_enable;
	if (tcam_tbl->BF.force_color)
		tcam_tbl->BF.color = acl_action->color;

	tcam_tbl->BF.mmp_valid = acl_action->mmp_valid;
	if (tcam_tbl->BF.mmp_valid)
	{
		tcam_tbl->BF.mmp_ptr = acl_action->mmp_ptr;
		tcam_tbl->BF.mmp_order = acl_action->mmp_order;
	}

	tcam_tbl->BF.update_counter = acl_action->update_counter_enable;
	if (tcam_tbl->BF.update_counter)
		tcam_tbl->BF.counter = acl_action->counter;

	tcam_tbl->BF.no_learning = acl_action->no_learning_enable;

	tcam_tbl->BF.update_cfi_dei = acl_action->update_cfi_dei_enable;
	if (tcam_tbl->BF.update_cfi_dei)
		tcam_tbl->BF.new_cfi_dei_value = acl_action->new_cfi_dei;

	tcam_tbl->BF.update_pcp = acl_action->update_pcp_enable;
	if (tcam_tbl->BF.update_pcp)
		tcam_tbl->BF.new_pcp_value = acl_action->new_pcp;

	tcam_tbl->BF.update_vid = acl_action->update_vid_enable;
	if (tcam_tbl->BF.update_vid)
		tcam_tbl->BF.new_vid_value = acl_action->new_vid;

	tcam_tbl->BF.update_etype = acl_action->update_etype_enable;
	if (tcam_tbl->BF.update_etype)
		tcam_tbl->BF.new_eth_type = acl_action->new_etype;

	tcam_tbl->BF.input_mirror = acl_action->input_mirror_enable;
	if (tcam_tbl->BF.input_mirror)
	{
		cport = jlxx_port_l2c(dev, acl_action->dest_input_mirror);
		tcam_tbl->BF.dest_input_mirror = cport;
	}


	return JL_ERR_OK;
}

static jl_ret_t jl61xx_acl_add_action_to_hash_large(jl_device_t *dev,
											jl_acl_action_t *acl_action,
											SWCORE_CONFIGURABLE_ACL_LARGE_TABLE_t* hash_tbl)
{
	jl_port_t cport;

	JL_CHECK_POINTER(dev);
	JL_CHECK_POINTER(acl_action);
	JL_CHECK_POINTER(hash_tbl);

	hash_tbl->BF.drop_enable = acl_action->drop_enable;
	hash_tbl->BF.send_to_cpu = acl_action->send2cpu_enable;

	hash_tbl->BF.send_to_port = acl_action->send2port_enable;
	if (hash_tbl->BF.send_to_port)
	{
		cport = jlxx_port_l2c(dev, acl_action->dest_port);
		hash_tbl->BF.dest_port = cport;
	}

	hash_tbl->BF.force_vid_valid = acl_action->force_vid_enable;
	if (hash_tbl->BF.force_vid_valid)
	{
		hash_tbl->BF.force_vid_0_6 = acl_action->force_vid;
		hash_tbl->BF.force_vid_7_11 = (acl_action->force_vid >> SWCORE_CONFIGURABLE_ACL_LARGE_TABLE_OFFSET_FORCE_VID_0_6_WIDTH);
	}

	hash_tbl->BF.force_queue = acl_action->force_queue_enable;
	if (hash_tbl->BF.force_queue)
		hash_tbl->BF.e_queue = acl_action->e_queue;

	hash_tbl->BF.force_color = acl_action->force_color_enable;
	if (hash_tbl->BF.force_color)
		hash_tbl->BF.color = acl_action->color;

	hash_tbl->BF.mmp_valid = acl_action->mmp_valid;
	if (hash_tbl->BF.mmp_valid)
	{
		hash_tbl->BF.mmp_ptr = acl_action->mmp_ptr;
		hash_tbl->BF.mmp_order = acl_action->mmp_order;
	}

	hash_tbl->BF.update_counter = acl_action->update_counter_enable;
	if (hash_tbl->BF.update_counter)
		hash_tbl->BF.counter = acl_action->counter;

	hash_tbl->BF.no_learning = acl_action->no_learning_enable;

	hash_tbl->BF.update_cfi_dei = acl_action->update_cfi_dei_enable;
	if (hash_tbl->BF.update_cfi_dei)
		hash_tbl->BF.new_cfi_dei_value = acl_action->new_cfi_dei;

	hash_tbl->BF.update_pcp = acl_action->update_pcp_enable;
	if (hash_tbl->BF.update_pcp)
		hash_tbl->BF.new_pcp_value = acl_action->new_pcp;

	hash_tbl->BF.update_vid = acl_action->update_vid_enable;
	if (hash_tbl->BF.update_vid)
		hash_tbl->BF.new_vid_value = acl_action->new_vid;

	hash_tbl->BF.update_etype = acl_action->update_etype_enable;
	if (hash_tbl->BF.update_etype)
		hash_tbl->BF.new_eth_type = acl_action->new_etype;

	hash_tbl->BF.update_etype = acl_action->update_etype_enable;
	if (hash_tbl->BF.update_etype)
		hash_tbl->BF.new_eth_type = acl_action->new_etype;

	hash_tbl->BF.input_mirror = acl_action->input_mirror_enable;
	if (hash_tbl->BF.input_mirror)
	{
		cport = jlxx_port_l2c(dev, acl_action->dest_input_mirror);
		hash_tbl->BF.dest_input_mirror = cport;
	}

	return JL_ERR_OK;
}

static jl_ret_t jl61xx_acl_add_action_to_hash_small(jl_device_t *dev,
												jl_acl_action_t *acl_action,
												SWCORE_CONFIGURABLE_ACL_SMALL_TABLE_t* hash_tbl)
{
	jl_port_t cport;

	JL_CHECK_POINTER(dev);
	JL_CHECK_POINTER(acl_action);
	JL_CHECK_POINTER(hash_tbl);

	hash_tbl->BF.drop_enable = acl_action->drop_enable;
	hash_tbl->BF.send_to_cpu = acl_action->send2cpu_enable;

	hash_tbl->BF.send_to_port = acl_action->send2port_enable;
	if (hash_tbl->BF.send_to_port)
	{
		cport = jlxx_port_l2c(dev, acl_action->dest_port);
		hash_tbl->BF.dest_port = cport;
	}

	hash_tbl->BF.force_vid_valid = acl_action->force_vid_enable;
	if (hash_tbl->BF.force_vid_valid)
	{
		hash_tbl->BF.force_vid_0_6 = acl_action->force_vid;
		hash_tbl->BF.force_vid_7_11 = (acl_action->force_vid >> SWCORE_CONFIGURABLE_ACL_SMALL_TABLE_OFFSET_FORCE_VID_0_6_WIDTH);
	}

	hash_tbl->BF.force_queue = acl_action->force_queue_enable;
	if (hash_tbl->BF.force_queue)
		hash_tbl->BF.e_queue = acl_action->e_queue;

	hash_tbl->BF.force_color = acl_action->force_color_enable;
	if (hash_tbl->BF.force_color)
		hash_tbl->BF.color = acl_action->color;

	hash_tbl->BF.mmp_valid = acl_action->mmp_valid;
	if (hash_tbl->BF.mmp_valid)
	{
		hash_tbl->BF.mmp_ptr = acl_action->mmp_ptr;
		hash_tbl->BF.mmp_order = acl_action->mmp_order;
	}

	hash_tbl->BF.update_counter = acl_action->update_counter_enable;
	if (hash_tbl->BF.update_counter)
		hash_tbl->BF.counter = acl_action->counter;

	hash_tbl->BF.no_learning = acl_action->no_learning_enable;

	hash_tbl->BF.update_cfi_dei = acl_action->update_cfi_dei_enable;
	if (hash_tbl->BF.update_cfi_dei)
		hash_tbl->BF.new_cfi_dei_value = acl_action->new_cfi_dei;

	hash_tbl->BF.update_pcp = acl_action->update_pcp_enable;
	if (hash_tbl->BF.update_pcp)
		hash_tbl->BF.new_pcp_value = acl_action->new_pcp;

	hash_tbl->BF.update_vid = acl_action->update_vid_enable;
	if (hash_tbl->BF.new_vid_value)
		hash_tbl->BF.new_vid_value = acl_action->new_vid;

	hash_tbl->BF.update_etype = acl_action->update_etype_enable;
	if (hash_tbl->BF.update_etype)
		hash_tbl->BF.new_eth_type = acl_action->new_etype;

	hash_tbl->BF.input_mirror = acl_action->input_mirror_enable;
	if (hash_tbl->BF.input_mirror)
	{
		cport = jlxx_port_l2c(dev, acl_action->dest_input_mirror);
		hash_tbl->BF.dest_input_mirror = cport;
	}

	return JL_ERR_OK;
}


static void jl61xx_acl_value_range_chk(jl_device_t *dev,
										jl_uint32 value, 
										jl_uint32 mask, 
										jl_acl_field_data_t* field_rule)
{
	jl_uint32 range_s = 0;
	jl_uint32 range_e = 0;
	jl_uint32 full_mask;
	jl_uint8 width;
	jl_uint8 idx;

	width = jl61xx_acl_field_bit_size[field_rule->field_type];
	full_mask = BITS_WIDTH(width);

	range_s = value;
	range_e = value;

	if (mask != full_mask)
	{
		/*get range max*/
		for (idx = 0; idx < width; idx++)
		{
			if (GET_BIT(mask, idx) == 0)
				SET_BIT(range_e, idx);
		}
	}

	if(field_rule->field_type == ACL_FIELD_SOURCE_PORT)
	{
		if (range_s <= 1 && range_e > 1)
		{
			range_s = 0;
			range_e = EXT_PORT1;
		}
		else
		{
			range_s =  jl_port_c2l_get(dev->id, range_s);
			range_e =  jl_port_c2l_get(dev->id, range_e);
		}
	}

	if (field_rule->field.val_data.type == ACL_DATA_RANGE_VALUE)
	{
		field_rule->field.val_data.range_s = (range_s < field_rule->field.val_data.range_s) ? 
														range_s : field_rule->field.val_data.range_s;
		field_rule->field.val_data.range_e = (range_e > field_rule->field.val_data.range_e) ? 
														range_e : field_rule->field.val_data.range_e;
	}
	else
	{
		if (field_rule->field.val_data.mask == 0) //first entry
		{
			if (range_s != range_e)
			{
				field_rule->field.val_data.type = ACL_DATA_RANGE_VALUE;
				field_rule->field.val_data.range_s = range_s;
				field_rule->field.val_data.range_e = range_e;
				field_rule->field.val_data.value = 0;
				field_rule->field.val_data.mask = 0;
			}
			else
			{
				field_rule->field.val_data.type = ACL_DATA_SPECIFIC_VALUE;
				field_rule->field.val_data.value = range_s;
				field_rule->field.val_data.mask = mask;
			}
		}
		else //follow entry
		{
			value = field_rule->field.val_data.value; //pre value

			if (range_s != range_e || range_s != value)
			{
				field_rule->field.val_data.type = ACL_DATA_RANGE_VALUE;
				field_rule->field.val_data.range_s = (range_s < value) ? range_s : value;
				field_rule->field.val_data.range_e = (range_e > value) ? range_e : value;
				field_rule->field.val_data.value = 0;
				field_rule->field.val_data.mask = 0;
			}
		}
	}
}

static jl_ret_t jl61xx_acl_value_range_split(jl_acl_field_data_t* pfield,
											jl61xx_acl_data_t *pdata)
{
	jl_ret_t ret = JL_ERR_OK;
	jl_uint8 is_new = 0;
	jl61xx_acl_split_val_t val = {0};
	jl61xx_acl_split_data_t* psplit = NULL;
	jl61xx_acl_split_data_t* psplit_temp = pdata->split_data;
	jl61xx_acl_split_data_t* psplit_end = NULL;

	while (psplit_temp) {
		if (psplit_temp->entry_type == pfield->field_type) {
			psplit = psplit_temp;
			break;
		}
		psplit_end = psplit_temp;
		psplit_temp = psplit_temp->next;
	}
	
	/*new field data*/
	if (psplit == NULL) {
		psplit = (jl61xx_acl_split_data_t*)port_mem_malloc(sizeof(jl61xx_acl_split_data_t));
		if (NULL == psplit)
			return JL_ERR_MEMORY;

		memset(psplit, 0, sizeof(jl61xx_acl_split_data_t));
		is_new = 1;
	}
	
	psplit->entry_type = pfield->field_type;
	psplit->templ_idx = pdata->templ_idx;
	psplit->reg_offset = pfield->reg_offset;

	if (pfield->field.val_data.type == ACL_DATA_SPECIFIC_VALUE) {
		ret = jl61xx_acl_split_add_data(pfield->field.val_data.value, pfield->field.val_data.mask, psplit);
	}
	else {
		val.val_l = pfield->field.val_data.range_s;
		val.val_h = pfield->field.val_data.range_e;
		val.mask = BITS_WIDTH(jl61xx_acl_field_bit_size[pfield->field_type]);

		jl61xx_acl_split_get_data_width(val.val_h, &(val.width_h));
		jl61xx_acl_split_get_data_width(val.val_l, &(val.width_l));

		ret = jl61xx_acl_split_get_list(&val, psplit);
	}
	
	if (is_new) {
		if (psplit_end == NULL)
			pdata->split_data = psplit;
		else
			psplit_end->next = psplit;
	}

	return ret;
}

static jl_ret_t jl61xx_acl_port_range_split(jl_device_t *dev, 
											jl_acl_field_data_t* pfield,
											jl61xx_acl_data_t *pdata)
{
	jl_ret_t ret = JL_ERR_OK;
	jl_acl_field_data_t t_field;
	jl_uint32 cport_s;
	jl_uint32 cport_e;
	jl_uint8 with_ext = 0;

	if (pfield->field_type != ACL_FIELD_SOURCE_PORT)
		return JL_ERR_PARAM;

	t_field = *pfield;

	if (pfield->field.source_port.type == ACL_DATA_SPECIFIC_VALUE)
	{
		/*'source port' with specific value is regarded as full masked*/		
		t_field.field.source_port.mask = BITS_WIDTH(jl61xx_acl_field_bit_size[t_field.field_type]);
		/*convert to chip port index*/
		t_field.field.source_port.value = jlxx_port_l2c(dev, pfield->field.source_port.value);
		ret = jl61xx_acl_value_range_split(&t_field, pdata);
		return ret;
	}
	else
	{	
		cport_s = jlxx_port_l2c(dev, pfield->field.source_port.range_s);
		cport_e = jlxx_port_l2c(dev, pfield->field.source_port.range_e);

		if (cport_s == cport_e)
		{
			t_field.field.source_port.range_s = cport_s;
			t_field.field.source_port.range_e = cport_e;
			ret = jl61xx_acl_value_range_split(&t_field, pdata);
			return ret;
		}
		if (((struct jl_switch_dev_s *)(dev->switch_dev))->ext_portmask)
			with_ext = 1;
		
		if (pfield->field.source_port.range_e == CPU_PORT0)
		{
			if (pfield->field.source_port.range_s >= EXT_PORT0)
			{
				/*example chip port 0-1 + 10*/
				t_field.field.source_port.range_s = 10;
				t_field.field.source_port.range_e = 10;

				ret = jl61xx_acl_value_range_split(&t_field, pdata);
				if (ret)
					return ret;

				t_field.field.source_port.range_s = cport_s;
				t_field.field.source_port.range_e = 1;
				
				ret = jl61xx_acl_value_range_split(&t_field, pdata);

				return ret;
			}
			else
			{	
				if (cport_s == 2)
				{	
					/*example chip port 0 - 10*/
					if (with_ext)
						t_field.field.source_port.range_s = 0;
					else
						t_field.field.source_port.range_s = 2;
					t_field.field.source_port.range_e = 10;

					ret = jl61xx_acl_value_range_split(&t_field, pdata);
					return ret;					
				}
				else
				{
					/*example chip port 0-1 + 3-10*/
					if (with_ext)
					{
						t_field.field.source_port.range_s = 0;
						t_field.field.source_port.range_e = 1;

						ret = jl61xx_acl_value_range_split(&t_field, pdata);
						if (ret)
							return ret;
					}
					
					t_field.field.source_port.range_s = cport_s;
					t_field.field.source_port.range_e = 10;

					ret = jl61xx_acl_value_range_split(&t_field, pdata);
					return ret;
				}
			}
		}
		else
		{
			if (cport_s <= cport_e)
			{
				/*example chip port 0-1 or 2-9*/
				t_field.field.source_port.range_s = cport_s;
				t_field.field.source_port.range_e = cport_e;

				ret = jl61xx_acl_value_range_split(&t_field, pdata);
				return ret;
			}
			else
			{
				if (cport_s == 2 && cport_e == 1)
				{
					t_field.field.source_port.range_s = 0;
					t_field.field.source_port.range_e = 9;

					ret = jl61xx_acl_value_range_split(&t_field, pdata);
					return ret;
				}
				else
				{
					t_field.field.source_port.range_s = 0;
					t_field.field.source_port.range_e = cport_e;

					ret = jl61xx_acl_value_range_split(&t_field, pdata);
					if (ret)
						return ret;

					t_field.field.source_port.range_s = cport_s;
					t_field.field.source_port.range_e = 9;
					ret = jl61xx_acl_value_range_split(&t_field, pdata);
					return ret;
				}
			}
		}
	}

	return JL_ERR_OK;
}

static jl_ret_t jl61xx_acl_insert_val_to_rule(jl_device_t *dev, jl_uint8 field_type,
												jl61xx_acl_val128_t* val, jl_acl_rule_t *acl)
{
	jl_ret_t ret;
	jl_uint8 is_new = 0;

	jl_acl_field_data_t* field_rule = NULL;
	jl_acl_field_data_t* field_tmp = NULL;

	JL_CHECK_POINTER(dev);
	JL_CHECK_POINTER(val);
	JL_CHECK_POINTER(acl);

	field_tmp = acl->head;

	while (field_tmp) {
		if (field_tmp->field_type == field_type) {
			field_rule = field_tmp; /*already exists*/
			break;
		}
		field_tmp = field_tmp->next;
	}

	/*new field*/
	if (field_rule == NULL) {
		field_rule = (jl_acl_field_data_t*)port_mem_malloc(sizeof(jl_acl_field_data_t));
		if(field_rule == NULL) {
			ret = JL_ERR_MEMORY;
			goto err;
		}
		memset(field_rule, 0, sizeof(jl_acl_field_data_t));
		is_new = 1;
	}

	field_rule->field_type = field_type;

	switch (field_type)
	{
		case ACL_FIELD_MAC_DA:
		case ACL_FIELD_MAC_SA:
			ret = jl61xx_acl_v2l(val, 48,(void*)&field_rule->field.mac_data);
			if (ret)
				goto err;
			break;
		case ACL_FIELD_SOURCE_PORT:
		case ACL_FIELD_OUTER_VID:
		case ACL_FIELD_OUTER_PCP:
		case ACL_FIELD_OUTER_DEI:
		case ACL_FIELD_INNER_VID:
		case ACL_FIELD_INNER_PCP:
		case ACL_FIELD_INNER_DEI:
		case ACL_FIELD_ETH_TYPE:
		case ACL_FIELD_MPLS:
		case ACL_FIELD_TOS:
		case ACL_FIELD_TTL:
		case ACL_FIELD_L4_PROTOCOL:
		case ACL_FIELD_L4_SOURCE_PORT:
		case ACL_FIELD_L4_DEST_PORT:
		case ACL_FIELD_ICMP_TYPE:
		case ACL_FIELD_ICMP_CODE:
		case ACL_FIELD_ICMP6_TYPE:
		case ACL_FIELD_ICMP6_CODE:
		case ACL_FIELD_IGMP_TYPE:
			jl61xx_acl_value_range_chk(dev, val->val_0_63, val->mask_0_63, field_rule);
			break;
		case ACL_FIELD_HAS_VLAN:
			field_rule->field.has_vlans.value = val->val_0_63;
			break;
		case ACL_FIELD_OUTER_ETH_TYPE:
			field_rule->field.outer_cstag.value = val->val_0_63;
			break;
		case ACL_FIELD_INNER_ETH_TYPE:
			field_rule->field.inner_cstag.value = val->val_0_63;
			break;
		case ACL_FIELD_L3_TYPE:
			field_rule->field.l3_type.value = val->val_0_63;
			break;
		case ACL_FIELD_IP_SA:
		case ACL_FIELD_IP_DA:
		case ACL_FIELD_IGMP_GROUP_ADDR:
			ret = jl61xx_acl_v2l(val, 32,(void*)&field_rule->field.ip_data);
			if (ret)
				goto err;
			break;
		case ACL_FIELD_IP6_SA:
		case ACL_FIELD_IP6_DA:
		case ACL_FIELD_MLD_ADDR:
			ret = jl61xx_acl_v2l(val, 128,(void*)&field_rule->field.ip6_data);
			if (ret)
				goto err;
			break;
		case ACL_FIELD_L4_TYPE:
			field_rule->field.l4_type.value = val->val_0_63;
			break;
		case ACL_FIELD_RULE_POINTER:
			field_rule->field.rule_pointer.value = val->val_0_63;
			acl->with_pointer = 1;
			break;
		default:
			ret = JL_ERR_PARAM;
			goto err;
	}

	if (is_new) {
		if (acl->head == NULL)
			acl->head = field_rule;
		else
		{
			field_tmp = acl->head;

			while(field_tmp->next)
				field_tmp = field_tmp->next;

			field_tmp->next = field_rule;
		}
	}
	
	return JL_ERR_OK;

err:
	if (field_rule)
		port_mem_free(field_rule);

	return ret;
}


jl_ret_t jl61xx_acl_field_insert(jl_device_t *dev,
								jl_acl_field_data_t *acl_field,
								jl_acl_rule_t *acl_rule)
{
	jl_acl_field_data_t* field_buf = NULL;
	jl_acl_field_data_t* field_pre = NULL;
	jl_acl_field_data_t* field_tmp = NULL;
	jl_acl_field_data_t* field_nxt = NULL;
	jl_api_ret_t ret = JL_ERR_OK;

	JL_CHECK_POINTER(dev);
	JL_CHECK_POINTER(acl_rule);
	JL_CHECK_POINTER(acl_field);

	ret = jl61xx_acl_field_chk(dev, acl_rule, acl_field);
	if (ret)
		return ret;

	field_buf = (jl_acl_field_data_t*)port_mem_malloc(sizeof(jl_acl_field_data_t));

	if (!field_buf)
		return JL_ERR_MEMORY;

	*field_buf = *acl_field;

	if (acl_rule->head == NULL)
		acl_rule->head = field_buf;
	else
	{
		field_tmp = acl_rule->head;
		while (field_tmp != NULL)
		{
			if (field_tmp->field_type == acl_field->field_type)
			{
				field_nxt = field_tmp->next;
				port_mem_free(field_tmp);
				field_tmp = NULL;
				acl_rule->field_num--;
				break;
			}
			field_pre = field_tmp;
			field_tmp = field_tmp->next;
		}

		if (acl_rule->field_num == ACL_RULE_FIELD_NUM)
		{
			if (field_buf)
				port_mem_free(field_buf);
			return JL_ERR_FULL;
		}

		if (field_pre == NULL)
			acl_rule->head = field_buf;
		else
		{
			field_buf->next = field_nxt;
			field_pre->next = field_buf;
		}
	}

	acl_rule->field_mask |= BIT(acl_field->field_idx);
	acl_rule->field_num++;

	if (acl_field->field_type == ACL_FIELD_RULE_POINTER)
		acl_rule->with_pointer = 1;

	return JL_ERR_OK;
}

static jl_ret_t jl61xx_acl_field_chk(jl_device_t *dev, jl_acl_rule_t *acl_rule, jl_acl_field_data_t *acl_field)
{
	jl_uint8 field_idx = 0;
	jl_uint8 field_type = 0;
	jl_uint8 templ_type = 0;
	jl_uint8 bit_width = 0;
	jl_uint8 bit_offset = 0;
	jl_uint8 idx = 0;
	jl_uint32 mask_val = 0;

	JL_CHECK_POINTER(dev);
	JL_CHECK_POINTER(acl_rule);
	JL_CHECK_POINTER(acl_field);

	field_type = acl_field->field_type;
	templ_type = acl_rule->templ_idx;
	acl_field->tcam_flag = 0;

	if (field_type > ACL_FIELD_RULE_POINTER || field_type == ACL_FIELD_NO_FIELD)
		return JL_ERR_PARAM;

	for (field_idx = 0; field_idx < ACL_RULE_FIELD_NUM; field_idx++)
	{
		bit_width = jl61xx_acl_field_bit_size[gp_acl_template[dev->id]->acl_templ[templ_type][field_idx]];

		if (field_type == gp_acl_template[dev->id]->acl_templ[templ_type][field_idx])
		{
			acl_field->field_idx = field_idx;
			acl_field->reg_offset = bit_offset;

			if (bit_width <= 32)
				mask_val = BITS_WIDTH(bit_width);

			switch (field_type)
			{
				case ACL_FIELD_SOURCE_PORT:
					if (acl_field->field.source_port.type == ACL_DATA_SPECIFIC_VALUE) {
						JL_CHECK_PORT(dev, acl_field->field.source_port.value);
					}
					else if (acl_field->field.source_port.type == ACL_DATA_RANGE_VALUE) {
						if (acl_field->field.source_port.range_s > acl_field->field.source_port.range_e)
							return JL_ERR_PARAM;
						JL_CHECK_PORT(dev, acl_field->field.source_port.range_s);
						JL_CHECK_PORT(dev, acl_field->field.source_port.range_e);
					}
					else
						return JL_ERR_PARAM;
					break;
				case ACL_FIELD_OUTER_VID:
				case ACL_FIELD_OUTER_PCP:
				case ACL_FIELD_OUTER_DEI:
				case ACL_FIELD_INNER_VID:
				case ACL_FIELD_INNER_PCP:
				case ACL_FIELD_INNER_DEI:
				case ACL_FIELD_ETH_TYPE:
				case ACL_FIELD_MPLS:
				case ACL_FIELD_TOS:
				case ACL_FIELD_TTL:
				case ACL_FIELD_L4_PROTOCOL:
				case ACL_FIELD_L4_SOURCE_PORT:
				case ACL_FIELD_L4_DEST_PORT:
				case ACL_FIELD_ICMP_TYPE:
				case ACL_FIELD_ICMP_CODE:
				case ACL_FIELD_ICMP6_TYPE:
				case ACL_FIELD_ICMP6_CODE:
				case ACL_FIELD_IGMP_TYPE:
					if (acl_field->field.val_data.type == ACL_DATA_SPECIFIC_VALUE) {
						if (acl_field->field.val_data.value > mask_val)
							return JL_ERR_PARAM;
					}
					else if (acl_field->field.val_data.type == ACL_DATA_RANGE_VALUE) {
						if ((acl_field->field.val_data.range_s > mask_val) ||
							(acl_field->field.val_data.range_e > mask_val) ||
							(acl_field->field.val_data.range_s > acl_field->field.val_data.range_e))
							return JL_ERR_PARAM;
					}
					else
						return JL_ERR_PARAM;
					break;
				case ACL_FIELD_MAC_DA:
				case ACL_FIELD_MAC_SA:
					if (acl_field->field.mac_data.value.val > 0xffffffffffff)
						return JL_ERR_PARAM;
					if (acl_field->field.mac_data.mask.val != 0xffffffffffff)
						acl_field->tcam_flag = 1;
					break;
				case ACL_FIELD_HAS_VLAN:
					if (acl_field->field.has_vlans.value > mask_val)
						return JL_ERR_PARAM;
					break;
				case ACL_FIELD_OUTER_ETH_TYPE:
					if (acl_field->field.outer_cstag.value > mask_val)
						return JL_ERR_PARAM;
					break;
				case ACL_FIELD_INNER_ETH_TYPE:
					if (acl_field->field.inner_cstag.value > mask_val)
						return JL_ERR_PARAM;
					break;
				case ACL_FIELD_L3_TYPE:
					if (acl_field->field.l3_type.value > mask_val)
						return JL_ERR_PARAM;
					break;
				case ACL_FIELD_L4_TYPE:
					if (acl_field->field.l4_type.value > mask_val)
						return JL_ERR_PARAM;
					break;
				case ACL_FIELD_IP_SA:
				case ACL_FIELD_IP_DA:
				case ACL_FIELD_IGMP_GROUP_ADDR:
					for (idx = 0; idx < 4; idx++) {
						if (acl_field->field.ip_data.mask.addr[idx] != 0xff) {
							acl_field->tcam_flag = 1;
							break;
						}
					}
					break;	
				case ACL_FIELD_IP6_SA:
				case ACL_FIELD_IP6_DA:
				case ACL_FIELD_MLD_ADDR:
					for (idx = 0; idx < 8; idx++) {
						if (acl_field->field.ip6_data.mask.addr[idx] != 0xffff) {
							acl_field->tcam_flag = 1;
							break;
						}
					}
				case ACL_FIELD_RULE_POINTER:
					break;
				default:
						return JL_ERR_PARAM;
			}
			return JL_ERR_OK;
		}
		bit_offset += bit_width;
	}

	return JL_ERR_CONFIG;
}

jl_ret_t jl61xx_acl_field_clear(jl_device_t *dev, jl_acl_rule_t *acl_rule)
{
	jl_acl_field_data_t* field_tmp = NULL;
	jl_acl_field_data_t* field_nxt = NULL;

	JL_CHECK_POINTER(dev);
	JL_CHECK_POINTER(acl_rule);
	
	field_tmp = acl_rule->head;

	while (field_tmp != NULL)
	{
		field_nxt = field_tmp->next;
		port_mem_free(field_tmp);
		field_tmp = field_nxt;
	}

	memset(acl_rule, 0, sizeof(jl_acl_rule_t));
	return JL_ERR_OK;
}

static jl_ret_t jl61xx_acl_get_rule_from_tcam(jl_device_t *dev,
											jl_uint8 index,
											jl_uint8 has_get,
											jl_acl_rule_t *acl_rule)
{
	jl_uint8 idx = 0;
	jl_uint8 templ_type = 0;
	jl_uint8 field_msk = 0;
	jl_uint8 field_type = 0;
	jl_uint8 field_size = 0;
	jl_uint8 field_offset = 7;
	jl_uint8 field_num = 0;

	jl_uint64 val = 0;
	jl_uint64 val_h = 0;

	jl61xx_acl_val128_t acl_val;
	jl_ret_t ret = JL_ERR_OK;

	JL_CHECK_POINTER(dev);
	JL_CHECK_POINTER(acl_rule);

	REGISTER_READ(dev, SWCORE, CONFIGURABLE_ACL_TCAM, tcam_tbl, index, 0);

	memset(&acl_val, 0 ,sizeof(jl61xx_acl_val128_t));

	templ_type = acl_rule->templ_idx;
	field_msk = acl_rule->field_mask;

	for (; idx < ACL_RULE_FIELD_NUM; idx++)
	{    
		if (field_num >= acl_rule->field_num)
			break;

		field_type = gp_acl_template[dev->id]->acl_templ[templ_type][idx];
		field_size = jl61xx_acl_field_bit_size[field_type];

		if (GET_BIT(field_msk,idx) == 1)
		{
			field_num++;

			if (field_type == ACL_FIELD_IP6_SA || 
				field_type == ACL_FIELD_IP6_DA || 
				field_type == ACL_FIELD_MLD_ADDR)
			{
				if (has_get)
				{
					field_offset += field_size;
					continue; //already get by front entry
				}

				jl_read_bits((jl_uint8*)tcam_tbl.val, &val, field_offset, 64);
				jl_read_bits((jl_uint8*)tcam_tbl.val, &val_h, field_offset + 64, 64);

				acl_val.mask_0_63 = val;
				acl_val.mask_64_127 = val_h;

				jl_read_bits((jl_uint8*)tcam_tbl.val, &val, field_offset + 208, 64);
				jl_read_bits(((jl_uint8*)tcam_tbl.val) + 26, &val_h, field_offset + 64, 64);

				acl_val.val_0_63 = val;
				acl_val.val_64_127 = val_h;
			}
			else
			{
				if (has_get && ((field_type == ACL_FIELD_MAC_DA) ||
								(field_type == ACL_FIELD_MAC_SA) ||
								(field_type == ACL_FIELD_IP_SA) ||
								(field_type == ACL_FIELD_IP_DA) ||
								(field_type == ACL_FIELD_IGMP_GROUP_ADDR) ||
								(field_type == ACL_FIELD_HAS_VLAN) ||
								(field_type == ACL_FIELD_OUTER_ETH_TYPE) ||
								(field_type == ACL_FIELD_INNER_ETH_TYPE) ||
								(field_type == ACL_FIELD_L3_TYPE) ||
								(field_type == ACL_FIELD_L4_TYPE) ||
								(field_type == ACL_FIELD_RULE_POINTER)))
				{
					field_offset += field_size;
					continue; //already get by front entry
				}

				jl_read_bits((jl_uint8*)tcam_tbl.val, &val, field_offset, field_size);
				acl_val.mask_0_63 = val;
				jl_read_bits(((jl_uint8*)tcam_tbl.val) + 26, &val, field_offset, field_size);
				acl_val.val_0_63 = val;
			}

			ret = jl61xx_acl_insert_val_to_rule(dev, field_type, &acl_val, acl_rule);
			if (ret)
				return ret;
		}
		field_offset += field_size;
	}

	return JL_ERR_OK;
}


static jl_ret_t jl61xx_acl_get_rule_from_hash(jl_device_t *dev,
												jl_uint8 large,
												jl_uint8 has_get,
												void* hash_tbl,
												jl_acl_rule_t *acl_rule)
{
	jl_ret_t ret = JL_ERR_OK;
	jl_uint8 idx = 0;
	jl_uint8 templ_type = 0;
	jl_uint8 field_msk = 0;
	jl_uint8 field_type = 0;
	jl_uint8 field_size = 0;
	jl_uint8 field_offset = 7;
	jl_uint8 field_num = 0;
	jl_uint64 val = 0;
	jl_uint64 val_h = 0;

	jl61xx_acl_val128_t acl_val;
	
	SWCORE_CONFIGURABLE_ACL_LARGE_TABLE_t* large_tbl = (SWCORE_CONFIGURABLE_ACL_LARGE_TABLE_t*)hash_tbl;
	SWCORE_CONFIGURABLE_ACL_SMALL_TABLE_t* small_tbl = (SWCORE_CONFIGURABLE_ACL_SMALL_TABLE_t*)hash_tbl;

	JL_CHECK_POINTER(dev);
	JL_CHECK_POINTER(hash_tbl);
	JL_CHECK_POINTER(acl_rule);

	memset(&acl_val, 0 ,sizeof(jl61xx_acl_val128_t));

	templ_type = acl_rule->templ_idx;
	field_msk = acl_rule->field_mask;

	for (; idx < ACL_RULE_FIELD_NUM; idx++)
	{
		if (field_num >= acl_rule->field_num)
			break;

		field_type = gp_acl_template[dev->id]->acl_templ[templ_type][idx];
		field_size = jl61xx_acl_field_bit_size[field_type];
		if (GET_BIT(field_msk,idx) == 1)
		{
			field_num++;

			if (field_type == ACL_FIELD_IP6_SA || 
				field_type == ACL_FIELD_IP6_DA || 
				field_type == ACL_FIELD_MLD_ADDR)
			{
				if (has_get)
				{
					field_offset += field_size;
					continue; //already get by front entry
				}

				acl_val.mask_0_63 = 0xffffffffffffffff;
				acl_val.mask_64_127 = 0xffffffffffffffff;

				if (large)
				{
					jl_read_bits((jl_uint8*)large_tbl->val, &val, field_offset, 64);
					jl_read_bits((jl_uint8*)large_tbl->val, &val_h, field_offset + 64, 64);
				}
				else
				{
					jl_read_bits((jl_uint8*)small_tbl->val, &val, field_offset, 64);
					jl_read_bits((jl_uint8*)small_tbl->val, &val_h, field_offset + 64, 64);
				}

				acl_val.val_0_63 = val;
				acl_val.val_64_127 = val_h;
			}
			else
			{
				if (has_get && ((field_type == ACL_FIELD_MAC_DA) ||
								(field_type == ACL_FIELD_MAC_SA) ||
								(field_type == ACL_FIELD_IP_SA) ||
								(field_type == ACL_FIELD_IP_DA) ||
								(field_type == ACL_FIELD_IGMP_GROUP_ADDR) ||
								(field_type == ACL_FIELD_HAS_VLAN) ||
								(field_type == ACL_FIELD_OUTER_ETH_TYPE) ||
								(field_type == ACL_FIELD_INNER_ETH_TYPE) ||
								(field_type == ACL_FIELD_L3_TYPE) ||
								(field_type == ACL_FIELD_L4_TYPE) ||
								(field_type == ACL_FIELD_RULE_POINTER)))
				{
					field_offset += field_size;
					continue; //already get by front entry
				}

				if (field_type == ACL_FIELD_MAC_DA || field_type == ACL_FIELD_MAC_SA)
					acl_val.mask_0_63 = 0xffffffffffff;
				else
					acl_val.mask_0_63 = BITS_WIDTH(field_size);

				if (large)
					jl_read_bits((jl_uint8*)large_tbl->val, &val, field_offset, field_size);
				else
					jl_read_bits((jl_uint8*)small_tbl->val, &val, field_offset, field_size);

				acl_val.val_0_63 = val;
			}

			ret = jl61xx_acl_insert_val_to_rule(dev, field_type, &acl_val, acl_rule);
			if (ret)
				return ret;
		}
		field_offset += field_size;
	}

	return JL_ERR_OK;
}

static jl_ret_t jl61xx_acl_get_action_from_tcam(jl_device_t *dev,
											jl_uint8 index,
											jl_uint8 has_get,
											jl_acl_action_t *acl_action)
{
	jl_ret_t ret = JL_ERR_OK;
	jl_port_t lport;

	JL_CHECK_POINTER(dev);
	JL_CHECK_POINTER(acl_action);

	if (has_get)
		return JL_ERR_OK;

	REGISTER_READ(dev, SWCORE, CONFIGURABLE_ACL_TCAM_ANSWER, tcam_tbl, index, 0);

	acl_action->drop_enable = tcam_tbl.BF.drop_enable;
	acl_action->send2cpu_enable = tcam_tbl.BF.send_to_cpu;
	acl_action->send2port_enable = tcam_tbl.BF.send_to_port;
	if (acl_action->send2port_enable)
	{
		lport = jlxx_port_c2l(dev, tcam_tbl.BF.dest_port);
		acl_action->dest_port = lport;
	}

	acl_action->force_vid_enable = tcam_tbl.BF.force_vid_valid;
	acl_action->force_vid = tcam_tbl.BF.force_vid;
	acl_action->force_queue_enable = tcam_tbl.BF.force_queue;
	acl_action->e_queue = tcam_tbl.BF.e_queue;
	acl_action->force_color_enable = tcam_tbl.BF.force_color;
	acl_action->color = tcam_tbl.BF.color;
	acl_action->mmp_valid = tcam_tbl.BF.mmp_valid;
	acl_action->mmp_ptr = tcam_tbl.BF.mmp_ptr;
	acl_action->mmp_order = tcam_tbl.BF.mmp_order;
	acl_action->update_counter_enable = tcam_tbl.BF.update_counter;
	acl_action->counter = tcam_tbl.BF.counter;
	acl_action->no_learning_enable = tcam_tbl.BF.no_learning;
	acl_action->update_cfi_dei_enable = tcam_tbl.BF.update_cfi_dei;
	acl_action->new_cfi_dei = tcam_tbl.BF.new_cfi_dei_value;
	acl_action->update_pcp_enable = tcam_tbl.BF.update_pcp;
	acl_action->new_pcp = tcam_tbl.BF.new_pcp_value;
	acl_action->update_vid_enable = tcam_tbl.BF.update_vid;
	acl_action->new_vid = tcam_tbl.BF.new_vid_value;
	acl_action->update_etype_enable = tcam_tbl.BF.update_etype;
	acl_action->new_etype = tcam_tbl.BF.new_eth_type;
	acl_action->input_mirror_enable = tcam_tbl.BF.input_mirror;
	if (acl_action->input_mirror_enable)
	{
		lport = jlxx_port_c2l(dev, tcam_tbl.BF.dest_input_mirror);
		acl_action->dest_input_mirror = lport;
	}
	return JL_ERR_OK;
}

static jl_ret_t jl61xx_acl_get_action_from_hash(jl_device_t *dev,
												jl_uint8 large,
												jl_uint8 has_get,
												void *hash_tbl,
												jl_acl_action_t *acl_action)
{
	jl_port_t lport;

	SWCORE_CONFIGURABLE_ACL_LARGE_TABLE_t* large_tbl = (SWCORE_CONFIGURABLE_ACL_LARGE_TABLE_t*)hash_tbl;
	SWCORE_CONFIGURABLE_ACL_SMALL_TABLE_t* small_tbl = (SWCORE_CONFIGURABLE_ACL_SMALL_TABLE_t*)hash_tbl;

	JL_CHECK_POINTER(dev);
	JL_CHECK_POINTER(acl_action);
	JL_CHECK_POINTER(hash_tbl);

	if (has_get)
		return JL_ERR_OK;

	if (large)
	{
		acl_action->drop_enable = large_tbl->BF.drop_enable;
		acl_action->send2cpu_enable = large_tbl->BF.send_to_cpu;
		acl_action->send2port_enable = large_tbl->BF.send_to_port;
		if (acl_action->send2port_enable)
		{
			lport = jlxx_port_c2l(dev, large_tbl->BF.dest_port);
			acl_action->dest_port = lport;
		}

		acl_action->force_vid_enable = large_tbl->BF.force_vid_valid;
		acl_action->force_vid = large_tbl->BF.force_vid_0_6 +
			((jl_uint32)large_tbl->BF.force_vid_7_11 << SWCORE_CONFIGURABLE_ACL_LARGE_TABLE_OFFSET_FORCE_VID_0_6_WIDTH);
		acl_action->force_queue_enable = large_tbl->BF.force_queue;
		acl_action->e_queue = large_tbl->BF.e_queue;
		acl_action->force_color_enable = large_tbl->BF.force_color;
		acl_action->color = large_tbl->BF.color;
		acl_action->mmp_valid = large_tbl->BF.mmp_valid;
		acl_action->mmp_ptr = large_tbl->BF.mmp_ptr;
		acl_action->mmp_order = large_tbl->BF.mmp_order;
		acl_action->update_counter_enable = large_tbl->BF.update_counter;
		acl_action->counter = large_tbl->BF.counter;
		acl_action->no_learning_enable = large_tbl->BF.no_learning;
		acl_action->update_cfi_dei_enable = large_tbl->BF.update_cfi_dei;
		acl_action->new_cfi_dei = large_tbl->BF.new_cfi_dei_value;
		acl_action->update_pcp_enable = large_tbl->BF.update_pcp;
		acl_action->new_pcp = large_tbl->BF.new_pcp_value;
		acl_action->update_vid_enable = large_tbl->BF.update_vid;
		acl_action->new_vid = large_tbl->BF.new_vid_value;
		acl_action->update_etype_enable = large_tbl->BF.update_etype;
		acl_action->new_etype = large_tbl->BF.new_eth_type;
		acl_action->input_mirror_enable = large_tbl->BF.input_mirror;
		if (acl_action->input_mirror_enable)
		{
			lport = jlxx_port_c2l(dev, large_tbl->BF.dest_input_mirror);
			acl_action->dest_input_mirror = lport;
		}
	}
	else
	{
		acl_action->drop_enable = small_tbl->BF.drop_enable;
		acl_action->send2cpu_enable = small_tbl->BF.send_to_cpu;
		acl_action->send2port_enable = small_tbl->BF.send_to_port;
		if (acl_action->send2port_enable)
		{
			lport = jlxx_port_c2l(dev, small_tbl->BF.dest_port);
			acl_action->dest_port = lport;
		}

		acl_action->force_vid_enable = small_tbl->BF.force_vid_valid;
		acl_action->force_vid = small_tbl->BF.force_vid_0_6 +
			((jl_uint32)small_tbl->BF.force_vid_7_11 << SWCORE_CONFIGURABLE_ACL_SMALL_TABLE_OFFSET_FORCE_VID_0_6_WIDTH);
		acl_action->force_queue_enable = small_tbl->BF.force_queue;
		acl_action->e_queue = small_tbl->BF.e_queue;
		acl_action->force_color_enable = small_tbl->BF.force_color;
		acl_action->color = small_tbl->BF.color;
		acl_action->mmp_valid = small_tbl->BF.mmp_valid;
		acl_action->mmp_ptr = small_tbl->BF.mmp_ptr;
		acl_action->mmp_order = small_tbl->BF.mmp_order;
		acl_action->update_counter_enable = small_tbl->BF.update_counter;
		acl_action->counter = small_tbl->BF.counter;
		acl_action->no_learning_enable = small_tbl->BF.no_learning;
		acl_action->update_cfi_dei_enable = small_tbl->BF.update_cfi_dei;
		acl_action->new_cfi_dei = small_tbl->BF.new_cfi_dei_value;
		acl_action->update_pcp_enable = small_tbl->BF.update_pcp;
		acl_action->new_pcp = small_tbl->BF.new_pcp_value;
		acl_action->update_vid_enable = small_tbl->BF.update_vid;
		acl_action->new_vid = small_tbl->BF.new_vid_value;
		acl_action->update_etype_enable = small_tbl->BF.update_etype;
		acl_action->new_etype = small_tbl->BF.new_eth_type;
		acl_action->input_mirror_enable = small_tbl->BF.input_mirror;
		if (acl_action->input_mirror_enable)
		{
			lport = jlxx_port_c2l(dev, small_tbl->BF.dest_input_mirror);
			acl_action->dest_input_mirror = lport;
		}
	}

	return JL_ERR_OK;
}

static jl_ret_t jl61xx_acl_info_counter_add(jl_device_t *dev, jl_uint8 index, jl_acl_action_t *acl_action)
{
	gp_acl_rule_info[dev->id]->acl_rule[index].counter_valid = acl_action->update_counter_enable;
	if(acl_action->update_counter_enable)
	{
		gp_acl_rule_info[dev->id]->acl_rule[index].counter_index = acl_action->counter;
		gp_acl_rule_info[dev->id]->counter[acl_action->counter]++;
	}
	return JL_ERR_OK;
}

static jl_ret_t jl61xx_acl_info_set(jl_device_t *dev,
										jl_uint8 index,
										jl61xx_acl_data_t* acl_data,
										jl_acl_action_t *acl_action)
{
	jl_ret_t ret;
	JL_CHECK_POINTER(dev);
	JL_CHECK_POINTER(acl_data);
	JL_CHECK_POINTER(acl_action);

	gp_acl_rule_info[dev->id]->acl_rule[index].valid = TRUE;
	gp_acl_rule_info[dev->id]->acl_rule[index].field_num = acl_data->field_num;
	gp_acl_rule_info[dev->id]->acl_rule[index].field_mask = acl_data->field_mask;
	gp_acl_rule_info[dev->id]->acl_rule[index].rule_templ = acl_data->templ_idx;
	gp_acl_rule_info[dev->id]->acl_rule[index].rule_entry = acl_data->rule_entry;

	ret = jl61xx_acl_info_counter_add(dev, index, acl_action);
	if (ret)
		return ret;
	
	if (acl_action->mmp_valid)
	{
		gp_acl_rule_info[dev->id]->acl_rule[index].mmp_index = acl_action->mmp_ptr;

		ret = jl61xx_set_mmp_pointer_info(dev, JL_MMP_MODULE_ACL, acl_action->mmp_ptr);
		if (ret)
			return ret;
	}
	else if (acl_action->mmp_valid == FALSE && gp_acl_rule_info[dev->id]->acl_rule[index].mmp_valid)
	{
		ret = jl61xx_clear_mmp_pointer_info(dev, JL_MMP_MODULE_ACL,
									gp_acl_rule_info[dev->id]->acl_rule[index].mmp_index);
	}
	gp_acl_rule_info[dev->id]->acl_rule[index].mmp_valid = acl_action->mmp_valid;

	return JL_ERR_OK;
}

static jl_ret_t jl61xx_acl_info_counter_clear(jl_device_t *dev, jl_uint8 index)
{
	jl_ret_t ret = JL_ERR_OK;
	jl_uint8 counter = 0;

	counter = gp_acl_rule_info[dev->id]->acl_rule[index].counter_index;

	if (gp_acl_rule_info[dev->id]->acl_rule[index].counter_valid)
	{
		if (--gp_acl_rule_info[dev->id]->counter[counter] == 0)
		{
			/*no rule use this counter,clear it*/
			ret = jl61xx_acl_stat_write(dev, counter, 0);
			if (ret)
				return ret;
		}
	}

	gp_acl_rule_info[dev->id]->acl_rule[index].counter_valid = 0;
	gp_acl_rule_info[dev->id]->acl_rule[index].counter_index = 0;

	return JL_ERR_OK;
}

static jl_ret_t jl61xx_acl_rule_info_clear(jl_device_t *dev, jl_uint8 index)
{
	jl_ret_t ret = JL_ERR_OK;
	jl61xx_acl_rule_entry_t *rule_entry;
	jl61xx_acl_rule_entry_t *rule_temp;

	JL_CHECK_POINTER(dev);

	if (gp_acl_rule_info[dev->id]->acl_rule[index].valid == TRUE)
	{
		rule_entry = gp_acl_rule_info[dev->id]->acl_rule[index].rule_entry;
		while (rule_entry) {
			if (rule_entry->table == ACL_TBL_TCAM) {
				CLR_BIT(gp_acl_rule_info[dev->id]->entry_mask_tcam, rule_entry->index);
				gp_acl_rule_info[dev->id]->used_tcam_num--;
			}
			else if (rule_entry->table == ACL_TBL_LARGE) {
				CLR_BIT(gp_acl_rule_info[dev->id]->entry_mask_large, rule_entry->index);
				gp_acl_rule_info[dev->id]->used_large_num--;
			}
			else {
				CLR_BIT(gp_acl_rule_info[dev->id]->entry_mask_small, rule_entry->index);
				gp_acl_rule_info[dev->id]->used_small_num--;
			}
			
			rule_temp = rule_entry;
			rule_entry = rule_entry->next;
			port_mem_free(rule_temp);
		}
		
		ret = jl61xx_acl_info_counter_clear(dev, index);
		if (ret)
			return ret;

		if (gp_acl_rule_info[dev->id]->acl_rule[index].mmp_valid)
		{
			ret = jl61xx_clear_mmp_pointer_info(dev, JL_MMP_MODULE_ACL,
									gp_acl_rule_info[dev->id]->acl_rule[index].mmp_index);
			if (ret)
				return ret;
		}

		memset(&gp_acl_rule_info[dev->id]->acl_rule[index], 0, sizeof(jl61xx_acl_entry_info_t));
	}

	return 	JL_ERR_OK;
}


static jl_ret_t jl61xx_acl_info_get(jl_device_t *dev,
										jl_uint8 index,
										jl61xx_acl_data_t *acl_data,
										jl_acl_action_t *acl_action)
{
	JL_CHECK_POINTER(dev);
	JL_CHECK_POINTER(acl_data);

	if (gp_acl_rule_info[dev->id]->acl_rule[index].valid == FALSE)
		return JL_ERR_NOT_FOUND;

	memset(acl_data, 0, sizeof(jl61xx_acl_data_t));
	
	acl_data->templ_idx = gp_acl_rule_info[dev->id]->acl_rule[index].rule_templ;
	acl_data->field_mask = gp_acl_rule_info[dev->id]->acl_rule[index].field_mask;
	acl_data->field_num = gp_acl_rule_info[dev->id]->acl_rule[index].field_num;
	acl_data->rule_entry = gp_acl_rule_info[dev->id]->acl_rule[index].rule_entry;

	if (acl_action) {
		acl_action->update_counter_enable = gp_acl_rule_info[dev->id]->acl_rule[index].counter_valid;
		acl_action->counter = gp_acl_rule_info[dev->id]->acl_rule[index].counter_index;
		acl_action->mmp_valid = gp_acl_rule_info[dev->id]->acl_rule[index].mmp_valid;
		acl_action->mmp_ptr = gp_acl_rule_info[dev->id]->acl_rule[index].mmp_index;
	}
	
	return JL_ERR_OK;
}

static jl_ret_t jl61xx_acl_action_chk(jl_device_t *dev, jl_acl_action_t *acl_action)
{
	JL_CHECK_POINTER(dev);
	JL_CHECK_POINTER(acl_action);

	if (acl_action->drop_enable > 1)
		return JL_ERR_PARAM;

	if (acl_action->send2port_enable > 1)
		return JL_ERR_PARAM;
	
	if (acl_action->send2port_enable) {
		JL_CHECK_PORT(dev, acl_action->dest_port);
	}

	if (acl_action->send2cpu_enable > 1)
		return JL_ERR_PARAM;

	if (acl_action->force_vid_enable > 1)
		return JL_ERR_PARAM;

	if (acl_action->force_vid_enable && (acl_action->force_vid > 0xfff))
		return JL_ERR_PARAM;

	if (acl_action->force_queue_enable > 1)
		return JL_ERR_PARAM;

	if (acl_action->force_queue_enable && (acl_action->e_queue > 7))
		return JL_ERR_PARAM;

	if (acl_action->force_color_enable > 1)
		return JL_ERR_PARAM;

	if (acl_action->force_color_enable && (acl_action->color > 2))
		return JL_ERR_PARAM;

	if (acl_action->mmp_valid > 1)
		return JL_ERR_PARAM;

	if (acl_action->mmp_valid &&
			(acl_action->mmp_ptr > 15 || acl_action->mmp_order > 3))
		return JL_ERR_PARAM;

	if (acl_action->update_counter_enable > 1)
		return JL_ERR_PARAM;

	if (acl_action->update_counter_enable && (acl_action->counter > 31))
		return JL_ERR_PARAM;

	if (acl_action->no_learning_enable > 1)
		return JL_ERR_PARAM;

	if (acl_action->update_cfi_dei_enable > 1)
		return JL_ERR_PARAM;

	if (acl_action->update_cfi_dei_enable  && (acl_action->new_cfi_dei > 1))
		return JL_ERR_PARAM;

	if (acl_action->update_pcp_enable > 1)
		return JL_ERR_PARAM;

	if (acl_action->update_pcp_enable && (acl_action->new_pcp > 7))
		return JL_ERR_PARAM;

	if (acl_action->update_vid_enable > 1)
		return JL_ERR_PARAM;

	if (acl_action->update_vid_enable && (acl_action->new_vid > 0xfff))
		return JL_ERR_PARAM;

	if (acl_action->update_etype_enable > 1)
		return JL_ERR_PARAM;

	if (acl_action->update_etype_enable && (acl_action->new_etype > 3))
		return JL_ERR_PARAM;

	if (acl_action->input_mirror_enable > 1)
		return JL_ERR_PARAM;

	if (acl_action->input_mirror_enable)
	{
		JL_CHECK_PORT(dev, acl_action->dest_input_mirror);
	}

	return JL_ERR_OK;
}

static jl_ret_t jl61xx_acl_space_chk(jl_device_t *dev, jl_uint8 index, jl61xx_acl_data_t *pdata)
{
	jl_uint8 rule_num = 1;
	jl_uint8 hash_num = 1;
	jl_uint8 tcam_num = 0;
	jl_uint8 old_hash_num = 0;
	jl_uint8 old_tcam_num = 0;
	jl61xx_acl_rule_entry_t *rule_entry;

	JL_CHECK_POINTER(dev);
	JL_CHECK_POINTER(pdata);

	jl61xx_acl_split_data_t *psplit = pdata->split_data;
	
	/*get needed number*/
	while (psplit)
	{
		rule_num *= (psplit->hash_entry_num + psplit->tcam_entry_num);
		hash_num *= psplit->hash_entry_num;
		psplit = psplit->next;
	}

	if (pdata->tcam_flag == 1) {
		tcam_num = rule_num;
		hash_num = 0;
	}
	else
		tcam_num = rule_num - hash_num;

	/*get old entry number*/
	if (gp_acl_rule_info[dev->id]->acl_rule[index].valid) {
		rule_entry = gp_acl_rule_info[dev->id]->acl_rule[index].rule_entry;

		while (rule_entry)
		{
			if (rule_entry->table == ACL_TBL_TCAM)
				old_tcam_num++;
			else 
				old_hash_num++;

			rule_entry = rule_entry->next;
		}
	}
	
	if (tcam_num > (JL61XX_ACL_TCAM_NUM - 
					gp_acl_rule_info[dev->id]->used_tcam_num + 
					old_tcam_num))
		return JL_ERR_RESOURCE;

	tcam_num = JL61XX_ACL_TCAM_NUM - gp_acl_rule_info[dev->id]->used_tcam_num + old_tcam_num - tcam_num;

	/*try best*/
	if (hash_num > ((JL61XX_ACL_LARGE_NUM + JL61XX_ACL_SMALL_NUM + tcam_num) - 
					(gp_acl_rule_info[dev->id]->used_large_num + gp_acl_rule_info[dev->id]->used_small_num) + 
					 old_hash_num))
		return JL_ERR_RESOURCE;

	return JL_ERR_OK;
}

static jl_ret_t jl61xx_acl_pre_process(jl_device_t *dev, 
											jl_acl_rule_t *prule,
											jl61xx_acl_data_t *pdata)
{
	jl_ret_t ret = JL_ERR_OK;
	jl_acl_field_data_t* pfield = NULL;

	JL_CHECK_POINTER(dev);
	JL_CHECK_POINTER(prule);
	JL_CHECK_POINTER(pdata);

	/*if lack of rule pointer data*/
	if (CHECK_BIT(gp_acl_template[dev->id]->rule_pointer_mask, prule->templ_idx) &&
															(prule->with_pointer == 0))
	{
		pfield = (jl_acl_field_data_t*)port_mem_malloc(sizeof(jl_acl_field_data_t));
		if (!pfield)
			return JL_ERR_MEMORY;
		
		memset(pfield, 0, sizeof(jl_acl_field_data_t));

		pfield->field_type = ACL_FIELD_RULE_POINTER;
		pfield->field_idx = 0;
		pfield->reg_offset = 0;
		pfield->field.rule_pointer.value = prule->templ_idx;
		pfield->next = prule->head;
		prule->head = pfield;
		prule->with_pointer = 1;
		prule->field_num++;
		SET_BIT(prule->field_mask, 0);
	}
	/*split all rules those with range data type*/
	pdata->templ_idx = prule->templ_idx;
	pdata->field_num = prule->field_num;
	pdata->field_mask = prule->field_mask;
	pdata->origin_data = prule->head;

	pfield = prule->head;

	while (pfield)
	{
		switch (pfield->field_type)
		{
			case ACL_FIELD_MAC_DA:
			case ACL_FIELD_MAC_SA:
			case ACL_FIELD_IP_SA:
			case ACL_FIELD_IP_DA:
			case ACL_FIELD_IGMP_GROUP_ADDR:
			case ACL_FIELD_IP6_SA:
			case ACL_FIELD_IP6_DA:
			case ACL_FIELD_MLD_ADDR:
			case ACL_FIELD_HAS_VLAN:
			case ACL_FIELD_OUTER_ETH_TYPE:
			case ACL_FIELD_INNER_ETH_TYPE:
			case ACL_FIELD_L3_TYPE:
			case ACL_FIELD_L4_TYPE:
			case ACL_FIELD_RULE_POINTER:
				pdata->tcam_flag |= pfield->tcam_flag;
				break;
			case ACL_FIELD_SOURCE_PORT:
				ret = jl61xx_acl_port_range_split(dev, pfield, pdata);
				if (ret)
					return ret;
				break;
			default: //jl_acl_val_t type
				ret = jl61xx_acl_value_range_split(pfield, pdata);
				if (ret)
					return ret;
				break;
		}
		pfield = pfield->next;
	}
	/*use tcam table when this rule is not full field typed*/
	if (prule->field_mask != gp_acl_template[dev->id]->templ_mask[prule->templ_idx])
		pdata->tcam_flag = 1;

	return JL_ERR_OK;
}

static void jl61xx_acl_after_process(jl61xx_acl_data_t *pdata)
{
	jl61xx_acl_split_data_t* psplit = NULL;
	jl61xx_acl_split_data_t* psplit_temp = NULL;

	psplit = pdata->split_data;
	while (psplit)
	{
		psplit_temp = psplit->next;
		jl61xx_acl_split_clear_data(psplit);
		port_mem_free(psplit);
		psplit = psplit_temp;
	}
	pdata->split_data = NULL;
	return;
}

static jl_ret_t jl61xx_acl_write_tcam(jl_device_t *dev,
					jl61xx_acl_hash_key_t *key,
					jl_acl_action_t *acl_action,
					jl61xx_acl_rule_entry_t **ppentry)
{
	jl_ret_t ret = JL_ERR_OK;
	jl_uint32 tcam_mask = 0;
	jl61xx_acl_rule_entry_t *entry = NULL;
	jl_uint8 idx = 0;
	jl_uint8 tcam_valid = FALSE;

	SWCORE_CONFIGURABLE_ACL_TCAM_t	tcam_table;
	SWCORE_CONFIGURABLE_ACL_TCAM_ANSWER_t answer_table;

	JL_CHECK_POINTER(dev);
	JL_CHECK_POINTER(key);
	JL_CHECK_POINTER(acl_action);
	JL_CHECK_POINTER(ppentry);

	memset((void*)&tcam_table, 0, sizeof(SWCORE_CONFIGURABLE_ACL_TCAM_t));
	memset((void*)&answer_table, 0, sizeof(SWCORE_CONFIGURABLE_ACL_TCAM_ANSWER_t));

	tcam_mask = gp_acl_rule_info[dev->id]->entry_mask_tcam;

	for (idx = 0; idx < 32; idx++)
	{
		if (!(tcam_mask & 0x1))
		{
			tcam_valid = TRUE;
			break;
		}
		tcam_mask >>= 1;
	}

	if (tcam_valid == FALSE)
		return JL_ERR_RESOURCE;

	ret = jl61xx_acl_add_key_to_tcam(key, (jl_uint8*)&tcam_table);  //pack rule data
	if (ret)
		return ret;

	ret = jl61xx_acl_add_action_to_tcam(dev, acl_action, &answer_table);  //pack action data
	if (ret)
		return ret;

	tcam_table.BF.valid = 1;

	REGISTER_WRITE(dev, SWCORE, CONFIGURABLE_ACL_TCAM_ANSWER, answer_table, idx, 0);
	REGISTER_WRITE(dev, SWCORE, CONFIGURABLE_ACL_TCAM, tcam_table, idx, 0);

	entry = (jl61xx_acl_rule_entry_t*)port_mem_malloc(sizeof(jl61xx_acl_rule_entry_t));
	if (entry == NULL)
		return JL_ERR_MEMORY;

	SET_BIT(gp_acl_rule_info[dev->id]->entry_mask_tcam, idx);
	gp_acl_rule_info[dev->id]->used_tcam_num++;

	entry->table = ACL_TBL_TCAM;
	entry->index = idx;
	entry->next = NULL;

	if ((*ppentry) == NULL) {
		entry->end = entry;
		*ppentry = entry;
	}
	else {
		(*ppentry)->end->next = entry;
		(*ppentry)->end = entry;
	}

	return JL_ERR_OK;
}


static jl_ret_t jl61xx_acl_write_hash_large(jl_device_t	 *dev,
						jl61xx_acl_hash_key_t *key,
						jl_acl_action_t *acl_action,
						jl61xx_acl_rule_entry_t **ppentry)
{
	jl_ret_t	ret = JL_ERR_OK;
	jl_uint8	hash_idx = 0;
	jl_uint8	key_idx = 0;
	jl_uint8	buckets_idx = 0;
	jl_bool		hash_valid = FALSE;
	jl_uint64	hash_mask = 0;
	jl61xx_acl_rule_entry_t *entry = NULL;

	SWCORE_CONFIGURABLE_ACL_LARGE_TABLE_t hash_table;

	JL_CHECK_POINTER(dev);
	JL_CHECK_POINTER(key);
	JL_CHECK_POINTER(acl_action);
	JL_CHECK_POINTER(ppentry);

	hash_mask = gp_acl_rule_info[dev->id]->entry_mask_large;

	key_idx = jl61xx_acl_calc_hash_idx(key, 1);

	for (buckets_idx = 0; buckets_idx < 4; buckets_idx++)
	{
		hash_idx = (buckets_idx << 4) + key_idx;

		if (GET_BIT(hash_mask,hash_idx) == 0)
		{
			hash_valid = TRUE;
			break;
		}
	}

	if (hash_valid == FALSE)
		return JL_ERR_RESOURCE;

	memset((void*)&hash_table, 0, sizeof(SWCORE_CONFIGURABLE_ACL_LARGE_TABLE_t));

	ret = jl61xx_acl_add_key_to_hash(key, (jl_uint8*)&hash_table);  //pack rule data
	if (ret)
		return ret;

	ret = jl61xx_acl_add_action_to_hash_large(dev, acl_action, &hash_table);  //pack action data
	if (ret)
		return ret;

	hash_table.BF.valid = 1;

	REGISTER_WRITE(dev, SWCORE, CONFIGURABLE_ACL_LARGE_TABLE, hash_table, hash_idx, 0);

	entry = (jl61xx_acl_rule_entry_t*)port_mem_malloc(sizeof(jl61xx_acl_rule_entry_t));
	if (entry == NULL)
		return JL_ERR_MEMORY;

	SET_BIT(gp_acl_rule_info[dev->id]->entry_mask_large, hash_idx);
	gp_acl_rule_info[dev->id]->used_large_num++;

	entry->table = ACL_TBL_LARGE;
	entry->index = hash_idx;
	entry->next = NULL;

	if ((*ppentry) == NULL) {
		entry->end = entry;
		*ppentry = entry;
	}
	else {
		(*ppentry)->end->next = entry;
		(*ppentry)->end = entry;
	}

	return JL_ERR_OK;
}

static jl_ret_t jl61xx_acl_write_hash_small(jl_device_t *dev,
					jl61xx_acl_hash_key_t *key,
					jl_acl_action_t *acl_action,
					jl61xx_acl_rule_entry_t **ppentry)
{
	jl_ret_t	ret = JL_ERR_OK;
	jl_uint8	hash_idx = 0;
	jl_uint8	key_idx = 0;
	jl_uint8	buckets_idx = 0;
	jl_bool		hash_valid = FALSE;
	jl_uint32	hash_mask = 0;
	jl61xx_acl_rule_entry_t *entry = NULL;

	SWCORE_CONFIGURABLE_ACL_SMALL_TABLE_t hash_table;

	JL_CHECK_POINTER(dev);
	JL_CHECK_POINTER(key);
	JL_CHECK_POINTER(acl_action);
	JL_CHECK_POINTER(ppentry);

	hash_mask = gp_acl_rule_info[dev->id]->entry_mask_small;

	key_idx = jl61xx_acl_calc_hash_idx(key, 0);

	for (buckets_idx = 0; buckets_idx < 4; buckets_idx++)
	{
		hash_idx = (buckets_idx << 3) + key_idx;

		if (GET_BIT(hash_mask,hash_idx) == 0)
		{
			hash_valid = TRUE;
			break;
		}
	}

	if (hash_valid == FALSE)
		return JL_ERR_RESOURCE;

	memset((void*)&hash_table, 0, sizeof(SWCORE_CONFIGURABLE_ACL_SMALL_TABLE_t));

	ret = jl61xx_acl_add_key_to_hash(key, (jl_uint8*)&hash_table);  //pack rule data
	if (ret)
		return ret;

	ret = jl61xx_acl_add_action_to_hash_small(dev, acl_action, &hash_table);  //pack action data
	if (ret)
		return ret;

	hash_table.BF.valid = 1;

	REGISTER_WRITE(dev, SWCORE, CONFIGURABLE_ACL_SMALL_TABLE, hash_table, hash_idx, 0);

	entry = (jl61xx_acl_rule_entry_t*)port_mem_malloc(sizeof(jl61xx_acl_rule_entry_t));
	if (entry == NULL)
		return JL_ERR_MEMORY;

	SET_BIT(gp_acl_rule_info[dev->id]->entry_mask_small, hash_idx);
	gp_acl_rule_info[dev->id]->used_small_num++;

	entry->table = ACL_TBL_SMALL;
	entry->index = hash_idx;
	entry->next = NULL;

	if ((*ppentry) == NULL) {
		entry->end = entry;
		*ppentry = entry;
	}
	else {
		(*ppentry)->end->next = entry;
		(*ppentry)->end = entry;
	}

	return JL_ERR_OK;
}

static jl_ret_t jl61xx_acl_read_tcam(jl_device_t *dev,
					jl_uint8 index,
					jl_uint8 *pget,
					jl_acl_rule_t *rule_out,
					jl_acl_action_t *act_out)

{
	jl_ret_t ret = JL_ERR_OK;

	JL_CHECK_POINTER(dev);
	JL_CHECK_POINTER(pget);
	JL_CHECK_POINTER(rule_out);
	JL_CHECK_POINTER(act_out);

	ret = jl61xx_acl_get_rule_from_tcam(dev, index, *pget, rule_out);
	if (ret)
		return ret;

	ret = jl61xx_acl_get_action_from_tcam(dev, index, *pget, act_out);

	if (*pget == 0)
		*pget = 1;

	return ret;
}


static jl_ret_t jl61xx_acl_read_hash_large(jl_device_t *dev,
					jl_uint8 index,
					jl_uint8 *pget,
					jl_acl_rule_t *rule_out,
					jl_acl_action_t *act_out)
{
	jl_ret_t ret = JL_ERR_OK;

	JL_CHECK_POINTER(dev);
	JL_CHECK_POINTER(pget);
	JL_CHECK_POINTER(rule_out);
	JL_CHECK_POINTER(act_out);

	REGISTER_READ(dev, SWCORE, CONFIGURABLE_ACL_LARGE_TABLE, hash_tbl, index, 0);

	ret = jl61xx_acl_get_rule_from_hash(dev, 1, *pget, (void*)&hash_tbl, rule_out);
	if (ret)
		return ret;
	
	ret = jl61xx_acl_get_action_from_hash(dev, 1, *pget, (void*)&hash_tbl, act_out);

	if (*pget == 0)
		*pget = 1;
	
	return ret;
}

static jl_ret_t jl61xx_acl_read_hash_mall(jl_device_t *dev,
					jl_uint8 index,
					jl_uint8 *pget,
					jl_acl_rule_t *rule_out,
					jl_acl_action_t *act_out)
{
	jl_ret_t ret = JL_ERR_OK;

	JL_CHECK_POINTER(dev);
	JL_CHECK_POINTER(pget);
	JL_CHECK_POINTER(rule_out);
	JL_CHECK_POINTER(act_out);

	REGISTER_READ(dev, SWCORE, CONFIGURABLE_ACL_SMALL_TABLE, hash_tbl, index, 0);

	ret = jl61xx_acl_get_rule_from_hash(dev, 0, *pget, (void*)&hash_tbl, rule_out);
	if (ret)
		return ret;

	ret = jl61xx_acl_get_action_from_hash(dev, 0, *pget, (void*)&hash_tbl, act_out);

	if (*pget == 0)
		*pget = 1;
	
	return ret;
}

static jl_ret_t jl61xx_acl_clear_table(jl_device_t *dev, jl_uint8 index)
{
	jl_ret_t ret = JL_ERR_OK;
	jl61xx_acl_rule_entry_t *rule_entry;

	JL_CHECK_POINTER(dev);

	if (gp_acl_rule_info[dev->id]->acl_rule[index].valid == FALSE)
		return JL_ERR_OK;

	rule_entry = gp_acl_rule_info[dev->id]->acl_rule[index].rule_entry;

	while (rule_entry) {
		if (rule_entry->table == ACL_TBL_TCAM)
			ret = jl61xx_acl_clear_tcam(dev, rule_entry->index);
		else if (rule_entry->table == ACL_TBL_LARGE)
			ret = jl61xx_acl_clear_hash_large(dev, rule_entry->index);
		else
			ret = jl61xx_acl_clear_hash_small(dev, rule_entry->index);
		
		if (ret)
			return ret;
		rule_entry = rule_entry->next;
	}

	ret = jl61xx_acl_rule_info_clear(dev, index);

	return ret;
}

static jl_ret_t jl61xx_acl_clear_tcam(jl_device_t *dev, jl_uint8 index)
{
	jl_ret_t ret = JL_ERR_OK;

	JL_CHECK_POINTER(dev);

	SWCORE_CONFIGURABLE_ACL_TCAM_t	tcam_tbl;
	SWCORE_CONFIGURABLE_ACL_TCAM_ANSWER_t answer_tbl;

	memset((void*)&tcam_tbl, 0, sizeof(SWCORE_CONFIGURABLE_ACL_TCAM_t));
	memset((void*)&answer_tbl, 0, sizeof(SWCORE_CONFIGURABLE_ACL_TCAM_ANSWER_t));

	REGISTER_WRITE(dev, SWCORE, CONFIGURABLE_ACL_TCAM, tcam_tbl, index, 0);
	REGISTER_WRITE(dev, SWCORE, CONFIGURABLE_ACL_TCAM_ANSWER, answer_tbl, index, 0);

	return JL_ERR_OK;
}

static jl_ret_t jl61xx_acl_clear_hash_large(jl_device_t *dev, jl_uint8 index)
{
	jl_ret_t ret = JL_ERR_OK;

	JL_CHECK_POINTER(dev);

	SWCORE_CONFIGURABLE_ACL_LARGE_TABLE_t hash_tbl;

	memset((void*)&hash_tbl, 0, sizeof(SWCORE_CONFIGURABLE_ACL_LARGE_TABLE_t));

	REGISTER_WRITE(dev, SWCORE, CONFIGURABLE_ACL_LARGE_TABLE, hash_tbl, index, 0);

	return JL_ERR_OK;
}

static jl_ret_t jl61xx_acl_clear_hash_small(jl_device_t *dev, jl_uint8 index)
{
	jl_ret_t ret = JL_ERR_OK;

	JL_CHECK_POINTER(dev);

	SWCORE_CONFIGURABLE_ACL_SMALL_TABLE_t hash_tbl;

	memset((void*)&hash_tbl, 0, sizeof(SWCORE_CONFIGURABLE_ACL_SMALL_TABLE_t));

	REGISTER_WRITE(dev, SWCORE, CONFIGURABLE_ACL_SMALL_TABLE, hash_tbl, index, 0);

	return JL_ERR_OK;
}


static jl_ret_t jl61xx_acl_l2v(void *val_in, jl_uint8 len, jl61xx_acl_val128_t* val_out)
{
	jl_acl_mac_t *p_mac = NULL;
	jl_acl_ip_t *p_ip = NULL;
	jl_acl_ip6_t *p_ip6 = NULL;

	jl_uint8 i = 0;
	jl_uint64 val = 0;

	JL_CHECK_POINTER(val_in);
	JL_CHECK_POINTER(val_out);

	memset(val_out, 0, sizeof(jl61xx_acl_val128_t));

	if (len == 32) //ipv4
	{
		p_ip = (jl_acl_ip_t*)val_in;

		for (i = 0; i < 4; i++)
		{
			val = p_ip->value.addr[i];
			val <<= ((3 - i) * 8);
			val_out->val_0_63 |= val;
			val = p_ip->mask.addr[i];
			val <<= ((3 - i) * 8);
			val_out->mask_0_63 |= val;
		}
	}
	else if (len == 48) //mac
	{
		p_mac = (jl_acl_mac_t*)val_in;

		for (i = 0; i < 6; i++)
		{
			val = p_mac->value.addr[i];
			val <<= ((5 - i) * 8);
			val_out->val_0_63 |= val;
			val = p_mac->mask.addr[i];
			val <<= ((5 - i) * 8);
			val_out->mask_0_63 |= val;
		}
	}
	else if (len == 128) //ipv6
	{
		p_ip6 = (jl_acl_ip6_t*)val_in;

		val_out->val_0_63 = *(jl_uint64*)&p_ip6->value.addr[0];
		val_out->val_64_127 = *(jl_uint64*)&p_ip6->value.addr[4];
		val_out->mask_0_63 = *(jl_uint64*)&p_ip6->mask.addr[0];
		val_out->mask_64_127 = *(jl_uint64*)&p_ip6->mask.addr[4];
	}
	else
		return JL_ERR_PARAM;

	return JL_ERR_OK;
}

static jl_ret_t jl61xx_acl_v2l(jl61xx_acl_val128_t* val_in, jl_uint8 len, void *val_out)
{
	jl_acl_mac_t *p_mac = NULL;
	jl_acl_ip_t *p_ip = NULL;
	jl_acl_ip6_t *p_ip6 = NULL;

	jl_uint8 i = 0;
	jl_uint64 val = 0;

	JL_CHECK_POINTER(val_in);
	JL_CHECK_POINTER(val_out);

	if (len == 32) //ipv4
	{
		p_ip = (jl_acl_ip_t*)val_out;
		memset(p_ip, 0, sizeof(jl_acl_ip_t));

		for (i = 0; i < 4; i++)
		{
			val =  (val_in->val_0_63 >> ((3 - i) * 8));
			p_ip->value.addr[i] = val & 0xff;
			val =  (val_in->mask_0_63 >> ((3 - i) * 8));
			p_ip->mask.addr[i] = val & 0xff;
		}
	}
	else if (len == 48) //mac
	{
		p_mac = (jl_acl_mac_t*)val_out;
		memset(p_mac, 0, sizeof(jl_acl_mac_t));

		for (i = 0; i < 6; i++)
		{
			val = (val_in->val_0_63 >> ((5 - i) * 8));
			p_mac->value.addr[i] = val & 0xff;
			val =  (val_in->mask_0_63 >> ((5 - i) * 8));
			p_mac->mask.addr[i] = val & 0xff;
		}
	}
	else if (len == 128) //ipv6
	{
		p_ip6 = (jl_acl_ip6_t*)val_out;

		*(jl_uint64*)&p_ip6->value.addr[0] = val_in->val_0_63;
		*(jl_uint64*)&p_ip6->value.addr[4] = val_in->val_64_127;
		*(jl_uint64*)&p_ip6->mask.addr[0] = val_in->mask_0_63;
		*(jl_uint64*)&p_ip6->mask.addr[4] = val_in->mask_64_127;
	}
	else
		return JL_ERR_PARAM;

	return JL_ERR_OK;
}

static jl_ret_t jl61xx_acl_get_mask_val(jl_device_t* dev,
								jl_acl_field_data_t* field_rule,
									jl61xx_acl_val128_t* val_out)
{
	jl_ret_t ret = JL_ERR_OK;
	jl_uint8 bit_width = 0;
	jl_uint32 mask_val = 0;

	val_out->val_0_63 = 0;
	val_out->val_64_127 = 0;
	val_out->mask_0_63 = 0;
	val_out->mask_64_127 = 0;

	JL_CHECK_POINTER(dev);
	JL_CHECK_POINTER(field_rule);
	JL_CHECK_POINTER(val_out);

	if (field_rule->field_type >= ACL_FIELD_NUM)
	return JL_ERR_PARAM;

	bit_width = jl61xx_acl_field_bit_size[field_rule->field_type];

	if (bit_width <= 32)
		mask_val = BITS_WIDTH(bit_width);

	switch (field_rule->field_type)
	{
		case ACL_FIELD_SOURCE_PORT:
		case ACL_FIELD_OUTER_VID:
		case ACL_FIELD_OUTER_PCP:
		case ACL_FIELD_OUTER_DEI:
		case ACL_FIELD_INNER_VID:
		case ACL_FIELD_INNER_PCP:
		case ACL_FIELD_INNER_DEI:
		case ACL_FIELD_ETH_TYPE:
		case ACL_FIELD_MPLS:
		case ACL_FIELD_TOS:
		case ACL_FIELD_TTL:
		case ACL_FIELD_L4_PROTOCOL:
		case ACL_FIELD_L4_SOURCE_PORT:
		case ACL_FIELD_L4_DEST_PORT:
		case ACL_FIELD_ICMP_TYPE:
		case ACL_FIELD_ICMP_CODE:
		case ACL_FIELD_ICMP6_TYPE:
		case ACL_FIELD_ICMP6_CODE:
		case ACL_FIELD_IGMP_TYPE:
			val_out->val_0_63 = field_rule->field.val_data.value & 
										field_rule->field.val_data.mask;
			val_out->mask_0_63 = field_rule->field.val_data.mask;
			break;
		case ACL_FIELD_MAC_DA:
		case ACL_FIELD_MAC_SA:
			ret = jl61xx_acl_l2v((void*)&field_rule->field.mac_data, 48, val_out);
			if (ret)
				return ret;
			break;
		case ACL_FIELD_HAS_VLAN:
			val_out->val_0_63 = field_rule->field.has_vlans.value;
			val_out->mask_0_63 = mask_val;
			break;
		case ACL_FIELD_OUTER_ETH_TYPE:
			val_out->val_0_63 = field_rule->field.outer_cstag.value;
			val_out->mask_0_63 = mask_val;
			break;
		case ACL_FIELD_INNER_ETH_TYPE:
			val_out->val_0_63 = field_rule->field.inner_cstag.value;
			val_out->mask_0_63 = mask_val;
			break;
		case ACL_FIELD_L3_TYPE:
			val_out->val_0_63 = field_rule->field.l3_type.value;
			val_out->mask_0_63 = mask_val;
			break;
		case ACL_FIELD_IP_SA:
		case ACL_FIELD_IP_DA:
		case ACL_FIELD_IGMP_GROUP_ADDR:
			ret = jl61xx_acl_l2v((void*)&field_rule->field.ip_data, 32, val_out);
			if (ret)
				return ret;
			break;
		case ACL_FIELD_IP6_SA:
		case ACL_FIELD_IP6_DA:
		case ACL_FIELD_MLD_ADDR:
			ret =jl61xx_acl_l2v((void*)&field_rule->field.ip6_data, 128, val_out);
			if (ret)
				return ret;
			break;
		case ACL_FIELD_L4_TYPE:
			val_out->val_0_63 = field_rule->field.l4_type.value;
			val_out->mask_0_63 = mask_val;
			break;
		case ACL_FIELD_RULE_POINTER:
			val_out->val_0_63 = field_rule->field.rule_pointer.value;	//template index
			val_out->mask_0_63 = mask_val;
			break;
		default:
				return JL_ERR_PARAM;
	}

	return 	JL_ERR_OK;
}

static jl_ret_t jl61xx_acl_rule_get_origin_key(jl_device_t *dev, 
											jl61xx_acl_data_t* pdata,
											jl61xx_acl_hash_key_t *pkey)
{
	jl_ret_t	ret = JL_ERR_OK;
	jl_uint8	bit_offset = 0;
	jl_uint8	bit_width = 0;
	jl_uint8	field_mask = 0;
	jl61xx_acl_val128_t	val;
	jl_acl_field_data_t	*pfield = NULL;

	JL_CHECK_POINTER(dev);
	JL_CHECK_POINTER(pdata);
	JL_CHECK_POINTER(pkey);

	pfield = pdata->origin_data;
	if (pfield == NULL)
		return JL_ERR_CONFIG;

	field_mask = pdata->field_mask & 0x3f;
	jl_write_bits(pkey->value, field_mask, 0, 6); //set field valid bits
	jl_write_bits(pkey->mask, field_mask, 0, 6);

	while (pfield)
	{
		switch (pfield->field_type)
		{
			case ACL_FIELD_MAC_DA:
			case ACL_FIELD_MAC_SA:
			case ACL_FIELD_IP_SA:
			case ACL_FIELD_IP_DA:
			case ACL_FIELD_IGMP_GROUP_ADDR:
			case ACL_FIELD_IP6_SA:
			case ACL_FIELD_IP6_DA:
			case ACL_FIELD_MLD_ADDR:
			case ACL_FIELD_HAS_VLAN:
			case ACL_FIELD_OUTER_ETH_TYPE:
			case ACL_FIELD_INNER_ETH_TYPE:
			case ACL_FIELD_L3_TYPE:
			case ACL_FIELD_L4_TYPE:
			case ACL_FIELD_RULE_POINTER:
				bit_width = jl61xx_acl_field_bit_size[pfield->field_type];
				bit_offset = pfield->reg_offset;

				ret = jl61xx_acl_get_mask_val(dev, pfield, &val);
				if (ret)
					return ret;

				pkey->tcam_flag |= pfield->tcam_flag;

				if (pfield->field_type == ACL_FIELD_IP6_SA ||
					pfield->field_type == ACL_FIELD_IP6_DA ||
					pfield->field_type == ACL_FIELD_MLD_ADDR)
				{
					jl_write_bits(pkey->mask, val.mask_0_63, bit_offset + 6, 64);
					jl_write_bits(pkey->mask, val.mask_64_127, bit_offset + 64 + 6, bit_width - 64);

					jl_write_bits(pkey->value, val.val_0_63, bit_offset + 6, 64);
					jl_write_bits(pkey->value, val.val_64_127, bit_offset + 64 + 6, bit_width - 64);
				}
				else
				{
					jl_write_bits(pkey->mask, val.mask_0_63, bit_offset + 6, bit_width);
					jl_write_bits(pkey->value, val.val_0_63, bit_offset + 6, bit_width);
				}
				break;
			default:
				break;
		}
		pfield = pfield->next;
	}

	return JL_ERR_OK;
}

static jl_ret_t jl61xx_acl_rule_get_split_key_list(jl_device_t *dev,
										jl61xx_acl_split_data_t *pdata,
										jl61xx_acl_hash_key_t *pkey,
										jl61xx_acl_key_list_t **plist)
{
	jl_ret_t	ret = JL_ERR_OK;
	jl_uint8	bit_offset = 0;
	jl_uint8	bit_width = 0;

	jl_acl_field_data_t	*pentry;
	jl61xx_acl_key_list_t *pkey_entry;
	jl61xx_acl_key_list_t *pkey_temp;
	jl61xx_acl_val128_t	val;
	jl61xx_acl_hash_key_t key = {0};
	
	if (pdata == NULL) {
		pkey_entry = (jl61xx_acl_key_list_t*)port_mem_malloc(sizeof(jl61xx_acl_key_list_t));
		if (pkey_entry == NULL)
			return JL_ERR_MEMORY;

		pkey_entry->key = *pkey;
		pkey_entry->next = NULL;

		if (*plist == NULL)
			*plist = pkey_entry;
		else 
		{
			pkey_temp = (*plist)->end;
			pkey_temp->next = pkey_entry;
		}

		(*plist)->end = pkey_entry;

		return JL_ERR_OK;
	}

	pentry = pdata->entry_head;

	while (pentry) {
		key = *pkey;
		ret = jl61xx_acl_get_mask_val(dev, pentry, &val);
		if (ret)
			return ret;

		bit_width = jl61xx_acl_field_bit_size[pentry->field_type];

		bit_offset = pdata->reg_offset;

		key.tcam_flag |= pentry->tcam_flag;

		jl_write_bits(key.value, val.val_0_63, bit_offset + 6, bit_width);
		jl_write_bits(key.mask, val.mask_0_63, bit_offset + 6, bit_width);
		
		ret = jl61xx_acl_rule_get_split_key_list(dev, pdata->next, &key, plist);
		if (ret)
			return ret;

		pentry = pentry->next;
	}

	return JL_ERR_OK;
}

static jl_ret_t jl61xx_acl_write_table(jl_device_t *dev, 
									jl_uint8 index, 
									jl61xx_acl_data_t* pdata,
									jl_acl_action_t *acl_action)
{
	jl_ret_t	ret = JL_ERR_OK;
	jl61xx_acl_hash_key_t key;
	jl61xx_acl_hash_key_t key_temp;
	jl61xx_acl_key_list_t *pkey_entry = NULL;
	jl61xx_acl_key_list_t *pkey_temp = NULL;
	jl_uint8	idx = 0;
	
	JL_CHECK_POINTER(dev);
	JL_CHECK_POINTER(pdata);
	JL_CHECK_POINTER(acl_action);

	memset((void*)&key, 0, sizeof(jl61xx_acl_hash_key_t));
	memset((void*)&key_temp, 0, sizeof(jl61xx_acl_hash_key_t));

	ret = jl61xx_acl_rule_get_origin_key(dev, pdata, &key);
	if (ret)
		return ret;

	ret = jl61xx_acl_rule_get_split_key_list(dev, pdata->split_data, &key_temp, &pkey_entry);
	if (ret)
		return ret;

	ret = jl61xx_acl_clear_table(dev, index);
	if (ret)
		return ret;

	pkey_temp = pkey_entry;

	while (pkey_temp) {
		/*make up key*/
		key_temp = key;

		for (idx = 0; idx < 26; idx++) {
			key_temp.value[idx] |= pkey_temp->key.value[idx];
			key_temp.mask[idx] |= pkey_temp->key.mask[idx];
		}
		
		if ((pdata->tcam_flag | key_temp.tcam_flag | pkey_temp->key.tcam_flag) == 0) {
			/*try hash large*/
			ret = jl61xx_acl_write_hash_large(dev, &key_temp, acl_action, &pdata->rule_entry);
			if (ret == JL_ERR_RESOURCE) {
				/*try hash small*/
				ret = jl61xx_acl_write_hash_small(dev, &key_temp, acl_action, &pdata->rule_entry);
				if (ret == JL_ERR_RESOURCE) {
					/*try tcam*/
					ret = jl61xx_acl_write_tcam(dev, &key_temp, acl_action, &pdata->rule_entry);
					if (ret)
						goto end;
				}
				else if (ret)
					goto end;
			}
			else if (ret)
				goto end;
		}
		else {
			ret = jl61xx_acl_write_tcam(dev, &key_temp, acl_action, &pdata->rule_entry);
			if (ret)
				goto end;
		}
		pkey_temp = pkey_temp->next;
	}

end:
	(void)jl61xx_acl_info_set(dev, index, pdata, acl_action);

	if (ret)
		(void)jl61xx_acl_clear_table(dev, index);

	pkey_temp = pkey_entry;
	while(pkey_temp) {
		pkey_entry = pkey_temp;
		pkey_temp = pkey_temp->next;
		port_mem_free(pkey_entry);
	}

	return ret;
}

jl_ret_t jl61xx_acl_rule_add(jl_device_t *dev,
				jl_uint8 index,
				jl_acl_rule_t *acl_rule,
				jl_acl_action_t *acl_action)
{
	jl_ret_t	ret = JL_ERR_OK;
	jl61xx_acl_data_t data;

	JL_CHECK_POINTER(dev);
	JL_CHECK_POINTER(acl_rule);
	JL_CHECK_POINTER(acl_action);

	if (acl_rule->field_num == 0)
		return JL_ERR_CONFIG;

	memset((void*)&data, 0, sizeof(jl61xx_acl_data_t));

	/*check action*/
	ret = jl61xx_acl_action_chk(dev, acl_action);
	if (ret)
		return ret;

	/*check data & split rules*/
	ret = jl61xx_acl_pre_process(dev, acl_rule, &data);
	if (ret)
		goto end;
	
	/*check space*/
	ret = jl61xx_acl_space_chk(dev, index, &data);
	if (ret)
		goto end;

	/*write table*/
	ret = jl61xx_acl_write_table(dev, index, &data, acl_action);

end:
	jl61xx_acl_after_process(&data);
	
	return ret;
}

jl_ret_t jl61xx_acl_rule_get(jl_device_t *dev,
				jl_uint8 index,
				jl_acl_rule_t *acl_rule,
				jl_acl_action_t *acl_action)
{
	jl_ret_t ret = JL_ERR_OK;
	jl61xx_acl_data_t t_data;
	jl61xx_acl_rule_entry_t *rule_entry;
	jl_acl_field_data_t* pfield;
	jl_acl_field_data_t* ptemp;
	jl_uint8 has_get = 0;

	JL_CHECK_POINTER(dev);
	JL_CHECK_POINTER(acl_rule);
	JL_CHECK_POINTER(acl_action);

	memset(acl_rule, 0, sizeof(jl_acl_rule_t));
	memset(acl_action, 0, sizeof(jl_acl_action_t));

	ret = jl61xx_acl_info_get(dev, index, &t_data, NULL);
	if (ret)
		return ret;

	rule_entry = t_data.rule_entry;
	acl_rule->templ_idx = t_data.templ_idx;
	acl_rule->field_num = t_data.field_num;
	acl_rule->field_mask = t_data.field_mask;

	while (rule_entry) {
		if (rule_entry->table == ACL_TBL_TCAM)
			ret = jl61xx_acl_read_tcam(dev, rule_entry->index, &has_get, acl_rule, acl_action);
		else if (rule_entry->table == ACL_TBL_LARGE)
			ret = jl61xx_acl_read_hash_large(dev, rule_entry->index, &has_get, acl_rule, acl_action);
		else
			ret = jl61xx_acl_read_hash_mall(dev, rule_entry->index, &has_get, acl_rule, acl_action);
		
		if (ret)
			break;		
		rule_entry = rule_entry->next;
	}

	if (ret) {
		//free pre memery
		pfield = acl_rule->head;
		while (pfield) {
			ptemp = pfield;
			pfield = pfield->next;
			port_mem_free(ptemp);
		}
		acl_rule->head = NULL;
	}

	pfield = acl_rule->head;
	while (pfield) {
		if(pfield->field_type == ACL_FIELD_SOURCE_PORT) {
			if (pfield->field.source_port.type == ACL_DATA_SPECIFIC_VALUE) {
				pfield->field.source_port.mask = 0; /*mask is invalid for 'source-port' with specific value*/
			}
			break;
		}
		pfield = pfield->next;
	}

	return ret;
}

jl_ret_t jl61xx_acl_rule_del(jl_device_t *dev, jl_uint8 index)
{
	jl_ret_t ret = JL_ERR_OK;

	JL_CHECK_POINTER(dev);

	ret = jl61xx_acl_clear_table(dev, index);

	return ret;
}


jl_ret_t jl61xx_acl_selection_set(jl_device_t *dev, jl_uint8 val)
{
	jl_ret_t ret = JL_ERR_OK;

	JL_CHECK_POINTER(dev);

	SWCORE_CONFIGURABLE_ACL_SELECTION_t selec_tbl;

	memset(&selec_tbl, 0, sizeof(SWCORE_CONFIGURABLE_ACL_SELECTION_t));

	selec_tbl.BF.select_tcam_or_table = (val & 0x1);
	selec_tbl.BF.select_small_or_large = ((val >> 1) & 0x1);

	REGISTER_WRITE(dev, SWCORE, CONFIGURABLE_ACL_SELECTION, selec_tbl, 0, 0);

	return JL_ERR_OK;
}

jl_ret_t jl61xx_acl_selection_get(jl_device_t *dev, jl_uint8 *val)
{
	jl_ret_t ret = JL_ERR_OK;

	JL_CHECK_POINTER(dev);

	REGISTER_READ(dev, SWCORE, CONFIGURABLE_ACL_SELECTION, selec_tbl, 0, 0);

	*val = selec_tbl.val[0];

	return JL_ERR_OK;
}


jl_ret_t jl61xx_acl_stat_read(jl_device_t *dev,
					jl_uint8 counter,
					jl_uint32 *packets)
{
	jl_ret_t ret = JL_ERR_OK;

	JL_CHECK_POINTER(dev);
	JL_CHECK_POINTER(packets);

	REGISTER_READ(dev, SWCORE, INGRESS_CONFIGURABLE_ACL_MATCH_COUNTER, counter_tbl, counter, 0);

	*packets = counter_tbl.BF.packets;

	return JL_ERR_OK;
}

jl_ret_t jl61xx_acl_stat_write(jl_device_t *dev,
					jl_uint8 counter,
					jl_uint32 packets)
{
	jl_ret_t ret = JL_ERR_OK;

	JL_CHECK_POINTER(dev);

	SWCORE_INGRESS_CONFIGURABLE_ACL_MATCH_COUNTER_t counter_tbl;

	memset(&counter_tbl, 0, sizeof(SWCORE_INGRESS_CONFIGURABLE_ACL_MATCH_COUNTER_t));

	counter_tbl.BF.packets = packets & 0xffffff;

	REGISTER_WRITE(dev, SWCORE, INGRESS_CONFIGURABLE_ACL_MATCH_COUNTER, counter_tbl, counter, 0);

	return JL_ERR_OK;
}

static jl_ret_t jl61xx_acl_stat_enable_tcam(jl_device_t *dev,
						jl_uint8 index,
						jl_uint8 counter,
						jl_uint8 enable)
{
	jl_ret_t ret = JL_ERR_OK;

	JL_CHECK_POINTER(dev);

	REGISTER_READ(dev, SWCORE, CONFIGURABLE_ACL_TCAM_ANSWER, answer_tbl, index, 0);

	if (enable)
	{
		answer_tbl.BF.update_counter = 1;
		answer_tbl.BF.counter = counter;
	}
	else
	{
		answer_tbl.BF.update_counter = 0;
		answer_tbl.BF.counter = 0;
	}

	REGISTER_WRITE(dev, SWCORE, CONFIGURABLE_ACL_TCAM_ANSWER, answer_tbl, index, 0);

	return JL_ERR_OK;
}

static jl_ret_t jl61xx_acl_stat_enable_hash_large(jl_device_t *dev,
						jl_uint8 index,
						jl_uint8 counter,
						jl_uint8 enable)
{
	jl_ret_t ret = JL_ERR_OK;

	JL_CHECK_POINTER(dev);

	REGISTER_READ(dev, SWCORE, CONFIGURABLE_ACL_LARGE_TABLE, hash_tbl, index, 0);

	if (enable)
	{
		hash_tbl.BF.update_counter = 1;
		hash_tbl.BF.counter = counter;
	}
	else
	{
		hash_tbl.BF.update_counter = 0;
		hash_tbl.BF.counter = 0;
	}

	REGISTER_WRITE(dev, SWCORE, CONFIGURABLE_ACL_LARGE_TABLE, hash_tbl, index, 0);

	return JL_ERR_OK;
}

static jl_ret_t jl61xx_acl_stat_enable_hash_small(jl_device_t *dev,
						jl_uint8 index,
						jl_uint8 counter,
						jl_uint8 enable)
{
	jl_ret_t ret = JL_ERR_OK;

	JL_CHECK_POINTER(dev);

	REGISTER_READ(dev, SWCORE, CONFIGURABLE_ACL_SMALL_TABLE, hash_tbl, index, 0);

	if (enable)
	{
		hash_tbl.BF.update_counter = 1;
		hash_tbl.BF.counter = counter;
	}
	else
	{
		hash_tbl.BF.update_counter = 0;
		hash_tbl.BF.counter = 0;
	}

	REGISTER_WRITE(dev, SWCORE, CONFIGURABLE_ACL_SMALL_TABLE, hash_tbl, index, 0);

	return JL_ERR_OK;
}

jl_ret_t jl61xx_acl_stat_enable(jl_device_t *dev,
								jl_uint8 index,
								jl_uint8 counter)
{
	jl_ret_t ret = JL_ERR_OK;
	jl61xx_acl_data_t acl_rule;
	jl_acl_action_t acl_act;
	jl_uint8 enable = TRUE;
	jl61xx_acl_rule_entry_t *entry;

	JL_CHECK_POINTER(dev);

	memset(&acl_rule, 0, sizeof(jl61xx_acl_data_t));
	memset(&acl_act, 0, sizeof(jl_acl_action_t));

	ret = jl61xx_acl_info_get(dev, index, &acl_rule, &acl_act);
	if (ret)
		return ret;

	if (acl_act.update_counter_enable &&
		(acl_act.counter == counter))
	{
		/*none change*/
		return JL_ERR_OK;
	}

	entry = acl_rule.rule_entry;

	while (entry) {
		if (entry->table == ACL_TBL_TCAM)
			ret = jl61xx_acl_stat_enable_tcam(dev, entry->index, counter, enable);
		else if (entry->table == ACL_TBL_LARGE)
			ret = jl61xx_acl_stat_enable_hash_large(dev, entry->index, counter, enable);
		else
			ret = jl61xx_acl_stat_enable_hash_small(dev, entry->index, counter, enable);
		
		if (ret)
			return ret;

		entry = entry->next;
	}

	/*update rule info*/
	acl_act.update_counter_enable = TRUE;
	acl_act.counter = counter;

	ret = jl61xx_acl_info_counter_clear(dev, index);
	if (ret)
		return ret;

	ret = jl61xx_acl_info_counter_add(dev, index, &acl_act);

	return ret;
}

jl_ret_t jl61xx_acl_stat_disable(jl_device_t *dev, jl_uint8 index)
{
	jl_ret_t ret = JL_ERR_OK;
	jl61xx_acl_data_t acl_rule;
	jl_acl_action_t acl_act;
	jl_uint8 enable = FALSE;
	jl61xx_acl_rule_entry_t *entry;

	JL_CHECK_POINTER(dev);

	memset(&acl_rule, 0, sizeof(jl61xx_acl_data_t));
	memset(&acl_act, 0, sizeof(jl_acl_action_t));

	ret = jl61xx_acl_info_get(dev, index, &acl_rule, &acl_act);
	if (ret)
		return ret;

	if (!acl_act.update_counter_enable)
	{
		/*none change*/
		return JL_ERR_OK;
	}

	entry = acl_rule.rule_entry;

	while (entry) {
		if (entry->table == ACL_TBL_TCAM)
			ret = jl61xx_acl_stat_enable_tcam(dev, entry->index, 0, enable);
		else if (entry->table == ACL_TBL_LARGE)
			ret = jl61xx_acl_stat_enable_hash_large(dev, entry->index, 0, enable);
		else
			ret = jl61xx_acl_stat_enable_hash_small(dev, entry->index, 0, enable);
		
		if (ret)
			return ret;

		entry = entry->next;
	}

	ret = jl61xx_acl_info_counter_clear(dev, index);
	if (ret)
		return ret;

	return ret;
}

jl_ret_t jl61xx_acl_stat_counter_get(jl_device_t *dev, jl_uint8 *counter)
{
	jl_uint8 idx = 0;

	JL_CHECK_POINTER(dev);
	JL_CHECK_POINTER(counter);

	for (; idx < ACL_MATCH_COUNTER; idx++)
	{
		if (gp_acl_rule_info[dev->id]->counter[idx] == 0)
		{
			*counter = idx;
			return JL_ERR_OK;
		}
	}

	return JL_ERR_RESOURCE;
}

static jl_ret_t jl61xx_acl_mac_action_chk(jl_device_t *dev, jl_acl_mac_action_t *mac_action)
{
	JL_CHECK_POINTER(dev);
	JL_CHECK_POINTER(mac_action);

	if (mac_action->drop_enable > 1)
		return JL_ERR_PARAM;

	if (mac_action->send2cpu_enable > 1)
		return JL_ERR_PARAM;

	if (mac_action->force_queue_enable > 1)
		return JL_ERR_PARAM;

	if (mac_action->force_queue_enable && mac_action->e_queue >= ACL_QUEUE_NUM)
		return JL_ERR_PARAM;

	if (mac_action->force_color_enable > 1)
		return JL_ERR_PARAM;

	if (mac_action->force_color_enable && mac_action->color > ACL_COLOR_RED)
		return JL_ERR_PARAM;

	if (mac_action->mmp_valid > 1)
		return JL_ERR_PARAM;

	if (mac_action->mmp_valid &&
				(mac_action->mmp_order > 3 || mac_action->mmp_ptr >= ACL_MMP_POINTER_NUM))
		return JL_ERR_PARAM;

	JL_CHECK_PORTMASK(dev, mac_action->port_mask);

	return JL_ERR_OK;
}

jl_ret_t jl61xx_acl_mac_rule_add(jl_device_t *dev,
				jl_uint8 index,
				jl_acl_mac_range_t *mac_range,
				jl_acl_mac_action_t *mac_action)
{
	jl_ret_t ret = JL_ERR_OK;
	jl_uint32 cportmask = 0;
	jl_uint64 val, vals = 0, vale = 0;
	jl_uint8 i;
	SWCORE_RESERVED_SOURCE_MAC_ADDRESS_RANGE_t smac_tbl;
	SWCORE_RESERVED_DESTINATION_MAC_ADDRESS_RANGE_t dmac_tbl;


	JL_CHECK_POINTER(dev);
	JL_CHECK_POINTER(mac_range);
	JL_CHECK_POINTER(mac_action);

	for (i = 0; i < 6; i++)
	{
		val = mac_range->mac_s.addr[i];
		val <<= ((5 - i) * 8);
		vals |= val;

		val = mac_range->mac_e.addr[i];
		val <<= ((5 - i) * 8);
		vale |= val;
	}

	if (vals > vale)
		return JL_ERR_CONFIG;

	ret = jl61xx_acl_mac_action_chk(dev, mac_action);
	if (ret)
		return ret;

	ret = jlxx_portmask_l2c(dev, mac_action->port_mask, &cportmask);
	if (ret)
		return ret;

	if (mac_range->mac_type == ACL_MAC_SOURCE)
	{
		memset(&smac_tbl, 0, sizeof(SWCORE_RESERVED_SOURCE_MAC_ADDRESS_RANGE_t));

		smac_tbl.BF.start_addr_0_31 = vals & 0xffffffff;

		smac_tbl.BF.start_addr_32_47 = (vals >> SWCORE_RESERVED_SOURCE_MAC_ADDRESS_RANGE_OFFSET_START_ADDR_0_31_WIDTH);

		smac_tbl.BF.stop_addr_0_15 = vale & 0xffff;

		smac_tbl.BF.stop_addr_16_47 = (vale >> SWCORE_RESERVED_SOURCE_MAC_ADDRESS_RANGE_OFFSET_STOP_ADDR_0_15_WIDTH);

		smac_tbl.BF.drop_enable = mac_action->drop_enable;
		smac_tbl.BF.send_to_cpu = mac_action->send2cpu_enable;
		smac_tbl.BF.force_queue = mac_action->force_queue_enable;

		smac_tbl.BF.e_queue = mac_action->e_queue;
		smac_tbl.BF.force_color = mac_action->force_color_enable;
		smac_tbl.BF.color = mac_action->color;
		smac_tbl.BF.mmp_valid = mac_action->mmp_valid;
		smac_tbl.BF.mmp_ptr = mac_action->mmp_ptr;
		smac_tbl.BF.mmp_order = mac_action->mmp_order;
		smac_tbl.BF.enable = cportmask & 0x7ff;

		REGISTER_WRITE(dev, SWCORE, RESERVED_SOURCE_MAC_ADDRESS_RANGE, smac_tbl, index, 0);

		if (gp_acl_rule_info[dev->id]->smac_rule[index].mmp_valid)
		{
			ret = jl61xx_clear_mmp_pointer_info(dev, JL_MMP_MODULE_SOURCE_MAC,
											gp_acl_rule_info[dev->id]->smac_rule[index].mmp_index);
			if (ret)
				return ret;
		}
		if (mac_action->mmp_valid)
		{
			ret = jl61xx_set_mmp_pointer_info(dev, JL_MMP_MODULE_SOURCE_MAC, mac_action->mmp_ptr);
			if (ret)
				return ret;

			gp_acl_rule_info[dev->id]->smac_rule[index].mmp_index = mac_action->mmp_ptr;
		}
		gp_acl_rule_info[dev->id]->smac_rule[index].mmp_valid = mac_action->mmp_valid;
	}
	else if (mac_range->mac_type == ACL_MAC_DEST)
	{
		memset(&dmac_tbl, 0, sizeof(SWCORE_RESERVED_DESTINATION_MAC_ADDRESS_RANGE_t));

		dmac_tbl.BF.start_addr_0_31 = vals & 0xffffffff;

		dmac_tbl.BF.start_addr_32_47 =
				(vals >> SWCORE_RESERVED_DESTINATION_MAC_ADDRESS_RANGE_OFFSET_START_ADDR_0_31_WIDTH);

		dmac_tbl.BF.stop_addr_0_15 = vale & 0xffff;

		dmac_tbl.BF.stop_addr_16_47 =
				(vale >> SWCORE_RESERVED_DESTINATION_MAC_ADDRESS_RANGE_OFFSET_STOP_ADDR_0_15_WIDTH);

		dmac_tbl.BF.drop_enable = mac_action->drop_enable;
		dmac_tbl.BF.send_to_cpu = mac_action->send2cpu_enable;
		dmac_tbl.BF.force_queue = mac_action->force_queue_enable;

		dmac_tbl.BF.e_queue = mac_action->e_queue;
		dmac_tbl.BF.force_color = mac_action->force_color_enable;
		dmac_tbl.BF.color = mac_action->color;
		dmac_tbl.BF.mmp_valid = mac_action->mmp_valid;
		dmac_tbl.BF.mmp_ptr = mac_action->mmp_ptr;
		dmac_tbl.BF.mmp_order = mac_action->mmp_order;
		dmac_tbl.BF.enable = cportmask & 0x7ff;

		REGISTER_WRITE(dev, SWCORE, RESERVED_DESTINATION_MAC_ADDRESS_RANGE, dmac_tbl, index, 0);

		if (gp_acl_rule_info[dev->id]->dmac_rule[index].mmp_valid)
		{
			ret = jl61xx_clear_mmp_pointer_info(dev, JL_MMP_MODULE_DEST_MAC,
											gp_acl_rule_info[dev->id]->dmac_rule[index].mmp_index);
			if (ret)
				return ret;
		}
		if (mac_action->mmp_valid)
		{
			ret = jl61xx_set_mmp_pointer_info(dev, JL_MMP_MODULE_DEST_MAC, mac_action->mmp_ptr);
			if (ret)
				return ret;

			gp_acl_rule_info[dev->id]->dmac_rule[index].mmp_index = mac_action->mmp_ptr;
		}
		gp_acl_rule_info[dev->id]->dmac_rule[index].mmp_valid = mac_action->mmp_valid;
	}
	else
		return JL_ERR_PARAM;

	return JL_ERR_OK;
}

jl_ret_t jl61xx_acl_mac_rule_del(jl_device_t *dev,
				jl_uint8 index,
				jl_acl_mac_type_t mac_type)
{
	jl_ret_t ret = JL_ERR_OK;

	SWCORE_RESERVED_SOURCE_MAC_ADDRESS_RANGE_t smac_tbl;
	SWCORE_RESERVED_DESTINATION_MAC_ADDRESS_RANGE_t dmac_tbl;

	JL_CHECK_POINTER(dev);

	if (mac_type == ACL_MAC_SOURCE)
	{
		memset(&smac_tbl, 0, sizeof(SWCORE_RESERVED_SOURCE_MAC_ADDRESS_RANGE_t));
		REGISTER_WRITE(dev, SWCORE, RESERVED_SOURCE_MAC_ADDRESS_RANGE, smac_tbl,index, 0);

		if (gp_acl_rule_info[dev->id]->smac_rule[index].mmp_valid)
		{
			ret = jl61xx_set_mmp_pointer_info(dev, JL_MMP_MODULE_SOURCE_MAC,
										gp_acl_rule_info[dev->id]->smac_rule[index].mmp_index);
			if (ret)
				return ret;

			gp_acl_rule_info[dev->id]->smac_rule[index].mmp_valid = 0;
		}
	}
	else
	{
		memset(&dmac_tbl, 0, sizeof(SWCORE_RESERVED_DESTINATION_MAC_ADDRESS_RANGE_t));
		REGISTER_WRITE(dev, SWCORE, RESERVED_DESTINATION_MAC_ADDRESS_RANGE, dmac_tbl,index, 0);

		if (gp_acl_rule_info[dev->id]->dmac_rule[index].mmp_valid)
		{
			ret = jl61xx_set_mmp_pointer_info(dev, JL_MMP_MODULE_DEST_MAC,
										gp_acl_rule_info[dev->id]->dmac_rule[index].mmp_index);
			if (ret)
				return ret;

			gp_acl_rule_info[dev->id]->dmac_rule[index].mmp_valid = 0;
		}
	}
	return JL_ERR_OK;
}

jl_ret_t jl61xx_acl_mac_rule_get(jl_device_t *dev,
				jl_uint8 index,
				jl_acl_mac_type_t mac_type,
				jl_acl_mac_range_t *mac_range,
				jl_acl_mac_action_t *mac_action)
{
	jl_ret_t ret = JL_ERR_OK;
	jl_uint64 value = 0, vals = 0,vale = 0;
	jl_uint32 lportmask = 0;
	jl_uint8 i = 0;

	JL_CHECK_POINTER(dev);
	JL_CHECK_POINTER(mac_range);
	JL_CHECK_POINTER(mac_action);

	if (mac_type == ACL_MAC_SOURCE)
	{
		REGISTER_READ(dev, SWCORE, RESERVED_SOURCE_MAC_ADDRESS_RANGE, smac_tbl, index, 0);

		vals = smac_tbl.BF.start_addr_0_31;
		value = smac_tbl.BF.start_addr_32_47;
		value <<= SWCORE_RESERVED_SOURCE_MAC_ADDRESS_RANGE_OFFSET_START_ADDR_0_31_WIDTH;
		vals |= value;

		vale = smac_tbl.BF.stop_addr_0_15;
		value = smac_tbl.BF.stop_addr_16_47;
		value <<= SWCORE_RESERVED_SOURCE_MAC_ADDRESS_RANGE_OFFSET_STOP_ADDR_0_15_WIDTH;
		vale |= value;

		mac_action->drop_enable = smac_tbl.BF.drop_enable;
		mac_action->send2cpu_enable = smac_tbl.BF.send_to_cpu;
		mac_action->force_queue_enable = smac_tbl.BF.force_queue;
		mac_action->e_queue = smac_tbl.BF.e_queue;
		mac_action->force_color_enable = smac_tbl.BF.force_color;
		mac_action->color = smac_tbl.BF.color;
		mac_action->mmp_valid = smac_tbl.BF.mmp_valid;
		mac_action->mmp_ptr = smac_tbl.BF.mmp_ptr;
		mac_action->mmp_order = smac_tbl.BF.mmp_order;

		ret = jlxx_portmask_c2l(dev, smac_tbl.BF.enable, &lportmask);
		if (ret)
			return ret;

		mac_action->port_mask = lportmask;
	}
	else if (mac_type == ACL_MAC_DEST)
	{
		REGISTER_READ(dev, SWCORE, RESERVED_DESTINATION_MAC_ADDRESS_RANGE, dmac_tbl,index, 0);

		vals = dmac_tbl.BF.start_addr_0_31;
		value = dmac_tbl.BF.start_addr_32_47;
		value <<= SWCORE_RESERVED_DESTINATION_MAC_ADDRESS_RANGE_OFFSET_START_ADDR_0_31_WIDTH;
		vals |= value;

		vale = dmac_tbl.BF.stop_addr_0_15;
		value = dmac_tbl.BF.stop_addr_16_47;
		value <<= SWCORE_RESERVED_DESTINATION_MAC_ADDRESS_RANGE_OFFSET_STOP_ADDR_0_15_WIDTH;
		vale |= value;

		mac_action->drop_enable = dmac_tbl.BF.drop_enable;
		mac_action->send2cpu_enable = dmac_tbl.BF.send_to_cpu;
		mac_action->force_queue_enable = dmac_tbl.BF.force_queue;
		mac_action->e_queue = dmac_tbl.BF.e_queue;
		mac_action->force_color_enable = dmac_tbl.BF.force_color;
		mac_action->color = dmac_tbl.BF.color;
		mac_action->mmp_valid = dmac_tbl.BF.mmp_valid;
		mac_action->mmp_ptr = dmac_tbl.BF.mmp_ptr;
		mac_action->mmp_order = dmac_tbl.BF.mmp_order;

		ret = jlxx_portmask_c2l(dev, dmac_tbl.BF.enable, &lportmask);
		if (ret)
			return ret;

		mac_action->port_mask = lportmask;
	}
	else
		return JL_ERR_PARAM;

	mac_range->mac_type = mac_type;

	for (i = 0; i < 6; i++)
	{
		mac_range->mac_s.addr[i] = vals >> ((5 - i) * 8) & 0xff;
		mac_range->mac_e.addr[i] = vale >> ((5 - i) * 8) & 0xff;
	}

	return JL_ERR_OK;
}

jl_ret_t jl61xx_acl_source_port_set(jl_device_t *dev,
					jl_uint8 lport,
					jl_acl_port_t *port_state)
{
	jl_ret_t ret = JL_ERR_OK;
	jl_uint8 cport;

	JL_CHECK_POINTER(dev);
	JL_CHECK_POINTER(port_state);
	JL_CHECK_PORT(dev, lport);

	if (port_state->valid == FALSE)
		return JL_ERR_OK;

	cport = jlxx_port_l2c(dev, lport);

	REGISTER_READ(dev, SWCORE, SOURCE_PORT_TABLE, port_tbl, cport, 0);

	if (port_state->enable == DISABLED)
	{
		port_tbl.BF.use_acl = 0;
		port_tbl.BF.acl_rule = 0;
		port_tbl.BF.acl_rule_icmp = 0;
		port_tbl.BF.acl_rule_icmpv6 = 0;
		port_tbl.BF.acl_rule_igmp = 0;
	}
	else
	{
		if (port_state->acl_tpl_type >= ACL_TEMPLATE_NUM ||
			port_state->icmp_tpl_type >= ACL_TEMPLATE_NUM ||
			port_state->icmpv6_tpl_type >= ACL_TEMPLATE_NUM ||
			port_state->igmp_tpl_type >= ACL_TEMPLATE_NUM)
		{
			return JL_ERR_PARAM;
		}

		port_tbl.BF.use_acl = 1;
		port_tbl.BF.acl_rule = port_state->acl_tpl_type;
		port_tbl.BF.acl_rule_icmp = port_state->icmp_tpl_type;
		port_tbl.BF.acl_rule_icmpv6 = port_state->icmpv6_tpl_type;
		port_tbl.BF.acl_rule_igmp = port_state->igmp_tpl_type;
	}

	REGISTER_WRITE(dev, SWCORE, SOURCE_PORT_TABLE, port_tbl, cport, 0);

	return JL_ERR_OK;
}


jl_ret_t jl61xx_acl_source_port_get(jl_device_t *dev,
					jl_uint8 lport,
					jl_acl_port_t *port_state)
{
	jl_ret_t ret = JL_ERR_OK;
	jl_uint8 cport;

	JL_CHECK_POINTER(dev);
	JL_CHECK_POINTER(port_state);
	JL_CHECK_PORT(dev, lport);

	cport = jlxx_port_l2c(dev, lport);

	REGISTER_READ(dev, SWCORE, SOURCE_PORT_TABLE, port_tbl, cport, 0);

	memset(port_state, 0, sizeof(jl_acl_port_t));

	if (port_tbl.BF.use_acl == 0)
		port_state->enable = DISABLED;
	else
		port_state->enable = ENABLED;

	port_state->acl_tpl_type = port_tbl.BF.acl_rule;
	port_state->icmp_tpl_type = port_tbl.BF.acl_rule_icmp;
	port_state->icmpv6_tpl_type = port_tbl.BF.acl_rule_icmpv6;
	port_state->igmp_tpl_type= port_tbl.BF.acl_rule_igmp;
	port_state->valid = TRUE;

	return JL_ERR_OK;
}

jl_ret_t jl61xx_acl_init(jl_device_t *dev)
{
	JL_CHECK_POINTER(dev);

	if (gp_acl_template[dev->id] == NULL)
	{
		gp_acl_template[dev->id] = (jl61xx_acl_template_t*)port_mem_malloc(sizeof(jl61xx_acl_template_t));

		if (!gp_acl_template[dev->id])
			return JL_ERR_MEMORY;

		memset((void*)gp_acl_template[dev->id], 0, sizeof(jl61xx_acl_template_t));
	}

	if (gp_acl_rule_info[dev->id] == NULL)
	{
		gp_acl_rule_info[dev->id] = (jl61xx_acl_rule_info_t*)port_mem_malloc(sizeof(jl61xx_acl_rule_info_t));

		if (!gp_acl_rule_info[dev->id])
			return JL_ERR_MEMORY;

		memset((void*)gp_acl_rule_info[dev->id], 0, sizeof(jl61xx_acl_rule_info_t));
	}

	return JL_ERR_OK;
}

jl_ret_t jl61xx_acl_deinit(jl_device_t *dev)
{
	JL_CHECK_POINTER(dev);

	if (gp_acl_template[dev->id])
		port_mem_free(gp_acl_template[dev->id]);

	gp_acl_template[dev->id] = NULL;

	if (gp_acl_rule_info[dev->id])
		port_mem_free(gp_acl_rule_info[dev->id]);

	gp_acl_rule_info[dev->id] = NULL;

	return JL_ERR_OK;
}

static jl_ret_t jl61xx_acl_split_add_data(jl_uint32 val, 
											jl_uint32 mask,
											jl61xx_acl_split_data_t* pdata)
{
	jl_acl_field_data_t* pentry =  NULL;
	jl_uint32 fmask;

	pentry = (jl_acl_field_data_t*)port_mem_malloc(sizeof(jl_acl_field_data_t));
	if (pentry == NULL)
		return JL_ERR_MEMORY;

	memset(pentry, 0, sizeof(jl_acl_field_data_t));
	
	fmask = BITS_WIDTH(jl61xx_acl_field_bit_size[pdata->entry_type]);
	
	pentry->field_type = pdata->entry_type;
	pentry->field.val_data.type = ACL_DATA_SPECIFIC_VALUE;
	pentry->field.val_data.value = val;
	pentry->field.val_data.mask = mask & fmask;

	if (pentry->field.val_data.mask != fmask)
		pentry->tcam_flag = 1;

	if (pdata->entry_head == NULL) {
		pdata->entry_head = pentry;
	}
	else
		pdata->entry_end->next = pentry;

	pdata->entry_end = pentry;

	if (pentry->tcam_flag)
		pdata->tcam_entry_num++;
	else
		pdata->hash_entry_num++;
	
	return JL_ERR_OK;
}

static void jl61xx_acl_split_clear_data(jl61xx_acl_split_data_t* pdata)
{	
	jl_acl_field_data_t* pentry_temp = NULL;
	jl_acl_field_data_t* pentry =  pdata->entry_head;

	while (pentry)
	{
		pentry_temp = pentry->next;
		port_mem_free(pentry);
		pentry = pentry_temp;
	}
	memset(pdata, 0, sizeof(jl61xx_acl_split_data_t));
	return;
}

static void jl61xx_acl_split_get_data_width(jl_uint32 val, jl_uint8 *pwidth)
{
	jl_uint32 temp = 1;
	jl_uint8 width = 0;

	while (val >= temp) {
		temp <<= 1;
		if (++width == 32)
			break;
	}

	*pwidth = width;
}

static jl_ret_t jl61xx_acl_split_get_list(jl61xx_acl_split_val_t *val, jl61xx_acl_split_data_t* rule) 
{
	jl_ret_t ret = JL_ERR_OK;
	jl_uint32 val_ex = 0;
	jl_uint32 mask = 0;
	jl_uint32 temp_h = 0;
	jl_uint32 temp_l = 0;
	jl_uint8 full_h = 0;
	jl_uint8 width_h = val->width_h;
	jl_uint8 width_l = val->width_l;

	jl61xx_acl_split_val_t temp = {0};
	mask = val->mask;

	/*specific value, add rule full masked*/
	if (val->val_l == val->val_h) {
		return	jl61xx_acl_split_add_data(val->val_l | val->val_ex, mask, rule);
	}

	if (val->val_h == BITS_WIDTH(width_h))
		full_h = 1;

	if (width_l == width_h) //when the bit widths are equal
	{
		/*pick up the continuous same bits, then recursion*/
		while (GET_BIT(val->val_h, width_h-- - 1) == GET_BIT(val->val_l, width_l-- - 1))
		{
			if (GET_BIT(val->val_h, width_h))
				val_ex |= BIT(width_h);
		}

		temp = *val;
		temp.val_h = val->val_h & (~val_ex);
		temp.val_l = val->val_l & (~val_ex);
		temp.val_ex = val->val_ex + val_ex;

		jl61xx_acl_split_get_data_width(temp.val_h, &(temp.width_h));
		jl61xx_acl_split_get_data_width(temp.val_l, &(temp.width_l));

		ret = jl61xx_acl_split_get_list(&temp, rule);
		if (ret)
			return ret;
	}
	else //when not equal
	{
		if (val->val_l == 0)
		{
			if (full_h == 1)
			{
				CLR_BITS(mask, 0, width_h - 1);
				return	jl61xx_acl_split_add_data(val->val_ex, mask, rule);
			}

			CLR_BITS(mask, 0, width_h - 2);
			ret = jl61xx_acl_split_add_data(val->val_ex, mask, rule);
			if (ret)
				return ret;

			temp = *val;
			temp.val_l = BIT(width_h - 1);
			temp.width_l = width_h;

			ret = jl61xx_acl_split_get_list(&temp, rule);
			if (ret)
				return ret;
		}
		else
		{
			temp_h = BIT(width_h - 1);
			if (width_l == 0)
				temp_l = 0;
			else
				temp_l = BITS_WIDTH(width_l);

			/*higher recursion*/
			temp = *val;
			temp.val_l = temp_h;
			temp.width_l = width_h;
			/*to equaled bit width flow*/
			ret = jl61xx_acl_split_get_list(&temp, rule);
			if (ret)
				return ret;

			/*middle recursion*/
			if ((width_h - width_l) > 1)
			{
				temp = *val;
				temp.val_h = temp_h - 1;
				temp.val_l = temp_l + 1;
				temp.width_h = width_h - 1;
				temp.width_l = width_l + 1;

				/*to equal or not equal bit width flow*/
				ret = jl61xx_acl_split_get_list(&temp, rule);
				if (ret)
					return ret;
			}

			/*lower recursion*/
			temp = *val;
			temp.val_h = temp_l;
			temp.width_h = width_l;
			/*to equaled bit width flow*/
			ret = jl61xx_acl_split_get_list(&temp, rule);
			if (ret)
				return ret;
		}
	}
	return JL_ERR_OK;
}