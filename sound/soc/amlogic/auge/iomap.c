/*
 * sound/soc/amlogic/auge/iomap.c
 *
 * Copyright (C) 2017 Amlogic, Inc. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 */

/*#define DEBUG*/

#include <linux/of.h>
#include <linux/io.h>
#include <linux/of_address.h>
#include <linux/platform_device.h>

#include "regs.h"
#include "iomap.h"
#include "audio_aed_reg_list.h"
#include "audio_top_reg_list.h"

#define DEV_NAME	"auge_snd_iomap"

static void __iomem *aml_snd_reg_map[IO_MAX];

#ifdef DEBUG
static void register_debug(u32 base_type, unsigned int reg, unsigned int val)
{
	if (base_type == IO_AUDIO_BUS) {
		pr_debug("audio top reg:[%s] addr: [%#x] val: [%#x]\n",
			top_register_table[reg].name,
			top_register_table[reg].addr, val);
	} else if (base_type == IO_EQDRC_BUS) {
		pr_debug("audio aed reg:[%s] addr: [%#x] val: [%#x]\n",
			aed_register_table[reg].name,
			aed_register_table[reg].addr, val);
	}
}
#endif

static int aml_snd_read(u32 base_type, unsigned int reg, unsigned int *val)
{
	if (base_type < IO_MAX) {
		*val = readl((aml_snd_reg_map[base_type] + (reg << 2)));

		return 0;
	}

	return -1;
}

static void aml_snd_write(u32 base_type, unsigned int reg, unsigned int val)
{

	if (base_type < IO_MAX) {
		writel(val, (aml_snd_reg_map[base_type] + (reg << 2)));
#ifdef DEBUG
		register_debug(base_type, reg, val);
#endif
		return;
	}

	pr_err("write snd reg %x error\n", reg);
}

static void aml_snd_update_bits(u32 base_type, unsigned int reg,
				unsigned int mask, unsigned int val)
{
	if (base_type < IO_MAX) {
		unsigned int tmp, orig;

		if (aml_snd_read(base_type, reg, &orig) == 0) {
			tmp = orig & ~mask;
			tmp |= val & mask;
			aml_snd_write(base_type, reg, tmp);

			return;
		}
	}
	pr_err("write snd reg %x error\n", reg);

}

int aml_pdm_read(unsigned int reg)
{
	int ret, val = 0;

	ret = aml_snd_read(IO_PDM_BUS, reg, &val);

	if (ret) {
		pr_err("read pdm reg %x error %d\n", reg, ret);
		return -1;
	}
	return val;
}
EXPORT_SYMBOL(aml_pdm_read);

void aml_pdm_write(unsigned int reg, unsigned int val)
{
	aml_snd_write(IO_PDM_BUS, reg, val);
}
EXPORT_SYMBOL(aml_pdm_write);

void aml_pdm_update_bits(unsigned int reg, unsigned int mask,
			 unsigned int val)
{
	aml_snd_update_bits(IO_PDM_BUS, reg, mask, val);
}
EXPORT_SYMBOL(aml_pdm_update_bits);

int audiobus_read(unsigned int reg)
{
	int ret, val = 0;

	ret = aml_snd_read(IO_AUDIO_BUS, reg, &val);

	if (ret) {
		pr_err("read audio reg %x error %d\n", reg, ret);
		return -1;
	}
	return val;
}
EXPORT_SYMBOL(audiobus_read);

void audiobus_write(unsigned int reg, unsigned int val)
{
	aml_snd_write(IO_AUDIO_BUS, reg, val);
}
EXPORT_SYMBOL(audiobus_write);

void audiobus_update_bits(unsigned int reg, unsigned int mask,
			  unsigned int val)
{
	aml_snd_update_bits(IO_AUDIO_BUS, reg, mask, val);
}
EXPORT_SYMBOL(audiobus_update_bits);

int audiolocker_read(unsigned int reg)
{
	int ret, val = 0;

	ret = aml_snd_read(IO_AUDIO_LOCKER, reg, &val);

	if (ret) {
		pr_err("read reg %x error %d\n", reg, ret);
		return -1;
	}
	return val;
}
EXPORT_SYMBOL(audiolocker_read);

void audiolocker_write(unsigned int reg, unsigned int val)
{
	aml_snd_write(IO_AUDIO_LOCKER, reg, val);
}
EXPORT_SYMBOL(audiolocker_write);

void audiolocker_update_bits(unsigned int reg, unsigned int mask,
			     unsigned int val)
{
	aml_snd_update_bits(IO_AUDIO_LOCKER, reg, mask, val);
}
EXPORT_SYMBOL(audiolocker_update_bits);

