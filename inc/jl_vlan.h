/**
 * @file jl_vlan.h
 * @brief vlan
 */

/**
 * @defgroup vlan_group Vlan
 * @{
 */

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

#ifndef __JL_VLAN_H__
#define __JL_VLAN_H__
#include "jl_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @example example_vlan.c
 */


/****Macro Define****/
#define VLAN_MIN_VID_VALUE          (1)     /*!< support min VID value */
#define VLAN_MAX_VID_VALUE          (4095)  /*!< support max VID value */
#define VLAN_MAX_PCP_VAL            (7)     /*!< support max PCP value */
#define VLAN_MAX_CFI_DEI_VAL        (1)     /*!< support max CFI value */
#define VLAN_MAX_TPID_VAL           (0xffff)/*!< support max TPID value */

/**
 * @enum JL_SPORT_VLAN_ASSIGN_E
 * @brief Control how a packets Ingress VID is assigned
 * @note The default value is VLAN_ASSIGN_PKT.\n If vlan_assignment is VLAN_ASSIGN_PKT and the incoming packet doesn't have that header, then
 * this table entry's default Vid will be used.
 */
/**
 * @public typedef jl_sport_vlan_assign_t
 */
typedef enum JL_SPORT_VLAN_ASSIGN_E {
	JL_SPORT_VLAN_ASSIGN_PKT = 0, /**< 0 */
	JL_SPORT_VLAN_ASSIGN_PORT, /**< 1 */
	JL_SPORT_VLAN_ASSIGN_MIXED, /**< 2 */
	JL_SPORT_VLAN_ASSIGN_END, /**< 3 */
} jl_sport_vlan_assign_t;


/**
 * @enum JL_VLAN_OPERATION_E
 * @brief A number of operations that can be performed on the packet's VLAN headers such as push/pop etc
 */
/**
 * @public typedef jl_vlan_operation_t
 */
typedef enum JL_VLAN_OPERATION_E {
	JL_VLAN_OPERATION_NONE, /**< 0 */
	JL_VLAN_OPERATION_SWAP, /**< 1 */
	JL_VLAN_OPERATION_PUSH, /**< 2 */
	JL_VLAN_OPERATION_POP, /**< 3 */
	JL_VLAN_OPERATION_REMOVE_ALL, /**< 4 */
	JL_VLAN_OPERATION_END, /**< 5 */
} jl_vlan_operation_t;

/**
 * @enum JL_VLAN_VAL_SEL_E
 * @brief To confirm which VLAN layers will be used as standard.\n If selected VLAN layer does not exist, the table entry's value will be used as default.
 */
/**
 * @public typedef jl_vlan_val_sel_t
 */
typedef enum JL_VLAN_VAL_SEL_E {
	JL_VLAN_VAL_SEL_OUTER, /**< 0 */
	JL_VLAN_VAL_SEL_TBL, /**< 1 */
	JL_VLAN_VAL_SEL_SECVLAN, /**< 2 */
	JL_VLAN_VAL_SEL_END, /**< 3 */
} jl_vlan_val_sel_t;



/**
 * @enum JL_VLAN_TPID_SEL_E
 * @brief Select which TPID to use when building a new VLAN header in push or swap operation.
 */
/**
 * @public typedef jl_vlan_tpid_sel_t
 */
typedef enum JL_VLAN_TPID_SEL_E {
	JL_VLAN_TPID_SEL_8100, /**< 0 */
	JL_VLAN_TPID_SEL_88A8, /**< 1 */
	JL_VLAN_TPID_SEL_EGPORT_ETHTYPE, /**< 1 */
	JL_VLAN_TPID_SEL_END, /**< 2 */
} jl_vlan_tpid_sel_t;

/**
 * @enum JL_VLAN_SEL_OPERATION_E
 * @brief Select ingress port/vlantable/egress port VLAN operation.
 */
/**
 * @public typedef jl_vlan_sel_operation_t
 */
typedef enum JL_VLAN_SEL_OPERATION_E {
	JL_VLAN_SEL_OPERATION_INGRESS, /**< 0 */
	JL_VLAN_SEL_OPERATION_VLANTBL, /**< 1 */
	JL_VLAN_SEL_OPERATION_EGRESS, /**< 2 */
	JL_VLAN_SEL_OPERATION_END, /**< 3 */
} jl_vlan_sel_operation_t;

