#ifndef _FAT12_H
#define _FAT12_H

#include <stdint.h>
#include <stddef.h>

struct ext_bios_parameter_block {
    /* DOS 2.0 bios parameter block */
    uint8_t sector_size[2];
    uint8_t sectors_per_cluster;
    uint8_t reserved_sectors[2];
    uint8_t fat_count;
    uint8_t root_dirs_max[2];
    uint8_t logical_sectors[2];
    uint8_t media_descriptor;
    uint8_t sectors_per_fat[2];
    
    /* DOS 3.31 bios parameter block */
    uint8_t sectors_per_track[2];
    uint8_t head_count[2];
    uint8_t hidden_count[4];
    uint8_t logical_sectors_331[4];
    
    /* Extended bios parameter block */
    uint8_t drive_number;
    uint8_t reserved;
    uint8_t extended_boot_signature;
    uint8_t volume_id[4];
    uint8_t partition_volume_label[11];
    uint8_t file_system_type[8];
    
} __attribute((packed));

struct boot_sector { 
    uint8_t jmp[3];
    uint8_t oem[8];
    struct ext_bios_parameter_block bpb;
    uint8_t boot_code[0x1BF];
    uint8_t drive_number;
    uint8_t boot_sector_signature[2];
} __attribute((packed));

void fat_print_oem(struct boot_sector *bs);
void fat_print_label(struct boot_sector *bs);
void fat_print_filesystem(struct boot_sector *bs);
uint16_t fat_get_entry(struct boot_sector *bs, uint8_t fat[], size_t entry_num);

#endif
