#ifndef FLASH_H
#define FLASH_H
#include "stm32f0xx.h"
#include "../globs.h"

#define FLASH_PAGE_SIZE         ((uint32_t)0x00000400)   // FLASH Page Size
#define FLASH_USER_START_ADDR   ((uint32_t)0x08006000)   //Start @ of user Flash area
#define FLASH_USER_END_ADDR     ((uint32_t)0x08007000)   // End @ of user Flash area

void erase_flash(uint32_t address);
void write_flash(uint32_t address, uint32_t data);
uint32_t read_flash(uint32_t address);
void flash_read_struct(struct SavedDomain *struct_p, uint32_t size);
void flash_write_struct(struct SavedDomain *struct_p, uint32_t size);

#endif
