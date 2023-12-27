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

#ifndef __JL6107X_DRV_ACL_H__
#define __JL6107X_DRV_ACL_H__

#include "jl_base.h"
#include "jl_acl.h"

#ifdef __cplusplus
extern "C" {
#endif

#define JL6107X_ACL_TCAM_NUM (32)
#define JL6107X_ACL_LARGE_NUM (64)
#define JL6107X_ACL_SMALL_NUM (32)

typedef struct jl6107x_mac_entry_info_s {
	jl_uint8 mmp_valid;
	jl_uint8 mmp_index;
} jl6107x_mac_entry_info_t;

typedef struct jl6107x_acl_rule_entry_s {
	jl_uint8 table;
	jl_uint8 index;
	struct jl6107x_acl_rule_entry_s *next;
	struct jl6107x_acl_rule_entry_s *end;
} jl6107x_acl_rule_entry_t;

typedef struct jl6107x_acl_entry_info_s {
	jl_uint8 valid;
	jl_uint8 rule_templ;
	jl_uint8 field_num;
	jl_uint8 field_mask;
	jl_uint8 counter_valid;
	jl_uint8 counter_index;
	jl_uint8 mmp_valid;
	jl_uint8 mmp_index;
	jl6107x_acl_rule_entry_t *rule_entry;
} jl6107x_acl_entry_info_t;

typedef struct jl6107x_acl_rule_info_s {
	jl_uint8 used_large_num;
	jl_uint8 used_small_num;
	jl_uint8 used_tcam_num;
	jl_uint64 entry_mask_large;
	jl_uint32 entry_mask_small;
	jl_uint32 entry_mask_tcam;
	jl_uint8 counter[ACL_MATCH_COUNTER];
	jl6107x_acl_entry_info_t acl_rule[ACL_TBL_RULE_NUM];   // with index used by user
	jl6107x_mac_entry_info_t smac_rule[ACL_MAC_RULE_NUM];
	jl6107x_mac_entry_info_t dmac_rule[ACL_MAC_RULE_NUM];
} jl6107x_acl_rule_info_t;

typedef struct jl6107x_acl_val128_s {
	jl_uint64 val_0_63;
	jl_uint64 val_64_127;
	jl_uint64 mask_0_63;
	jl_uint64 mask_64_127;
} jl6107x_acl_val128_t;

typedef struct jl6107x_acl_hash_key_s {
	jl_uint8 value[26];
	jl_uint8 mask[26];
	jl_uint8 tcam_flag;
} jl6107x_acl_hash_key_t;

typedef struct jl6107x_template_s {
	jl_uint8 rule_pointer_mask;
	jl_uint8 templ_mask[ACL_TEMPLATE_NUM];
	jl_acl_field_type_t acl_templ[ACL_TEMPLATE_NUM][ACL_RULE_FIELD_NUM];
} jl6107x_acl_template_t;

typedef struct jl6107x_acl_split_val_s {
	jl_uint32 val_l; //lower value
	jl_uint32 val_h; //higher value
	jl_uint32 val_ex; //external value
	jl_uint32 mask; //full masked value
	jl_uint8 width_l; //lower value bit width
	jl_uint8 width_h; //higher value bit width
} jl6107x_acl_split_val_t;

typedef struct jl6107x_acl_split_data_s {
	jl_acl_field_type_t entry_type;
	jl_uint8 templ_idx;
	jl_uint8 reg_offset;
	jl_uint8 tcam_entry_num;
	jl_uint8 hash_entry_num;
	jl_acl_field_data_t *entry_head;
	jl_acl_field_data_t *entry_end;
	struct jl6107x_acl_split_data_s* next;
} jl6107x_acl_split_data_t;

typedef struct jl6107x_acl_data_s {
	jl_uint8 templ_idx;
	jl_uint8 field_num;
	jl_uint8 field_mask;
	jl_uint8 tcam_flag;
	jl_acl_field_data_t* origin_data; /* input rule data, malloc & free by user*/
	jl6107x_acl_split_data_t* split_data; /* used for spliting rules, free in the end*/
	jl6107x_acl_rule_entry_t *rule_entry; /*used for recording info, free in info clear */
} jl6107x_acl_data_t;

typedef struct jl6107x_acl_key_list_s {
	jl6107x_acl_hash_key_t key;
	struct jl6107x_acl_key_list_s *next;
	struct jl6107x_acl_key_list_s *end;
} jl6107x_acl_key_list_t;

jl_ret_t jl6107x_acl_template_set(jl_device_t *dev, jl_uint8 index, jl_acl_template_t *acl_templ);
jl_ret_t jl6107x_acl_template_get(jl_device_t *dev, jl_uint8 index, jl_acl_template_t *acl_templ);

jl_ret_t jl6107x_acl_rule_add(jl_device_t *dev, jl_uint8 index, jl_acl_rule_t *acl_rule,jl_acl_action_t *acl_action);
jl_ret_t jl6107x_acl_rule_get(jl_device_t *dev, jl_uint8 index, jl_acl_rule_t *acl_rule,jl_acl_action_t *acl_action);
jl_ret_t jl6107x_acl_rule_del(jl_device_t *dev, jl_uint8 index);

jl_ret_t jl6107x_acl_field_insert(jl_device_t *dev, jl_acl_field_data_t *acl_field, jl_acl_rule_t *acl_rule);
jl_ret_t jl6107x_acl_field_clear(jl_device_t *dev, jl_acl_rule_t *acl_rule);													

jl_ret_t jl6107x_acl_selection_set(jl_device_t *dev, jl_uint8 val);
jl_ret_t jl6107x_acl_selection_get(jl_device_t *dev, jl_uint8 *val);

jl_ret_t jl6107x_acl_stat_enable(jl_device_t *dev, jl_uint8 index, jl_uint8 counter);
jl_ret_t jl6107x_acl_stat_disable(jl_device_t *dev, jl_uint8 index);
jl_ret_t jl6107x_acl_stat_read(jl_device_t *dev, jl_uint8 counter, jl_uint32 *ppackets);
jl_ret_t jl6107x_acl_stat_write(jl_device_t *dev, jl_uint8 counter, jl_uint32 packets);
jl_ret_t jl6107x_acl_stat_counter_get(jl_device_t *dev, jl_uint8 *counter);

jl_ret_t jl6107x_acl_mac_rule_add(jl_device_t *dev, jl_uint8 index, 
										jl_acl_mac_range_t *mac_range, jl_acl_mac_action_t *mac_action);
jl_ret_t jl6107x_acl_mac_rule_get(jl_device_t *dev, jl_uint8 index, jl_acl_mac_type_t mac_type, 
										jl_acl_mac_range_t *mac_range, jl_acl_mac_action_t *mac_action);
jl_ret_t jl6107x_acl_mac_rule_del(jl_device_t *dev, jl_uint8 index, jl_acl_mac_type_t mac_type);

jl_ret_t jl6107x_acl_source_port_set(jl_device_t *dev, jl_uint8 inport, jl_acl_port_t *port_templ);
jl_ret_t jl6107x_acl_source_port_get(jl_device_t *dev, jl_uint8 inport, jl_acl_port_t *port_templ);

jl_ret_t jl6107x_acl_init(jl_device_t *dev);
jl_ret_t jl6107x_acl_deinit(jl_device_t *dev);


#ifdef __cplusplus
}
#endif

#endif /* __JL6107X_DRV_ACL_H__ */
