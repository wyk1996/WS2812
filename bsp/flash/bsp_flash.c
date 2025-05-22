#include <stdint.h>
#include "bsp_flash.h"
#include "gd32e23x_fmc.h"
#include "gd32e23x_rcu.h"

#define FLASH_PAGE_SIZE          1024  // 每页大小为 1KB
#define FLASH_START_ADDRESS      0x08000000  // Flash 起始地址
#define FLASH_TARGET_ADDRESS     0x0800E000  // 目标地址（根据需要修改）

/**
 * @brief 解锁 Flash
 */
void flash_unlock(void)
{
    fmc_unlock();  // 解锁 FMC（Flash Memory Controller）
}



/**
 * @brief 锁定 Flash
 */
void flash_lock(void)
{
    fmc_lock();  // 锁定 FMC
}


/**
 * @brief 擦除 Flash 页
 * @param address: 要擦除的页地址
 * @return FMC 状态
 */
fmc_state_enum flash_erase_page(uint32_t address)
{
    return fmc_page_erase(address);  // 擦除指定页
}




/**
 * @brief 写入数据到 Flash
 * @param address: 写入的起始地址
 * @param data: 要写入的数据指针
 * @param length: 要写入的数据长度（以字为单位，每个字 4 字节）
 * @return FMC 状态
 */
fmc_state_enum flash_write_data(uint32_t address, uint32_t *data, uint32_t length)
{
    fmc_state_enum status;

    for (uint32_t i = 0; i < length; i++) {
        status = fmc_word_program(address + i * 4, data[i]);  // 写入一个字（4 字节）
        if (status != FMC_READY) {
            return status;  // 如果写入失败，返回错误状态
        }
    }

    return FMC_READY;  // 写入成功
}



/**
 * @brief 从 Flash 读取数据
 * @param address: 读取的起始地址
 * @param buffer: 存储读取数据的缓冲区
 * @param length: 要读取的数据长度（以字为单位，每个字 4 字节）
 */
void flash_read_data(uint32_t address, uint32_t *buffer, uint32_t length)
{
    for (uint32_t i = 0; i < length; i++) {
        buffer[i] = *(volatile uint32_t *)(address + i * 4);  // 读取一个字（4 字节）
    }
}



uint32_t write_data[4] = {0x12345678, 0x23456789, 0xAABBCCDD, 0x11223344};  // 要写入的数据
uint32_t read_data[4] = {0};  // 用于存储读取的数据


/**
 * @brief 测试 Flash 写入和读取
 */
void flash_test(void)
{
   
    

    /* 解锁 Flash */
    flash_unlock();

    /* 擦除目标页 */
    if (flash_erase_page(FLASH_TARGET_ADDRESS) != FMC_READY) {
        // 擦除失败，处理错误
        while (1);
    }

    /* 写入数据到 Flash */
    if (flash_write_data(FLASH_TARGET_ADDRESS, write_data, 4) != FMC_READY) {
        // 写入失败，处理错误
        while (1);
    }

    /* 锁定 Flash */
    flash_lock();

   
    

}

void flash_read(void)
{
    
    /* 读取数据并验证 */
    flash_read_data(FLASH_TARGET_ADDRESS, read_data, 4);

    for (uint32_t i = 0; i < 4; i++) {
        if (read_data[i] != write_data[i]) {
            // 数据验证失败，处理错误
            printf("ooooooo\r\n");
            // while (1);
        }
    }

    for(uint8_t i = 0; i < 4; i++){
        printf("read_data[0] = %02x\r\n", read_data[i]);
    }
}