/****Type Define****/
/**
 * @struct jl_vlan_macbase_tbl_s
 * @brief For VLAN preprocessing, redirect packets to the specified VLAN table according to the MAC address range.
 * @note Assign Ingress VID from other packet field ranges:
 * - If order not same, the one with the highest order wins.\n
 * - If order same, use as packet parser order, like as follows:
 *    source port-->>mac -->>outer--->>inner--->>etype
 */
/**
 * @public typedef jl_vlan_macbase_tbl_t
 */
typedef struct jl_vlan_macbase_tbl_s {
	jl_uint32       portmsk; /**< 0 */
#define     JL_VLAN_MACBASED_SMAC       0 /*!< MAC based mode: SMAC */
#define     JL_VLAN_MACBASED_DMAC       1 /*!< MAC based mode: DMAC */
	jl_uint32       mode; /**< 1 */
	jl_uint32       vid; /**< 2 */
	jl_uint32       order; /**< 3 */
	jl_mac_addr_t   mac_range_start; /**< 4 */
	jl_mac_addr_t   mac_range_end; /**< 5 */
} jl_vlan_macbase_tbl_t;

/**
 * @struct jl_vlan_outvidbase_tbl_s
 * @brief For VLAN preprocessing, redirect packets to the specified VLAN table according to the outer VID
 * @note Assign Ingress VID from other packet field ranges:
 * - If order not same, the one with the highest order wins.\n
 * - If order same, use as packet parser order, like as follows:
 *    source port-->>mac -->>outer--->>inner--->>etype
 */
/**
 * @public typedef jl_vlan_outvidbase_tbl_t
 */
typedef struct jl_vlan_outvidbase_tbl_s {
	jl_uint32   portmsk; /**< 0 */
	jl_uint32   mode; /**< 1 */
#define     JL_VLAN_OUTVID_CTYPE        0 /*!< VID based mode: CTYPE */
#define     JL_VLAN_OUTVID_STYPE        1 /*!< VID based mode: STYPE */
	jl_uint32   checkvidstart; /**< 2 */
	jl_uint32   checkvidend; /**< 3 */
	jl_uint32   vid; /**< 4 */
	jl_uint8    order; /**< 5 */
} jl_vlan_outvidbase_tbl_t;

/**
 * @struct jl_vlan_invidbase_tbl_s
 * @brief For VLAN preprocessing, redirect packets to the specified VLAN table according to the inner VID
 * @note Assign Ingress VID from other packet field ranges:
 * - If order not same, the one with the highest order wins.\n
 * - If order same, use as packet parser order, like as follows:
 *    source port-->>mac -->>outer--->>inner--->>etype
 */
/**
 * @public typedef jl_vlan_invidbase_tbl_t
 */
typedef struct jl_vlan_invidbase_tbl_s {
	jl_uint32   portmsk; /**< 0 */
	jl_uint32   mode; /**< 1 */
#define     JL_VLAN_INVID_CTYPE        0 /*!< VID based mode: CTYPE */
#define     JL_VLAN_INVID_STYPE        1 /*!< VID based mode: STYPE */
	jl_uint32   checkvidstart; /**< 2 */
	jl_uint32   checkvidend; /**< 3 */
	jl_uint32   vid; /**< 4 */
	jl_uint8    order; /**< 5 */
} jl_vlan_invidbase_tbl_t;

/**
 * @struct jl_vlan_ethertypebase_tbl_s
 * @brief For VLAN preprocessing, redirect packets to the specified VLAN table according to the ethertype
 * @note Assign Ingress VID from other packet field ranges:
 * - If order not same, the one with the highest order wins.\n
 * - If order same, use as packet parser order, like as follows:
 *    source port-->>mac -->>outer--->>inner--->>etype
 */
/**
 * @public typedef jl_vlan_ethertypebase_tbl_t
 */
typedef struct jl_vlan_ethertypebase_tbl_s {
	jl_uint32   portmsk; /**< 0 */
	jl_uint16   ethertype_range_start; /**< 1 */
	jl_uint16   ethertype_range_end; /**< 2 */
	jl_uint32   vid; /**< 3 */
	jl_uint8    order; /**< 4 */
} jl_vlan_ethertypebase_tbl_t;


/**
 * @struct jl_vlan_tbl_s
 * @brief VLAN tbl setting
 * @note VLAN action processing order:\n
 *   source port&ingress preprocessing-->>acl-->>VLAN table--->egress port
 */
/**
 * @public typedef jl_vlan_tbl_t
 */
