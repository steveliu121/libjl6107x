/**
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

#ifndef __JL6107X_DRV_QOS_H__
#define __JL6107X_DRV_QOS_H__

#include "jl_base.h"
#include "jl_qos.h"

#ifdef __cplusplus
extern "C" {
#endif

#define QOS_IFG_DEFAULT_LENGTH (20)
#define QOS_FCS_DEFAULT_LENGTH (4)
#define QOS_CPU_PORT		   (7)
#define QOS_PORT_MAX		   (8)

#define QOS_MIN_MMP_CAPACITY (0xfff)/**< QOS_MIN_MMP_CAPACITY */

#define QOS_BUCKET_THRESHOLD_MULTIPLE_PPS  (1)/**< QOS_BUCKET_THRESHOLD_MULTIPLE_PPS */
#define QOS_BUCKET_THRESHOLD_MULTIPLE_BPS  (5)/**< QOS_BUCKET_THRESHOLD_MULTIPLE_BPS */
#define QOS_BUCKET_BURSTSIZE_MULTIPLE_PPS  (2)/**< QOS_BUCKET_BURSTSIZE_MULTIPLE_PPS */
#define QOS_BUCKET_BURSTSIZE_MULTIPLE_BPS  (2)/**< QOS_BUCKET_BURSTSIZE_MULTIPLE_BPS */
#define QOS_BUCKET_MINDEPTH_VAL_BPS  (2000)/**< QOS_BUCKET_MINDEPTH_VAL_BPS */

#define QOS_RESOURCE_TOTAL_CELLS (912)/**< QOS_RESOURCE_TOTAL_CELLS */
#define QOS_DEFAULT_GUARANTEE_CELLS (23)/**< QOS_DEFAULT_GUARANTEE_CELLS */

#define QOS_MULTICAST_RES_MANAGEMENT_ID (8) /**< QOS_MULTICAST_RES_MANAGEMENT_ID */
#define QOS_RESOURCE_MANAGEMENT_ID_NUM (QOS_MULTICAST_RES_MANAGEMENT_ID + 1)/**< QOS_RESOURCE_MANAGEMENT_ID_NUM */
#define QOS_RESOURCE_MANAGEMENT_POINTER_NUM (4)/**< QOS_RESOURCE_MANAGEMENT_POINTER_NUM */

#define QOS_RES_LIMITER_XON (400)/**< QOS_RES_LIMITER_XON */

typedef struct jl6107x_qos_res_cfg_s {
	jl_qos_queue_res_mode_t mode;
	jl_uint8 ptr[QOS_QUEUE_NUM]; /**< Resource management pointer */
	jl_uint8 hard_ptr; /**< Hard limiter pointer */
} jl6107x_qos_res_cfg_t;

typedef struct jl6107x_qos_res_management_s {
	jl_uint8 res_init;
	jl_uint8 lport_num;
	jl_uint8 queue_num;
	jl_uint8 enqueue_mask[QOS_PORT_MAX]; /**< Queue enable state on each chip port */
	jl_uint16 global_margin; /**< Global margin resource */
	jl_uint16 ffa_margin; /**< FFA margin resource */
	jl_uint16 guarantee[QOS_RESOURCE_MANAGEMENT_POINTER_NUM]; /**< Guarantee resource parameter */
	jl_uint16 limit_xon[QOS_RESOURCE_MANAGEMENT_POINTER_NUM]; /**< Limiter resource XON parameter */
	jl_uint16 limit_trd[QOS_RESOURCE_MANAGEMENT_POINTER_NUM]; /**< Limiter resource threshold parameter */
	jl6107x_qos_res_cfg_t res_cfg[QOS_RESOURCE_MANAGEMENT_ID_NUM]; /**< Resource management config */
} jl6107x_qos_res_management_t;

jl_ret_t jl6107x_qos_map_2queue_nonvlan_set(jl_device_t *dev,
        jl_uint8 eport,
        jl_uint8 queue);

