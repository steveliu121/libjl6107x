#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <signal.h>
//#include <getopt.h>
#include <string.h>

#include "getopt.h"
#include "jl_device.h"
#include "driver/jl_reg_io.h"

struct option longopts[] = {
	{"type", required_argument, NULL, 't'},
	{"address", required_argument, NULL, 'a'},
	{"value", required_argument, NULL, 'v'},
	{"size", required_argument, NULL, 's'},
	{"read", no_argument, NULL, 'r'},
	{"write", no_argument, NULL, 'w'},
	{"help", no_argument, NULL, 'h'},
	{0, 0, 0, 0}
};
static int g_exit;

static void sig_handle(int sig)
{
	(void) sig;
	g_exit = 1;
}

void print_usage(char *app)
{
	printf("Usage: read/write registers\n");
	printf("Example:\n");
	printf("\twrite:%s -w -t <ftdi/hid/win> -a <register address> -v <value0,value1,...> -s <size of the value to be writen, in 32 bits>\n", app);
	printf("\twrite:%s -w -t win -a 0x1202d8 -v 0x10222345,0x2087 -s 2\n", app);
	printf("\tread:%s -r -t <ftdi/hid/win> -a <register address> -s <size of the value to be read, in 32 bits>\n", app);
	printf("\tread:%s -r -t win -a 0x1202d8 -s 2\n", app);
}

int main(int argc, char *argv[])
{
	(void) argc;
	int opt;
	char value_str[256] = {0};
	char *str0 = NULL;
	char *str1 = NULL;
	char value[16] = {0};
	uint32_t buf[8] = {0};
	uint32_t register_addr = 0;
	uint8_t size = 1;
	/* 0:read, 1:write */
	uint8_t rw = 0;
	int i = 0;
	jl_api_ret_t ret;
	jl_io_desc_t io_desc = {
		.chip = JL_CHIP_61XX,
		.type = JL_IO_SMI,
		.smi.mdio.bus_id = 0
	};

	signal(SIGINT, sig_handle);
	signal(SIGTERM, sig_handle);

	while ((opt = getopt_long(argc, argv,
		":t:a:v:s:rwh", longopts, NULL)) != -1) {
		switch (opt) {
		case 't':
			if (strcmp(optarg, "hid") == 0)
				io_desc.type = JL_IO_SMI_HID_DEMO;
			else if (strcmp(optarg, "win") == 0)
				io_desc.type = JL_IO_SMI;
			else
				io_desc.type = JL_IO_SMI_SPI_DEMO;
			break;
		case 'a':
			register_addr = strtoul(optarg, NULL, 16);
			break;
		case 'v':
			strncpy(value_str, optarg, sizeof(value_str) - 1);
			break;
		case 's':
			size = strtoul(optarg, NULL, 10);
			break;
		case 'r':
			rw = 0;
			break;
		case 'w':
			rw = 1;
			break;
		case 'h':
			print_usage(argv[0]);
			return 0;
		case ':':
			fprintf(stdout, "required argument : -%c\n", optopt);
			return -1;
		case '?':
			fprintf(stdout, "invalid param: -%c\n", optopt);
			print_usage(argv[0]);
			return -1;
		}
	}

	ret = jl_reg_io_init(&io_desc);
	if (ret) {
		printf("smi init fail\n");
		return -1;
	}

	if (rw) {
		/* write */
		str0 = value_str;
		str1 = strstr(str0, ",");
		int i = 0;

		if (str1 == NULL)
			buf[i] = strtoul(str0, NULL, 16);

		while (str1 != NULL) {
			memset(value, 0, 16);
			memcpy(value, str0, str1 - str0);
			buf[i] = strtoul(value, NULL, 16);
			printf("0x%08x\t", buf[i]);

			str0 = str1 + 1;
			str1 = strstr(str0, ",");
			i++;

			if (str1 == NULL) {
				buf[i] = strtoul(str0, NULL, 16);
				printf("0x%08x\t", buf[i]);
			}

		}

		ret = jl_reg_burst_write(&io_desc, register_addr, &buf[0], size);
		if (ret) {
			printf("####error[%d]int func[%s] line[%d]\n", ret, __func__, __LINE__);
			goto exit;
		}
	} else {
		/* read */
		ret = jl_reg_burst_read(&io_desc, register_addr, &buf[0], size);
		if (ret) {
			printf("####error[%d]int func[%s] line[%d]\n", ret, __func__, __LINE__);
			goto exit;
		}

		printf("read register[0x%08x]\n", register_addr);
		for (i = 0; i < size; i++)
			printf("\t0x%08x", buf[i]);
	}


exit:
	printf("\n");
	jl_reg_io_deinit(&io_desc);

	return 0;
}
