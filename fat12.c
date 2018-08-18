#include "fat12.h"
#include <stdio.h>

void fat_print_oem(struct boot_sector *bs) {
    int i;
    for (i = 0; i < 8; i++) {
        printf("%c", bs->oem[i]);
    }
    printf("\n");
}

void fat_print_label(struct boot_sector *bs) {
    if (bs->bpb.extended_boot_signature != 0x28) {
        int i;
        for (i = 0; i < 11; i++) {
            printf("%c", bs->bpb.partition_volume_label[i]);
        }
        printf("\n");
    }
    else {
        puts("Not supported");
    }
}

void fat_print_filesystem(struct boot_sector *bs) {
    if (bs->bpb.extended_boot_signature != 0x28) {
        int i;
        for (i = 0; i < 8; i++) {
            printf("%c", bs->bpb.file_system_type[i]);
        }
        printf("\n");
    }
    else {
        puts("Not supported");
    }
}

uint16_t fat_get_entry(struct boot_sector *bs, uint8_t fat[], size_t entry_num) {
    uint16_t entry;
    
    size_t i_msn = !(entry_num % 2) + (3 * entry_num) / 2;
    size_t i_lsb = (entry_num % 2) + (3 * entry_num) / 2;
    
    entry = ((fat[i_msn] >> (4 * (entry_num % 2))) & 0xF) << 8;
    entry += fat[i_lsb];
    return entry;
}