typedef struct jl_vlan_tbl_s {

	jl_uint32                   portmsk; /**< 0 */
	jl_vlan_operation_t         vlanopt; /**< 1 */
	jl_vlan_val_sel_t           vidsel; /**< 2 */
	jl_uint32                   vid; /**< 3 */
	jl_vlan_val_sel_t           cfisel; /**< 4 */
	jl_uint32                   cfi; /**< 5 */
	jl_vlan_val_sel_t           pcpsel; /**< 6 */
	jl_uint32                   pcp; /**< 7 */
	jl_vlan_tpid_sel_t          typesel; /**< 8 */
	jl_uint32                   fid; /**< 9 */
	jl_bool                     mmpvalid; /**< 10 */
	jl_uint8                    mmpptr; /**< 11 */
	jl_uint8                    mmporder; /**< 12 */
	jl_uint8                    msptptr; /**< 13 */
} jl_vlan_tbl_t;


/**
 * @brief Init Vlan
 * @details This function is used to initialize the vlan module.
 * @note 61XX chip supports functions below:
 * -# default Port VLAN setting
 * -# Pritag frame support.
 * -# Old standard service VLAN VID 9100 support
 * -# MSTP setting
 * @param chip_id
 * @return Init vlan successfully or not
 *      @retval JL_ERR_OK       Ok
 *      @retval Other           Fail
 * @see     jl_error.h
 */
JL_API jl_api_ret_t jl_vlan_init(
	const jl_uint32     chip_id
);

/**
 * @brief Deinit Vlan
 * @details This function will reset and disable VLAN module
 * @param chip_id
 * @return Deinit vlan successfully or not
 *      @retval JL_ERR_OK       Ok
 *      @retval Other           Fail
 */
JL_API jl_api_ret_t jl_vlan_deinit(
	const jl_uint32     chip_id
);

/**
 * @brief Set the VLAN assignment for the specified ingress port.
 * @details  This API is used to set ingress port (switch rx) VLAN operation assignment.
 * @param     chip_id
 * @param     sportidx
 * @param     opt     vlan layer setting
 * @param     vidsel  VID mode setting.
 * @param     cfisel  CFI mode setting.
 * @param     pcpsel  PCP mode setting.
 * @param     ethertype  VLAN TPID mode setting.
 * @param     mode   Control how a packets Ingress VID is assigned.
 * @return Set successfully or not
 *      @retval JL_ERR_OK       Ok
 *      @retval Other           Fail
 * @note If jl_sport_vlan_assign_t mode opt is JL_SPORT_VLAN_ASSIGN_PKT but the incoming packet doesn't have a vlan header, then
 * this table entry's default Vid will be used as Ingress VID.
 */
JL_API jl_api_ret_t jl_vlan_ingressport_mode_set(
	const jl_uint32                 chip_id,
	const jl_uint32                 sportidx,
	const jl_vlan_operation_t       opt,
	const jl_vlan_val_sel_t         vidsel,
	const jl_vlan_val_sel_t         cfisel,
	const jl_vlan_val_sel_t         pcpsel,
	const jl_vlan_tpid_sel_t        ethertype,
	const jl_sport_vlan_assign_t    mode
);

/**
 * @brief Set the VLAN default value for the specified ingress port.
 * @details  this API is used to set ingress port (switch rx) VLAN default value.
 * @param     chip_id
 * @param     sportidx
 * @param     default_vid Value for VLAN id value setting
 * @param     default_cfi Value for VLAN CFI value setting
 * @param     default_pcp Value for VLAN PCP value setting
 * @param     default_order Set priority for source port VLAN default value,
 * which will be compared with the priority of ingress preprocessing mode(MAC/OUTVID/INVID/ETHERTYPE).
 * @return Set successfully or not
 *      @retval JL_ERR_OK       Ok
 *      @retval Other           Fail
 * @note Order range from 0-3 for 61XX chip.
 */
JL_API jl_api_ret_t jl_vlan_ingressport_default_set(
	const jl_uint32         chip_id,
	const jl_uint32         sportidx,
	const jl_uint32         default_vid,
	const jl_uint32         default_cfi,
	const jl_uint32         default_pcp,
	const jl_uint8          default_order
);

