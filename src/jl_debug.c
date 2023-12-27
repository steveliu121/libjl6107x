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

#include "jl_error.h"
#include "jl_debug.h"

const char *_jl_err_msg[] = _JL_ERRMSG_STR;
#ifdef CONFIG_JL_DEBUG
const char *_jl_dbg_token[] = _JL_DBG_TOKEN;
#endif
