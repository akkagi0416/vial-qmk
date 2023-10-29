// Copyright 2023 sekigon-gonnoc
/* SPDX-License-Identifier: GPL-2.0-or-later */

#include <string.h>

#include "print.h"
#include "eeprom.h"

#include "bmp_file.h"
#include "bmp_vial.h"
#include "eeconfig.h"
#include "eeprom_bmp.h"
#include "crc16.h"

bmp_file_t detect_file_type(const uint8_t *data, uint16_t len) {
    if (((const flash_vial_data_t *)data)->magic == BMP_VIAL_FLASH_PAGE_MAGIC) {
        return BMP_FILE_CONFIG;
    } else if ((data[0] == (EECONFIG_MAGIC_NUMBER & 0xff)) //
               && data[1] == (EECONFIG_MAGIC_NUMBER >> 8)) {
        return BMP_FILE_EEPROM;
    }

    return BMP_FILE_NONE;
}

static bmp_file_res_t write_eeprom_file(bmp_file_t file_type, const uint8_t *data, uint32_t offset, uint32_t len) {
    if (offset == 0) {
        eeprom_bmp_set_cache_mode(EEPROM_BMP_CACHE_WRITE_BACK);
    }

    if (offset + len > EEPROM_SIZE) {
        return BMP_FILE_ERROR;
    }

    eeprom_write_block(data, (void *)offset, len);

    if (offset + len >= EEPROM_SIZE) {
        eeprom_bmp_flush();
        eeprom_bmp_set_cache_mode(EEPROM_BMP_CACHE_WRITE_THROUGH);
        eeprom_bmp_save_default();

        return BMP_FILE_COMPLETE;
    }

    return BMP_FILE_CONTINUE;
}

bmp_file_res_t write_bmp_file(bmp_file_t file_type, const uint8_t *data, uint32_t offset, uint32_t len) {
    switch (file_type) {
        case BMP_FILE_CONFIG: {
            if (offset + len > BMP_USER_FLASH_PAGE_SIZE) {
                return BMP_FILE_ERROR;
            }

            memcpy((void *)&flash_vial_data + offset, data, len);

            if (offset + len >= BMP_USER_FLASH_PAGE_SIZE) {
                // check crc and save to flash
                uint16_t crc = crc16((const uint8_t *)&flash_vial_data, BMP_USER_FLASH_PAGE_SIZE - sizeof(flash_vial_data.crc16));
                printf("crc received:%04lx calc:%04x\n", flash_vial_data.crc16, crc);
                if (flash_vial_data.crc16 == crc) {
                    bmp_vial_save_config();
                }

                return BMP_FILE_COMPLETE;
            }
        } break;

        case BMP_FILE_EEPROM: {
            return write_eeprom_file(file_type, data, offset, len);
        } break;

        case BMP_FILE_NONE:
            return BMP_FILE_COMPLETE;
            break;
    }

    return BMP_FILE_CONTINUE;
}