/**
 * @brief Set the VLAN assignment for the specified egress port.
 * @details  This API is used to set egress port (switch tx) VLAN operation assignment.
 * @param     chip_id
 * @param     dportidx
 * @param     opt  vlan layer setting
 * @param     vidsel  VID mode setting.
 * @param     cfisel  CFI mode setting.
 * @param     pcpsel  PCP mode setting.
 * @param     ethertype   For VLAN TPID mode setting.
 * @return Set successfully or not
 *      @retval JL_ERR_OK       Ok
 *      @retval Other           Fail
 */
JL_API jl_api_ret_t jl_vlan_egressport_mode_set(
	const jl_uint32                 chip_id,
	const jl_uint32                 dportidx,
	const jl_vlan_operation_t       opt,
	const jl_vlan_val_sel_t         vidsel,
	const jl_vlan_val_sel_t         cfisel,
	const jl_vlan_val_sel_t         pcpsel,
	const jl_vlan_tpid_sel_t        ethertype
);

/**
 * @brief Set the VLAN default value for the specified egress port.
 * @details  This API is used to set egress port (switch tx) VLAN default value.
 * @param     chip_id
 * @param     dportidx
 * @param     default_vid Value for VLAN id value setting
 * @param     default_cfi Value for VLAN CFI value setting
 * @param     default_pcp Value for VLAN PCP value setting
 * @return Set successfully or not
 *      @retval JL_ERR_OK       Ok
 *      @retval Other           Fail
 */
JL_API jl_api_ret_t jl_vlan_egressport_default_set(
	const jl_uint32         chip_id,
	const jl_uint32         dportidx,
	const jl_uint32         default_vid,
	const jl_uint32         default_cfi,
	const jl_uint32         default_pcp
);


/**
 * @brief Set the ingress preprocessing VID Assignment from Packet Fields based on the L2 MAC layer.
 * @details Set the ingress preprocessing VLAN function -- MAC based VLAN mode value.
 * @param     chip_id
 * @param     idx    Entry idx
 * @param     pcfg   MAC config.
 * @return Set successfully or not
 *      @retval JL_ERR_OK       Ok
 *      @retval Other           Fail
 * @note
 * Please choice suitable order for this entry takes effect.
 */
JL_API jl_api_ret_t jl_vlan_preprocess_mac_set(
	const jl_uint32         chip_id,
	const jl_uint8          idx,
	jl_vlan_macbase_tbl_t  *pcfg
);

/**
 * @brief Get the ingress preprocessing VLAN function -- MAC based VLAN mode value
 * @param     chip_id
 * @param     idx    Entry idx
 * @param     pcfg   MAC config
 * @return Get successfully or not
 *      @retval JL_ERR_OK       Ok
 *      @retval Other           Fail
 */
JL_API jl_api_ret_t jl_vlan_preprocess_mac_get(
	const jl_uint32         chip_id,
	const jl_uint8          idx,
	jl_vlan_macbase_tbl_t  *pcfg
);

/**
 * @brief Set the ingress preprocessing VLAN function -- outer VLAN based mode value.
 * @details  This API will convert frame input VLAN id for VLAN table base on stream outer vlan VID range.
 * @param     chip_id
 * @param     idx   Entry idx
 * @param     pcfg  Outer VLAN config.
 * @return Set successfully or not
 *      @retval JL_ERR_OK       Ok
 *      @retval Other           Fail
 * @note
 * Please choice suitable order for this entry takes effect.
 */
JL_API jl_api_ret_t jl_vlan_preprocess_outvid_set(
	const jl_uint32             chip_id,
	const jl_uint8              idx,
	jl_vlan_outvidbase_tbl_t   *pcfg
);

/**
 * @brief Get the ingress preprocessing VLAN function -- outer VLAN based mode value.
 * @param     chip_id
 * @param     idx     Entry idx
 * @param     pcfg    Outer VLAN config.
 * @return Get successfully or not
 *      @retval JL_ERR_OK       Ok
 *      @retval Other           Fail
 */
JL_API jl_api_ret_t jl_vlan_preprocess_outvid_get(
	const jl_uint32             chip_id,
	const jl_uint8              idx,
	jl_vlan_outvidbase_tbl_t   *pcfg
);

/**
 * @brief Set the ingress preprocessing VLAN function -- inner VLAN based mode value.
 * @details  This API will convert frame input VLAN id for VLAN table base on stream inner vlan VID range.
 * @param     chip_id
 * @param     idx    Entry idx
 * @param     pcfg   Inner VLAN config.
 * @return Set successfully or not
 *      @retval JL_ERR_OK       Ok
 *      @retval Other           Fail
 * @note
 * Please choice suitable order for this entry takes effect.
 */
