#ifndef PROS_EXT2_HPP
#define PROS_EXT2_HPP

#include <Driver/Storage.hpp>
#include <Lib/Types.hpp>

#define EXT2_SIGNATURE 0xEF53

struct Superblock {
    u32 inodes;
    u32 blocks;
    u32 reservedForRoot;
    u32 unallocatedBlocks;
    u32 unallocatedInodes;
    u32 superblockId;
    u32 blockSizeHint;    // shift by 1024 to the left
    u32 fragmentSizeHint; // same ^
    u32 blocksInGroup;
    u32 fragmentsInGroup;
    u32 inodesInGroup;
    u32 lastMount;
    u32 lastWrite;
    u16 mountsSinceLastCheck;
    u16 maxMountsSinceLastCheck;
    u16 magic;
    u16 state;
    u16 errorHandling;
    u16 minorVersion;
    u32 lastCheck;
    u32 intervalBetweenChecks;
    u32 osId;
    u32 majorVersion;
    u16 uuid;
    u16 gid;
    u8 unused[940];
} __attribute__((packed));

class Ext2 : public Storage {
public:
    //    static bool probe(File* device);
    //    static bool mount(File* device);

    void init();
    u32 read(VFSNode *file, u32 offset, u32 size, u8 *buf)  override;
};

#endif
