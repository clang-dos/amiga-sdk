#ifndef RESOURCES_BATTMEMBITSAMIGA_H
#define RESOURCES_BATTMEMBITSAMIGA_H
/*
    battmem.resource: bits used by AmigaOS only.
    Part of the clang-dos Amiga SDK; written for it.
*/

/* Set when the battery-backed memory has lost its contents */
#define BATTMEM_AMIGA_AMNESIA_ADDR  0
#define BATTMEM_AMIGA_AMNESIA_LEN   1

/* SCSI selection timeout: 0 = 128 ms, 1 = 2 s */
#define BATTMEM_SCSI_TIMEOUT_ADDR   1
#define BATTMEM_SCSI_TIMEOUT_LEN    1

/* Scan logical units on SCSI devices */
#define BATTMEM_SCSI_LUNS_ADDR      2
#define BATTMEM_SCSI_LUNS_LEN       1

#endif /* RESOURCES_BATTMEMBITSAMIGA_H */