JL_API jl_api_ret_t jl_vlan_preprocess_invid_set(
	const jl_uint32             chip_id,
	const jl_uint8              idx,
	jl_vlan_invidbase_tbl_t    *pcfg
);

/**
 * @brief Get the ingress preprocessing VLAN function -- inner VLAN based mode value.
 * @param     chip_id
 * @param     idx     Entry idx
 * @param     pcfg    Inner VLAN config.
 * @return Get successfully or not
 *      @retval JL_ERR_OK       Ok
 *      @retval Other           Fail
 */
JL_API jl_api_ret_t jl_vlan_preprocess_invid_get(
	const jl_uint32             chip_id,
	const jl_uint8              idx,
	jl_vlan_invidbase_tbl_t    *pcfg
);

/**
 * @brief Set the ingress preprocessing VLAN function -- ethertype based mode value.
 * @details  this API will convert frame input VLAN id for VLAN table base on stream ethertype range which is following the innermost VLAN tag..
 * @param     chip_id
 * @param     idx    Entry idx
 * @param     pcfg   Ethertype config.
 * @return Set successfully or not
 *      @retval JL_ERR_OK       Ok
 *      @retval Other           Fail
 * @note
 * Please choice suitable order for this entry takes effect.
 */
JL_API jl_api_ret_t jl_vlan_preprocess_ethtype_set(
	const jl_uint32                 chip_id,
	const jl_uint8                  idx,
	jl_vlan_ethertypebase_tbl_t    *pcfg
);

/**
 * @brief Get the ingress preprocessing mode  ethertype based function value.
 * @param     chip_id
 * @param     idx    Entry idx
 * @param     pcfg   Ethertype config.
 * @return Get successfully or not
 *      @retval JL_ERR_OK       Ok
 *      @retval Other           Fail
 * @note
 * Please choice suitable order for this entry takes effect.
 */
JL_API jl_api_ret_t jl_vlan_preprocess_ethtype_get(
	const jl_uint32                 chip_id,
	const jl_uint8                  idx,
	jl_vlan_ethertypebase_tbl_t    *pcfg
);


/**
 * @brief Set a VLAN Group configuration to a specified VLAN table entry
 * @details This function sets a valid/invalid VLAN Group configuration into the specified VLAN table entry.
 * @param     chip_id
 * @param     vid    Vlan entry idx,range: (2-4094)
 * @param     pcfg   Vlan table config value
 * @return Set successfully or not
 *      @retval JL_ERR_OK       Ok
 *      @retval Other           Fail
 * @note
 * for IVL/SVL setting:\n
 *   - When setting VLAN tbl, the FID value suggest to be set as same as VLAN ID.
 *   - When using SVL mode, plz change bigger VID tbl FID value to smaller VID's FID value to share the MAC table.
 */
JL_API jl_api_ret_t jl_vlan_tbl_set(
	const jl_uint32     chip_id,
	const jl_uint32     vid,
	jl_vlan_tbl_t      *pcfg
);


/**
 * @brief Get a VLAN Group configuration to a specified VLAN table entry
 * @param     chip_id
 * @param     vid    VLAN entry idx
 * @param     pcfg    VLAN table config value
 * @return Get successfully or not
 *      @retval JL_ERR_OK       Ok
 *      @retval Other           Fail
 */
JL_API jl_api_ret_t jl_vlan_tbl_get(
	const jl_uint32     chip_id,
	const jl_uint32     vid,
	jl_vlan_tbl_t      *pcfg
);

/**
 * @brief Del a VLAN Group configuration to a specified VLAN table entry
 * @param     chip_id
 * @param     vid  VLAN entry idx
 * @return Del successfully or not
 *      @retval JL_ERR_OK       Ok
 *      @retval Other           Fail
 * @note This function will auto reset VLAN table value for the specified input VLAN index.
 */
JL_API jl_api_ret_t jl_vlan_tbl_del(
	const jl_uint32     chip_id,
	const jl_uint32     vid
);


/**
 * @brief Set a specified VLAN table entry's FID value
 * @details This function is used to set a specified VLAN table entry's fid value.
 * @param     chip_id
 * @param     vid  VLAN entry idx
 * @param     fid  VLAN table's fid value
 * @return Set successfully or not
 *      @retval JL_ERR_OK       Ok
 *      @retval Other           Fail
 */