jl_ret_t jl6107x_qos_map_2queue_nonvlan_get(jl_device_t *dev,
        jl_uint8 eport,
        jl_uint8 *pqueue);

jl_ret_t jl6107x_qos_map_2queue_unknown_set(jl_device_t *dev,
        jl_uint8 eport,
        jl_uint8 queue);

jl_ret_t jl6107x_qos_map_2queue_unknown_get(jl_device_t *dev,
        jl_uint8 eport,
        jl_uint8 *pqueue);

jl_ret_t jl6107x_qos_map_2queue_mac_set(jl_device_t *dev,
		jl_uint8 eport,
		jl_uint8 rule_idx,
		jl_qos_mac_map_t *pmap);

jl_ret_t jl6107x_qos_map_2queue_mac_get(jl_device_t *dev,
		jl_uint8 eport,
		jl_uint8 rule_idx,
		jl_qos_mac_map_t *pmap);

jl_ret_t jl6107x_qos_map_2queue_vid_set(jl_device_t *dev,
		jl_uint8 eport,
		jl_uint8 rule_idx,
		jl_qos_vid_map_t *pmap);

jl_ret_t jl6107x_qos_map_2queue_vid_get(jl_device_t *dev,
		jl_uint8 eport,
		jl_uint8 rule_idx,
		jl_qos_vid_map_t *pmap);

jl_ret_t jl6107x_qos_map_2queue_ip_set(jl_device_t *dev,
		jl_uint8 eport,
		jl_uint8 rule_idx,
		jl_qos_ip_map_t *pmap);

jl_ret_t jl6107x_qos_map_2queue_ip_get(jl_device_t *dev,
		jl_uint8 eport,
		jl_uint8 rule_idx,
		jl_qos_ip_map_t *pmap);

jl_ret_t jl6107x_qos_map_2queue_ethtype_set(jl_device_t *dev,
        jl_uint8 eport,
        jl_uint8 rule_idx,
        jl_qos_ethtype_map_t *pmap);

jl_ret_t jl6107x_qos_map_2queue_ethtype_get(jl_device_t *dev,
        jl_uint8 eport,
        jl_uint8 rule_idx,
        jl_qos_ethtype_map_t *pmap);

jl_ret_t jl6107x_qos_map_2queue_tos_set(jl_device_t *dev,
		jl_uint8 eport,
		jl_uint16 tos,
		jl_uint8 queue);

jl_ret_t jl6107x_qos_map_2queue_tos_get(jl_device_t *dev,
		jl_uint8 eport,
		jl_uint16 tos,
		jl_uint8 *pqueue);

jl_ret_t jl6107x_qos_map_2queue_exp_set(jl_device_t *dev,
		jl_uint8 eport,
		jl_uint8 exp,
		jl_uint8 queue);

jl_ret_t jl6107x_qos_map_2queue_exp_get(jl_device_t *dev,
		jl_uint8 eport,
		jl_uint8 exp,
		jl_uint8 *pqueue);

jl_ret_t jl6107x_qos_map_2queue_pcp_set(jl_device_t *dev,
		jl_uint8 eport,
		jl_uint8 pcp,
		jl_uint8 queue);

jl_ret_t jl6107x_qos_map_2queue_pcp_get(jl_device_t *dev,
		jl_uint8 eport,
		jl_uint8 pcp,
		jl_uint8 *pqueue);

jl_ret_t jl6107x_qos_map_2queue_l4port_range_set(jl_device_t *dev,
        jl_uint8 eport,
        jl_uint8 rule_idx,
        jl_qos_l4_port_range_map_t *pmap);

jl_ret_t jl6107x_qos_map_2queue_l4port_range_get(jl_device_t *dev,
        jl_uint8 eport,
        jl_uint8 rule_idx,
        jl_qos_l4_port_range_map_t *pmap);

