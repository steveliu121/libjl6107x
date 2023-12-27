#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "jl_base.h"
#include "jl_device.h"
#include "portable/jl_portable.h"
#include "jl61xx/jl61xx_reg.h"
#include "driver/jl_reg_io.h"

#define APP_VERSION			"1.1"
#define FILE_NAME_LENGTH	64
#define SRAM_SIZE			60000

static jl_uint16 timeout = 60;

static void print_usage(char *app)
{
	printf("Usage: app loader, version %s\n", APP_VERSION);
	printf("Example:\n");
	printf("\t%s <arg1:img name, max length is %d> <arg2:(optional) timeout seconds, default %ds>\n", app, FILE_NAME_LENGTH, timeout);
	printf("\t%s lms_flasher.img\n", app);
	printf("\t%s lms_flasher.img 100\n", app);
}

static jl_ret_t ccs_reset(jl_io_desc_t *io_desc)
{
	jl_uint16 try = 1000;
	TOP_FW_RESERVED19_t csr;
	TOP_FW_RESERVED20_t data0;
	CRG_CCS_RST_t ccs;
	jl_ret_t ret = JL_ERR_OK;

	/*clear control&status register reserved19, fill data0 register reserved20 with '0xed0c'*/
	csr.val[0] = 0;
	ret = jl_reg_burst_write(io_desc, TOP_FW_RESERVED19, csr.val, 1);
	if (ret)
		goto err;
	data0.val[0] = 0xed0c;
	ret = jl_reg_burst_write(io_desc, TOP_FW_RESERVED20, data0.val, 1);
	if (ret)
		goto err;

	 /* reset CCS */
	ccs.val[0] = 1;
	ret = jl_reg_burst_write(io_desc, CRG_CCS_RST, ccs.val, 1);
	if (ret)
		goto err;

	port_udelay(30*1000*1000); //30s

	while (--try) {
		ret = jl_reg_burst_read(io_desc, TOP_FW_RESERVED20, data0.val, 1);
		if (ret)
			goto err;
		if (data0.BF.fw_reserved20 == 0xc0de)
			break;
	}

	if (!try) {
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_ERROR, "Timeout get data0 failed !!!\n");
		return JL_ERR_TIMEOUT;
	}

	return ret;

err:
	JL_DBG_MSG(JL_FLAG_SYS, _DBG_ERROR, "ccs reset reg read/write fail[%d]!!!\n", ret);

	return ret;
}

static jl_ret_t sram_load(jl_io_desc_t *io_desc,
		jl_uint8 *patch_data, jl_uint32 patch_size)
{
	jl_uint8 try = 100;
	jl_uint32 index = 0;
	jl_uint16 i;
	TOP_FW_RESERVED19_t csr;
	jl_uint32 data[4] = {0};
	jl_ret_t ret = JL_ERR_OK;

	while (index < patch_size) {
		/* format data */
		for (i = 0; i < 4; i++) {
			data[i] = (((jl_uint32)patch_data[index+3]) << 24)
					| (((jl_uint32)patch_data[index+2]) << 16)
					| (((jl_uint32)patch_data[index+1]) << 8)
					| ((jl_uint32)patch_data[index]);
			index += 4;
		}

		/* check if ready to receive image data */
		try = 100;
		while (--try) {
			ret = jl_reg_burst_read(io_desc, TOP_FW_RESERVED19, csr.val, 1);
			if (ret)
				goto err;
			if (GET_BIT(csr.BF.fw_reserved19, 8))
				return JL_ERR_FAIL;
			if (!GET_BIT(csr.BF.fw_reserved19, 15))
				break;
		}

		if (!try) {
			JL_DBG_MSG(JL_FLAG_SYS, _DBG_ERROR, "Timeout check receive data failed !!!\n");
			return JL_ERR_TIMEOUT;
		}

		/* download image data */
		ret = jl_reg_burst_write(io_desc, TOP_FW_RESERVED20, data, sizeof(data)/sizeof(jl_uint32));
		if (ret)
			goto err;
		SET_BIT(csr.BF.fw_reserved19, 15);
		ret = jl_reg_burst_write(io_desc, TOP_FW_RESERVED19, csr.val, 1);
		if (ret)
			goto err;
	}

	return ret;

err:
	JL_DBG_MSG(JL_FLAG_SYS, _DBG_ERROR, "sram load reg read/write fail[%d]!!!\n", ret);

	return ret;
}

