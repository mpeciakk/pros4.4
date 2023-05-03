#include <FS/PartitionTable.hpp>
#include <Lib/Log.hpp>

MasterBootRecord PartitionTable::readPartitions(ATA* disk) {
    MasterBootRecord mbr;
//    disk->read((u8*) &mbr, 0, sizeof(MasterBootRecord));

    if (mbr.magicNumber != 0xAA55) {
        log(2, "MBR Error");
    }

    for (int i = 0; i < 4; ++i) {
        if (mbr.primaryPartition[i].type == 0x00) {
            continue;
        }

        bool bootable = mbr.primaryPartition[i].bootable == 0x80;

        log(0, "Partition %d %s, type: 0x%x", i, bootable ? "bootable" : "not bootable",
            mbr.primaryPartition[i].type);
    }

    return mbr;
}