#ifndef _BSP_FLASH_H_
#define _BSP_FLASH_H_

#include "gd32e23x.h"








void flash_unlock(void);
void flash_lock(void);
fmc_state_enum flash_erase_page(uint32_t address);
fmc_state_enum flash_write_data(uint32_t address, uint32_t *data, uint32_t length);
void flash_read_data(uint32_t address, uint32_t *buffer, uint32_t length);


void flash_test(void);
void flash_read(void);

#endif



