#ifndef PROS_ATA_HPP
#define PROS_ATA_HPP

#include <Driver/Storage.hpp>
#include <Hardware/Port.hpp>

#define ATA_SECTOR_SIZE 512

class ATA : public Storage {
public:
    ATA(u16 portBase, bool master);

    void identify();

    u32 read(VFSNode* file, u32 offset, u32 size, u8* buf) override;
    VFSNode* findDirectory(VFSNode* node, char* name) override;

private:
    u8* readSector(u32 lba);

private:
    Port16Bit dataPort;
    Port8Bit errorPort;
    Port8Bit sectorCountPort;
    Port8Bit lbaLowPort;
    Port8Bit lbaMidPort;
    Port8Bit lbaHighPort;
    Port8Bit devicePort;
    Port8Bit commandPort;
    Port8Bit controlPort;

    bool master;
};

#endif
