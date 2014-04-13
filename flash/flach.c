#include "flash.h"
#include "stm32f0xx_flash.h"


void erase_flash(uint32_t address){
    // Unlock the Flash to enable the flash control register access
    FLASH_Unlock();

    // Clear pending flags (if any)
    FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPERR);


    FLASH_ErasePage(address);

    FLASH_Lock();
}

void write_flash(uint32_t address, uint32_t data){
    /* Program the user Flash area word by word
    (area defined by FLASH_USER_START_ADDR and FLASH_USER_END_ADDR) ***********/
    FLASH_Unlock();

    FLASH_ProgramWord(address, data);

    /* Lock the Flash to disable the flash control register access (recommended
     to protect the FLASH memory against possible unwanted operation) *********/
    FLASH_Lock();
}

uint32_t read_flash(uint32_t address){
    return *(__IO uint32_t *)address;
}

void flash_read_struct(struct SavedDomain *struct_p, uint32_t size){
	for(uint32_t i=0; i<(size/4); i++){
		struct_p->enc[i]=*(__IO uint32_t*)(FLASH_USER_START_ADDR+i*4);
	}
}

void flash_write_struct(struct SavedDomain *struct_p, uint32_t size){
	uint16_t data=0;

	erase_flash(FLASH_USER_START_ADDR);

	for(uint32_t i=0; i<(size/4); i++){
        write_flash(FLASH_USER_START_ADDR+i*4, struct_p->enc[i]);
	}
}