jl_ret_t jl6107x_qos_map_2queue_l4_protocol_set(jl_device_t *dev,
        jl_uint8 eport,
        jl_uint8 rule_idx,
        jl_qos_l4_protocol_map_t *pmap);

jl_ret_t jl6107x_qos_map_2queue_l4_protocol_get(jl_device_t *dev,
        jl_uint8 eport,
        jl_uint8 rule_idx,
        jl_qos_l4_protocol_map_t *pmap);

jl_ret_t jl6107x_qos_map_2color_nonvlan_set(jl_device_t *dev,
        jl_uint8 color,
        jl_uint8 enable);

jl_ret_t jl6107x_qos_map_2color_nonvlan_get(jl_device_t *dev,
        jl_uint8 *pcolor,
        jl_uint8 *penable);

jl_ret_t jl6107x_qos_map_2color_unknown_set(jl_device_t *dev,
        jl_uint8 color,
        jl_uint8 enable);

jl_ret_t jl6107x_qos_map_2color_unknown_get(jl_device_t *dev,
        jl_uint8 *pcolor,
        jl_uint8 *penable);

jl_ret_t jl6107x_qos_map_2color_tos_set(jl_device_t *dev,
		jl_uint8 tos,
		jl_uint8 color);

jl_ret_t jl6107x_qos_map_2color_tos_get(jl_device_t *dev,
		jl_uint8 tos,
		jl_uint8 *pcolor);

jl_ret_t jl6107x_qos_map_2color_exp_set(jl_device_t *dev,
		jl_uint8 exp,
		jl_uint8 color);

jl_ret_t jl6107x_qos_map_2color_exp_get(jl_device_t *dev,
		jl_uint8 exp,
		jl_uint8 *pcolor);

jl_ret_t jl6107x_qos_map_2color_pcpdei_set(jl_device_t *dev,
        jl_uint8 pcp,
        jl_uint8 dei,
        jl_uint8 color);

jl_ret_t jl6107x_qos_map_2color_pcpdei_get(jl_device_t *dev,
        jl_uint8 pcp,
        jl_uint8 dei,
        jl_uint8 *pcolor);

jl_ret_t jl6107x_qos_remap_vlan_set(jl_device_t *dev,
		jl_uint8 eport,
		jl_qos_remap_vlan_t *pcfg);

jl_ret_t jl6107x_qos_remap_vlan_get(jl_device_t *dev,
		jl_uint8 eport,
		jl_qos_remap_vlan_t *pcfg);

jl_ret_t jl6107x_qos_color_remap_set(jl_device_t *dev,
		jl_uint8 eport,
		jl_qos_color_remap_t *pcfg);

jl_ret_t jl6107x_qos_color_remap_get(jl_device_t *dev,
		jl_uint8 eport,
		jl_qos_color_remap_t *pcfg);

jl_ret_t jl6107x_qos_mmp_cfg_set(jl_device_t *dev,
		jl_uint8 mmp_pointer,
		jl_qos_mmp_cfg_t *pcfg);

jl_ret_t jl6107x_qos_mmp_cfg_get(jl_device_t *dev,
		jl_uint8 mmp_pointer,
		jl_qos_mmp_cfg_t *pcfg);

jl_ret_t jl6107x_qos_mmp_unused_pointer_get(jl_device_t *dev, jl_uint8 *ppointer);

jl_ret_t jl6107x_qos_mmp_used_pointer_info_get(jl_device_t *dev,
        jl_uint8 mmp_pointer,
        jl_qos_mmp_used_info_t *ppointer_info);

jl_ret_t jl6107x_qos_ingress_initial_mmp_set(jl_device_t *dev,
        jl_uint8 inport,
        jl_uint8 pcp,
        jl_qos_mmp_pointer_t *ppointer);

