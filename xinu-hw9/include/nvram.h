/**
 * @file nvram.h
 * Contains various values used for reading/writing nvram settings.
 *
 */
/* Embedded Xinu, Copyright (C) 2008.  All rights reserved. */

#ifndef _NVRAM_H_
#define _NVRAM_H_

/** NVRAM variables are located at base flash + size of flash - 0x8000 */
#define NVRAM_LOCATION 0xbc3f8000

/**
 * Header to NVRAM settings block
 */
struct nvram_header
{
    unsigned int magic;           /**< 'FLSH' (little endian)           */
    unsigned int length;          /**< bytes (header+vars)              */
    unsigned int crc_ver_init;
    unsigned int config_refresh;
    unsigned int config_ncdl;
};

/**
 * Describes an single NVRAM tuple entity.
 */
struct nvram_tuple
{
    struct nvram_tuple *next;   /**< linked list of tuples              */
    char pair[1];               /**< name = value                       */
};

/*
 * NVRAM constants
 */
#define NVRAM_MAGIC  0x48534c46 /**< 'FLSH' in little-endian            */
#define NVRAM_SIZE   0x8000     /**< bytes reserved for nvram space     */
#define NVRAM_NHASH  256        /**< maximum number of name=value pairs */
#define NVRAM_STRMAX 1024       /**< maximum nvram string size          */

extern struct nvram_header *nvram_header;  /**< header data for nvram   */

/** array of pointers to tuples  */
extern struct nvram_tuple *nvram_tuples[NVRAM_NHASH];

devcall nvramInit(void);
char *nvramGet(char *name);

#endif                          /* _NVRAM_H_ */
