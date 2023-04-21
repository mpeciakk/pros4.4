#ifndef PROS_MEMORYMANAGER_HPP
#define PROS_MEMORYMANAGER_HPP

#include <Lib/Types.hpp>
#include <Multiboot.hpp>
#include <Lib/StrongAlias.hpp>

#define BLOCKS_PER_BYTE 8
#define BLOCK_SIZE 4096
#define BLOCK_ALIGN BLOCK_SIZE

#define PHYS_2_VIRT(x) ((u32) (x) + 3_GB)
#define VIRT_2_PHYS(x) ((u32) (x) - 3_GB)

#define PAGES_PER_TABLE 1024
#define PAGES_PER_DIR 1024
#define PAGE_SIZE 4096

#define PAGE_DIRECTORY_INDEX(x) ((((u32) x) >> 22) & 0x3ff)
#define PAGE_TABLE_INDEX(x) ((((u32) x) >> 12) & 0x3ff)
#define PAGE_GET_PHYSICAL_ADDRESS(x) ((*x) & ~0xFFF)

#define GET_FRAME_ADDRESS(x) ((x & 0xFFFFF000) >> 12)

struct PageTableEntry {
    int present: 1;
    int write: 1;
    int user: 1;
    int writeThrough: 1;
    int cache: 1;
    int accessed: 1;
    int dirty: 1;
    int pageAttributeTable: 1;
    int global: 1;
    int available: 3;
    int frame: 20;
} __attribute__((packed));

struct PageDirectoryEntry {
    int present: 1;
    int write: 1;
    int user: 1;
    int writeThrough: 1;
    int cache: 1;
    int accessed: 1;
    int available1: 1;
    int pageSize: 1;
    int available2: 4;
    int frame: 20;
} __attribute__((packed));

struct PageTable {
    int entries[PAGES_PER_TABLE];
} __attribute__((packed));

struct PageDirectory {
    PageDirectoryEntry entries[PAGES_PER_DIR];
} __attribute__((packed));

STRONG_ALIAS(u32, PhysicalAddress)
STRONG_ALIAS(u32, VirtualAddress)

class MemoryManager {
public:
    static MemoryManager& instance() {
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

    // VMM
    void initVMM();

    void setPageDirectory(PageDirectory* pageDirectory);

    void mapPage(PhysicalAddress phys, VirtualAddress virt);

private:
    u32 memorySize;

    MemoryManager();

    // PMM
    u32 usedBlocks;
    u32 maxBlocks;
    u32* memoryMap;

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

    // VMM
    PageDirectory* currentPageDirectory;
};

#endif