static jl_ret_t sram_apply(jl_io_desc_t *io_desc)
{
	TOP_FW_RESERVED19_t csr;
	jl_uint16 try = timeout * 10;
	jl_ret_t ret = JL_ERR_OK;

	/*TODO if pre-delay need*/
	/* skip ida status check(skip a read operation) */
	io_desc->reserved = 1;

	/*deliver execute patch instruction*/
	csr.BF.fw_reserved19 = 0;
	SET_BIT(csr.BF.fw_reserved19, 14);
	ret = jl_reg_burst_write(io_desc, TOP_FW_RESERVED19, csr.val, 1);
	if (ret) {
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_ERROR, "sram apply reg write fail[%d]!!!\n", ret);
		return ret;
	}

	/*check patch done running*/
	while (--try) {
		port_udelay(1000*100);
		ret = jl_reg_burst_read(io_desc, TOP_FW_RESERVED19, csr.val, 1);
		if (ret) {
			JL_DBG_MSG(JL_FLAG_SYS, _DBG_ERROR, "sram apply reg read fail[%d]!!!\n", ret);
			return ret;
		}
		if (GET_BITS(csr.BF.fw_reserved19, 5, 7) == 0x7)
			break;
	}
	if (!try) {
		/* bit6 : sram runs in main() , bit7：sram is running */
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_ERROR, "sram apply timeout check patch done running flag[%lx] failed !!!\n", \
			GET_BITS(csr.BF.fw_reserved19, 5, 7));
		return JL_ERR_TIMEOUT;
	}
	if (GET_BIT(csr.BF.fw_reserved19, 8)) {
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_ERROR, "sram apply auto flasher execute failed !!!\n");
		return JL_ERR_FAIL;
	}
	return JL_ERR_OK;
}

int main(int argc, char *argv[])
{
	(void) argc;
	FILE *fp = NULL;
	char name[FILE_NAME_LENGTH+1] = {0};
	jl_uint8 sram_data[SRAM_SIZE];
	size_t sram_size = 0;
	jl_api_ret_t ret;
	jl_io_desc_t io_desc = {
		.chip = JL_CHIP_61XX,
		.type = JL_IO_SMI,
		.smi.mdio.bus_id = 0
	};

	if (argc < 2 || argv[1][0] == '-') {
		print_usage(argv[0]);
		return -1;
	}
	strncpy(name, argv[1], FILE_NAME_LENGTH);
	name[FILE_NAME_LENGTH] = 0;

	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "version %s\n", APP_VERSION);

	if (argc > 2) {
		timeout = strtoul(argv[2], NULL, 10);
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "tftp timeout %d s\n", timeout);
	}

	fp = fopen(name, "rb");
	if (fp == NULL) {
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_ERROR, "fopen fail, please check the image name and confirm that the file exists\n");
		return -1;
	}
	sram_size = fread(sram_data, sizeof(jl_uint8), SRAM_SIZE, fp);
	if (sram_size <= 0) {
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_ERROR, "fread fail\n");
		return -1;
	}
	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "file size: %zu Bytes\n", sram_size);

	ret = jl_reg_io_init(&io_desc);
	if (ret) {
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_ERROR, "io init fail\n");
		goto exit;
	}

	ret = ccs_reset(&io_desc);
	if (ret) {
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_ERROR, "ccs_reset fail\n");
		goto exit;
	}

	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "start load sram\n");
	ret = sram_load(&io_desc, sram_data, (jl_uint32)sram_size);
	if (ret) {
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_ERROR, "sram_load fail\n");
		goto exit;
	}

	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "start execute sram\n");
	ret = sram_apply(&io_desc);
	if (ret) {
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_ERROR, "sram_apply fail\n");
		goto exit;
	}

exit:
	if (ret)
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_ERROR, "sram load flasher fail\n");
	else
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "sram load flasher success\n");
	if (fp)
		fclose(fp);
	jl_reg_io_deinit(&io_desc);

	return 0;
}
