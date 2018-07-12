/*
 * Copyright (c) 2015 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr.h>
#include <misc/printk.h>
#include <shell/shell.h>
#include <version.h>
#include <board.h>
#include <device.h>
#include <gpio.h>

/* Change this if you have an LED connected to a custom port */
#ifndef LED0_GPIO_CONTROLLER
#define LED0_GPIO_CONTROLLER 	LED0_GPIO_PORT
#endif

#define LED_PORT LED0_GPIO_CONTROLLER

/* Change this if you have an LED connected to a custom pin */
#define LED	LED0_GPIO_PIN

/* 1000 msec = 1 sec */
//#define SLEEP_TIME 	1000
#define SLEEP_TIME 	200

/* size of stack area used by each thread */
#define STACKSIZE 1024

/* scheduling priority used by each thread */
#define PRIORITY 7

static int shell_cmd_ping(int argc, char *argv[])
{
	ARG_UNUSED(argc);
	ARG_UNUSED(argv);

	printk("pong\n");

	return 0;
}

static int shell_cmd_params(int argc, char *argv[])
{
	int cnt;

	printk("argc = %d\n", argc);
	for (cnt = 0; cnt < argc; cnt++) {
		printk("  argv[%d] = %s\n", cnt, argv[cnt]);
	}
	return 0;
}

#define SHELL_CMD_VERSION "version"
static int shell_cmd_version(int argc, char *argv[])
{
	ARG_UNUSED(argc);
	ARG_UNUSED(argv);

	printk("Zephyr version %s\n", KERNEL_VERSION_STRING);
	return 0;
}

SHELL_REGISTER_COMMAND(SHELL_CMD_VERSION, shell_cmd_version,
		       "Show kernel version");

#define MY_SHELL_MODULE "sample_module"

static struct shell_cmd commands[] = {
	{ "ping", shell_cmd_ping, NULL },
	{ "params", shell_cmd_params, "print argc" },
	{ NULL, NULL, NULL }
};


void main(void)
{
	SHELL_REGISTER(MY_SHELL_MODULE, commands);
}

void blink(void)
{
	int cnt = 0;
	struct device *dev;

	dev = device_get_binding(LED_PORT);
	/* Set LED pin as output */
	gpio_pin_configure(dev, LED, GPIO_DIR_OUT);

	while (1) {
		/* Set pin to HIGH/LOW every 1 second */
		gpio_pin_write(dev, LED, cnt % 2);
		cnt++;
		k_sleep(SLEEP_TIME);
	}
}

K_THREAD_DEFINE(blink_id, STACKSIZE, blink, NULL, NULL, NULL,
		PRIORITY, 0, K_NO_WAIT);

#if 0
void test(void)
{
	int cnt = 0;

	while (1) {
		printk("cnt %d\n", cnt++);
		k_sleep(SLEEP_TIME);
	}
}

K_THREAD_DEFINE(test_id, STACKSIZE, test, NULL, NULL, NULL,
		PRIORITY, 0, K_NO_WAIT);
#endif
