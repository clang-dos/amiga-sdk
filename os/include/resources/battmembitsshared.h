#ifndef RESOURCES_BATTMEMBITSSHARED_H
#define RESOURCES_BATTMEMBITSSHARED_H
/*
    battmem.resource: bits shared by all operating systems (64 and up).
    Part of the clang-dos Amiga SDK; written for it.
*/

/* Set when the shared bits have lost their contents */
#define BATTMEM_SHARED_AMNESIA_ADDR     64
#define BATTMEM_SHARED_AMNESIA_LEN      1

/* SCSI ID of the host adapter, stored inverted */
#define BATTMEM_SCSI_HOST_ID_ADDR       65
#define BATTMEM_SCSI_HOST_ID_LEN        3

/* Synchronous transfers allowed */
#define BATTMEM_SCSI_SYNC_XFER_ADDR     68
#define BATTMEM_SCSI_SYNC_XFER_LEN      1

/* Fast synchronous transfers allowed */
#define BATTMEM_SCSI_FAST_SYNC_ADDR     69
#define BATTMEM_SCSI_FAST_SYNC_LEN      1

/* Tagged queuing allowed */
#define BATTMEM_SCSI_TAG_QUEUES_ADDR    70
#define BATTMEM_SCSI_TAG_QUEUES_LEN     1

/* Extra wait for slow IDE drives at boot */
#define BATTMEM_IDE_EXTRA_WAIT_ADDR     71
#define BATTMEM_IDE_EXTRA_WAIT_LEN      1

#endif /* RESOURCES_BATTMEMBITSSHARED_H */