JL_API jl_api_ret_t jl_vlan_tbl_set_fid(
	const jl_uint32     chip_id,
	const jl_uint32     vid,
	const jl_uint32     fid
);


/**
 * @brief Get a specified VLAN table entry's FID value
 * @details This function is used to get a specified VLAN table entry's fid value.
 * @param     chip_id
 * @param     vid  VLAN entry idx
 * @param     pfid  VLAN table's fid value
 * @return Get successfully or not
 *      @retval JL_ERR_OK       Ok
 *      @retval Other           Fail
 */
JL_API jl_api_ret_t jl_vlan_tbl_get_fid(
	const jl_uint32     chip_id,
	const jl_uint32     vid,
	jl_uint32          *pfid
);


/**
 * @brief Set a specified VLAN table entry's VLAN member port mask value
 * @details This function is used to set a specified VLAN table entry's VLAN member port mask value.
 * @param     chip_id
 * @param     vid  VLAN entry idx
 * @param     portmsk  VLAN member port mask
 * @return Set successfully or not
 *      @retval JL_ERR_OK       Ok
 *      @retval Other           Fail
 */
JL_API jl_api_ret_t jl_vlan_tbl_set_memberPortmsk(
	const jl_uint32     chip_id,
	const jl_uint32     vid,
	const jl_uint32     portmsk
);

/**
 * @brief Get a specified VLAN table entry's VLAN member port mask value
 * @details This function is used to get a specified VLAN table entry's VLAN member port mask value.
 * @param     chip_id
 * @param     vid  VLAN entry idx
 * @param     pportmsk  VLAN member port mask
 * @return Get successfully or not
 *      @retval JL_ERR_OK       Ok
 *      @retval Other           Fail
 */
JL_API jl_api_ret_t jl_vlan_tbl_get_memberPortmsk(
	const jl_uint32     chip_id,
	const jl_uint32     vid,
	jl_uint32           *pportmsk
);

/**
 * @brief Set VLAN operation for selected mode with the specified index.
 * @details This function is used to set VLAN operation for ingress port/vlan table/egress port with specified index (port index or vlan index).
 * @param     chip_id
 * @param     idx      Port index or vlan index
 * @param     selmode  VLAN operation with selected input
 * @param     value    VLAN operation value
 * @return Set successfully or not
 *      @retval JL_ERR_OK       Ok
 *      @retval Other           Fail
 */
JL_API jl_api_ret_t jl_vlan_set_operation_selection(
	const jl_uint32                 chip_id,
	const jl_uint32                 idx,
	const jl_vlan_sel_operation_t   selmode,
	const jl_vlan_operation_t       value
);

/**
 * @brief Get VLAN operation for selected mode with the specified index.
 * @details This function is used to get VLAN operation for ingress port/vlan table/egress port with specified index (port index or vlan index).
 * @param     chip_id
 * @param     idx      Port index or vlan index
 * @param     selmode  VLAN operation with selected input
 * @param     pvalue   VLAN operation value
 * @return Get successfully or not
 *      @retval JL_ERR_OK       Ok
 *      @retval Other           Fail
 */
JL_API jl_api_ret_t jl_vlan_get_operation_selection(
	const jl_uint32                 chip_id,
	const jl_uint32                 idx,
	const jl_vlan_sel_operation_t   selmode,
	jl_vlan_operation_t             *pvalue
);

/**
 * @brief Set VLAN TPID table field value .
 * @details This function get VLAN TPID value is used when JL_VLAN_TPID_SEL_EGPORT_ETHTYPE is set.
 * @param     chip_id
 * @param     val      tpid value

 * @return Set successfully or not
 *      @retval JL_ERR_OK       Ok
 *      @retval Other           Fail
 */
JL_API jl_api_ret_t jl_vlan_set_tpid_val(
	const jl_uint32         chip_id,
	const jl_uint32         val
);

/**
 * @brief Get VLAN TPID table field value .
 * @details This function get VLAN TPID value is used when JL_VLAN_TPID_SEL_EGPORT_ETHTYPE is set.
 * @param     chip_id
 * @param     pval      pointer of tpid value

 * @return Set successfully or not
 *      @retval JL_ERR_OK       Ok
 *      @retval Other           Fail
 */
JL_API jl_api_ret_t jl_vlan_get_tpid_val(
	const jl_uint32     chip_id,
	jl_uint32           *pval
);

#ifdef __cplusplus
}
#endif

#endif
/** @} Vlan APIs*/
