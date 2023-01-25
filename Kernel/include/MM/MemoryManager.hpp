#ifndef PROS_MEMORYMANAGER_HPP
#define PROS_MEMORYMANAGER_HPP

#include <Lib/Types.hpp>
#include <Multiboot.hpp>

#define BLOCKS_PER_BYTE 8
#define BLOCK_SIZE 4096
#define BLOCK_ALIGN BLOCK_SIZE

#define phys2virt(x) ((u32) (x) + 3_GB)
#define virt2phys(x) ((u32) (x) - 3_GB)

typedef u32 PhysicalAddress;
typedef u32 VirtualAddress;

class MemoryManager {
public:
    static MemoryManager &instance() {
        static MemoryManager memoryManager;
        return memoryManager;
    }

    void init(PhysicalAddress* mbi);

    // PMM
    void initPMM(multiboot_mmap_entry* grubMemoryMap, u32 grubMemoryMapSize);

    void setRegionUsed(PhysicalAddress start, u32 size);
    void setRegionFree(PhysicalAddress start, u32 size);

    PhysicalAddress* getFreePhysicalPage(u32 size = 1);
    void freePage(PhysicalAddress* ptr, u32 size = 1);

    u32 getFreeBlockCount();


private:
    u32 memorySize;
    u32 usedBlocks;
    u32 maxBlocks;
    u32 *memoryMap;

    MemoryManager();

    i32 firstFreePhysicalPagesBits(u32 size = 1);

    inline void setBit(int bit) {
        memoryMap[bit / 32] |= (1 << (bit % 32));
    }

    inline void unsetBit(int bit) {
        memoryMap[bit / 32] &= ~(1 << (bit % 32));
    }

    inline bool testBit(int bit) {
        return memoryMap[bit / 32] & (1 << (bit % 32));
    }
};

#endif
