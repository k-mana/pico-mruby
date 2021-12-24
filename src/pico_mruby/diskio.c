/*-----------------------------------------------------------------------*/
/* Low level disk I/O module SKELETON for FatFs     (C)ChaN, 2019        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control modules to the FatFs module with a defined API.       */
/*-----------------------------------------------------------------------*/

#include "ff.h"			/* Obtains integer types */
#include "diskio.h"		/* Declarations of disk functions */

#include <stdio.h>
#include <string.h>

#include "hardware/flash.h"
#include "hardware/regs/addressmap.h"
#include "hardware/sync.h"

#include "block.h"

#define BLOCK_BASE ((uintptr_t)flash_storage - XIP_BASE)

const uint8_t flash_storage[BLOCK_SIZE] __attribute__((aligned(4096))) = {0};

DSTATUS stat = STA_NOINIT;

/*-----------------------------------------------------------------------*/
/* Get Drive Status                                                      */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status (
	BYTE pdrv		/* Physical drive nmuber to identify the drive */
)
{
	return stat;
}



/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize (
	BYTE pdrv				/* Physical drive nmuber to identify the drive */
)
{
	stat = 0;
	return stat;
}



/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read (
	BYTE pdrv,		/* Physical drive nmuber to identify the drive */
	BYTE *buff,		/* Data buffer to store read data */
	LBA_t sector,	/* Start sector in LBA */
	UINT count		/* Number of sectors to read */
)
{
        uint8_t const* addr = (uint8_t*)(XIP_NOCACHE_NOALLOC_BASE + BLOCK_BASE + (sector * BLOCK_SECTOR_SIZE));
	memcpy(buff, addr, count * BLOCK_SECTOR_SIZE);
	return RES_OK;
}



/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

#if FF_FS_READONLY == 0

DRESULT disk_write (
	BYTE pdrv,			/* Physical drive nmuber to identify the drive */
	const BYTE *buff,	/* Data to be written */
	LBA_t sector,		/* Start sector in LBA */
	UINT count			/* Number of sectors to write */
)
{
	uint8_t temp[FLASH_SECTOR_SIZE];
	uint32_t offset = sector / (FLASH_SECTOR_SIZE / BLOCK_SECTOR_SIZE) * FLASH_SECTOR_SIZE;
       	uint8_t const* addr = (uint8_t*)(XIP_NOCACHE_NOALLOC_BASE + BLOCK_BASE + offset);
	memcpy(temp, addr, FLASH_SECTOR_SIZE);
	uint32_t offset2 = sector % (FLASH_SECTOR_SIZE / BLOCK_SECTOR_SIZE);
	memcpy(temp + offset2 * BLOCK_SECTOR_SIZE, buff, BLOCK_SECTOR_SIZE);
	{
		uint32_t ints = save_and_disable_interrupts();
		flash_range_erase(BLOCK_BASE + offset, FLASH_SECTOR_SIZE);
		flash_range_program(BLOCK_BASE + offset, temp, FLASH_SECTOR_SIZE);
		restore_interrupts(ints);
	}
	return RES_OK;
}

#endif


/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

DRESULT disk_ioctl (
	BYTE pdrv,		/* Physical drive nmuber (0..) */
	BYTE cmd,		/* Control code */
	void *buff		/* Buffer to send/receive control data */
)
{
	DRESULT res;

	res = RES_ERROR;
	switch (cmd) {
		case CTRL_SYNC:
			res = RES_OK;
			break;
		case GET_SECTOR_COUNT:
			*(LBA_t*)buff = BLOCK_COUNT;
			res = RES_OK;
			break;
		case GET_SECTOR_SIZE:
			*(DWORD*)buff = BLOCK_SECTOR_SIZE;
			res = RES_OK;
			break;
		case GET_BLOCK_SIZE:
			*(DWORD*)buff = 1;
			res = RES_OK;
			break;
		default:
			res = RES_PARERR;
	}

	return res;
}