jl_ret_t jl6107x_qos_ingress_initial_mmp_get(jl_device_t *dev,
        jl_uint8 inport,
        jl_uint8 pcp,
        jl_qos_mmp_pointer_t *ppointer);

jl_ret_t jl6107x_qos_queue_map_trust_l3_set(jl_device_t *dev,
        jl_uint8 eport,
        jl_uint8 enable);

jl_ret_t jl6107x_qos_queue_map_trust_l3_get(jl_device_t *dev,
        jl_uint8 eport,
        jl_uint8 *penable);

jl_ret_t jl6107x_qos_color_map_trust_l3_set(jl_device_t *dev,
        jl_uint8 inport,
        jl_uint8 enable);

jl_ret_t jl6107x_qos_color_map_trust_l3_get(jl_device_t *dev,
        jl_uint8 inport,
        jl_uint8 *penable);

jl_ret_t jl6107x_qos_queue_priority_set(jl_device_t *dev,
		jl_uint8 eport,
		jl_qos_queue_priority_t *pqueue_pri);

jl_ret_t jl6107x_qos_queue_priority_get(jl_device_t *dev,
		jl_uint8 eport,
		jl_qos_queue_priority_t *pqueue_pri);

jl_ret_t jl6107x_qos_queue_shaper_set(jl_device_t *dev,
		jl_uint8 eport,
		jl_qos_queue_shaper_t *pshaper);

jl_ret_t jl6107x_qos_queue_shaper_get(jl_device_t *dev,
		jl_uint8 eport,
		jl_qos_queue_shaper_t *pshaper);

jl_ret_t jl6107x_qos_prio_shaper_set(jl_device_t *dev,
		jl_uint8 eport,
		jl_qos_priority_shaper_t *pshaper);

jl_ret_t jl6107x_qos_prio_shaper_get(jl_device_t *dev,
		jl_uint8 eport,
		jl_qos_priority_shaper_t *pshaper);

jl_ret_t jl6107x_qos_queue_dwrr_set(jl_device_t *dev,
		jl_uint8 eport,
		jl_qos_queue_dwrr_t *pdwrr);

jl_ret_t jl6107x_qos_queue_dwrr_get(jl_device_t *dev,
		jl_uint8 eport,
		jl_qos_queue_dwrr_t *pdwrr);

jl_ret_t jl6107x_qos_port_shaper_set(jl_device_t *dev,
		jl_uint8 eport,
		jl_qos_port_shaper_t *pshaper);

jl_ret_t jl6107x_qos_port_shaper_get(jl_device_t *dev,
		jl_uint8 eport,
		jl_qos_port_shaper_t *pshaper);
									 
jl_ret_t jl6107x_qos_enqueue_enable_set(jl_device_t *dev,
		jl_uint8 eport,
		jl_qos_enqueue_state_t *pstate);

jl_ret_t jl6107x_qos_enqueue_enable_get(jl_device_t *dev,
		jl_uint8 eport,
		jl_qos_enqueue_state_t *pstate);

jl_ret_t jl6107x_qos_res_limiter_get(jl_device_t *dev,
        jl_uint8 pointer,
        jl_uint16 *pxon,
		jl_uint16 *pthreshold);

jl_ret_t jl6107x_qos_schedule_set(jl_device_t *dev, jl_uint8 eport, jl_qos_schedule_t *psche);

jl_ret_t jl6107x_qos_schedule_get(jl_device_t *dev, jl_uint8 eport, jl_qos_schedule_t *psche);

jl_ret_t jl6107x_qos_init(jl_device_t *dev);

jl_ret_t jl6107x_qos_deinit(jl_device_t *dev);

jl_ret_t jl6107x_qos_resouce_init(jl_device_t *dev, jl_uint8 queue_num);

jl_ret_t jl6107x_qos_resouce_deinit(jl_device_t *dev);


#ifdef __cplusplus
}
#endif

#endif /* __JL6107X_DRV_QOS_H__ */
