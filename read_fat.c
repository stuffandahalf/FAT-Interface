#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

#include "fat12.h"

size_t fsize(FILE *fp);

int main(int argc, char **argv) {
    FILE *fp = fopen("Disk1.img", "r");
    printf("%ld\n", fsize(fp));
    
    struct boot_sector bs;
    fread(&bs, 1, sizeof(bs), fp);
    
    size_t fat_size = (*(uint16_t *)bs.bpb.sectors_per_fat) * (*(uint16_t *)bs.bpb.sector_size);
    
    uint8_t fat1[fat_size];
    uint8_t fat2[fat_size];
    
    //fseek(fp, *(uint16_t *)bs.bpb.reserved_sectors 
    fseek(fp, *(uint16_t *)bs.bpb.sector_size, SEEK_SET);
    fread(fat1, 1, fat_size, fp);
    
    fseek(fp, *(uint16_t *)bs.bpb.sector_size + fat_size, SEEK_SET);
    fread(fat2, 1, fat_size, fp);
    
    int i;
    for (i = 0; i < fat_size; i++) {
        if (fat1[i] != fat2[i]) {
            puts("Failed to match");
        }
        printf("%X\n", fat1[i]);
    }
    
    printf("JMP instruction: %X%X%X\n", bs.jmp[0], bs.jmp[1], bs.jmp[2]);
    printf("boot sector is %ld bytes\n", sizeof(struct boot_sector));
    
    fat_print_oem(&bs);
    fat_print_label(&bs);
    fat_print_filesystem(&bs);
    printf("maximum root directories %d\n", *(uint16_t *)bs.bpb.root_dirs_max);
    printf("total logical sectors %d\n", *(uint32_t *)bs.bpb.logical_sectors);
    printf(" %d\n", bs.bpb.fat_count);
    printf("Sector size %d\n", *(uint16_t *)bs.bpb.sector_size);
    printf("Sectors per FAT %d\n", *(uint16_t *)bs.bpb.sectors_per_fat);
    
    //printf("%lX\n", );
    
    printf("First boot byte %X\n", ((uint8_t *)&bs)[bs.jmp[1]]);
    
    for (i = 0; i < 384; i++) {
        printf("0x%X\n", fat_get_entry(&bs, fat1, i));
    }
    
    fclose(fp);
}

size_t fsize(FILE *fp) {
    size_t size, position;
    
    position = ftell(fp);
    fseek(fp, 0L, SEEK_END);
    size = ftell(fp);
    fseek(fp, position, SEEK_SET);
    rewind(fp);
    
    return size;
}