int eqdrc_read(unsigned int reg)
{
	int ret, val = 0;

	ret = aml_snd_read(IO_EQDRC_BUS, reg, &val);

	if (ret) {
		pr_err("read audio reg %x error %d\n", reg, ret);
		return -1;
	}
	return val;
}
EXPORT_SYMBOL(eqdrc_read);

void eqdrc_write(unsigned int reg, unsigned int val)
{
	aml_snd_write(IO_EQDRC_BUS, reg, val);
}
EXPORT_SYMBOL(eqdrc_write);

void eqdrc_update_bits(unsigned int reg, unsigned int mask,
		       unsigned int val)
{
	aml_snd_update_bits(IO_EQDRC_BUS, reg, mask, val);
}
EXPORT_SYMBOL(eqdrc_update_bits);

int audioreset_read(unsigned int reg)
{
	int ret, val = 0;

	ret = aml_snd_read(IO_RESET, reg, &val);

	if (ret) {
		pr_err("read reg %x error %d\n", reg, ret);
		return -1;
	}
	return val;
}
EXPORT_SYMBOL(audioreset_read);

void audioreset_write(unsigned int reg, unsigned int val)
{
	aml_snd_write(IO_RESET, reg, val);
}
EXPORT_SYMBOL(audioreset_write);

void audioreset_update_bits(unsigned int reg, unsigned int mask,
			    unsigned int val)
{
	aml_snd_update_bits(IO_RESET, reg, mask, val);
}
EXPORT_SYMBOL(audioreset_update_bits);

int vad_read(unsigned int reg)
{
	int ret, val = 0;

	ret = aml_snd_read(IO_VAD, reg, &val);

	if (ret) {
		pr_err("read audio reg %x error %d\n", reg, ret);
		return -1;
	}
	return val;
}
EXPORT_SYMBOL(vad_read);

void vad_write(unsigned int reg, unsigned int val)
{
	aml_snd_write(IO_VAD, reg, val);
}
EXPORT_SYMBOL(vad_write);

void vad_update_bits(unsigned int reg, unsigned int mask,
		     unsigned int val)
{
	aml_snd_update_bits(IO_VAD, reg, mask, val);
}
EXPORT_SYMBOL(vad_update_bits);

unsigned int new_resample_read(enum resample_idx id, unsigned int reg)
{
	unsigned int val = 0;

	if (id == RESAMPLE_A)
		val = readl((aml_snd_reg_map[IO_RESAMPLEA] +
			    (reg << 2)));
	else if (id == RESAMPLE_B)
		val = readl((aml_snd_reg_map[IO_RESAMPLEB] +
			    (reg << 2)));

	return val;
}

void new_resample_write(enum resample_idx id, unsigned int reg,
			unsigned int val)
{
	if (id == RESAMPLE_A)
		writel(val, (aml_snd_reg_map[IO_RESAMPLEA] + (reg << 2)));
	else if (id == RESAMPLE_B)
		writel(val, (aml_snd_reg_map[IO_RESAMPLEB] + (reg << 2)));
}

void new_resample_update_bits(enum resample_idx id, unsigned int reg,
			      unsigned int mask, unsigned int val)
{
	unsigned int tmp, orig;

	orig = new_resample_read(id, reg);
	tmp = orig & ~mask;
	tmp |= val & mask;

	new_resample_write(id, reg, tmp);
}

static int snd_iomap_probe(struct platform_device *pdev)
{
	struct resource res;
	struct device_node *np, *child;
	int i = 0;
	int ret = 0;

	np = pdev->dev.of_node;
	for_each_child_of_node(np, child) {
		if (of_address_to_resource(child, 0, &res)) {
			ret = -1;
			pr_err("%s could not get resource",
				__func__);
			break;
		}
		aml_snd_reg_map[i] =
			ioremap_nocache(res.start, resource_size(&res));
		pr_info("aml_snd_reg_map[%d], reg:%x, size:%x\n",
			i, (u32)res.start, (u32)resource_size(&res));

		i++;
	}
	pr_info("amlogic %s probe done\n", DEV_NAME);

	return ret;
}

static const struct of_device_id snd_iomap_dt_match[] = {
	{ .compatible = "amlogic, snd-iomap" },
	{},
};

static  struct platform_driver snd_iomap_platform_driver = {
	.probe		= snd_iomap_probe,
	.driver		= {
		.owner		    = THIS_MODULE,
		.name		    = DEV_NAME,
		.of_match_table	= snd_iomap_dt_match,
	},
};

int __init auge_snd_iomap_init(void)
{
	int ret;

	ret = platform_driver_register(&snd_iomap_platform_driver);

	return ret;
}
core_initcall(auge_snd_iomap_init);
