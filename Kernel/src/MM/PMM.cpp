#include <MM/MemoryManager.hpp>
#include <Lib/String.hpp>
#include <Lib/Log.hpp>

extern "C" u32* kernelStart;
extern "C" u32* kernelEnd;

void MemoryManager::initPMM(multiboot_mmap_entry* grubMemoryMap, u32 grubMemoryMapSize) {
    klog(3, "PMM: Initializing PMM");

    memoryMap = (u32*) &kernelEnd;
    klog(3, "PMM: Kernel memory map located at 0x%x", (u32) memoryMap);

    maxBlocks = memorySize / BLOCK_SIZE;
    usedBlocks = maxBlocks;
    memset(memoryMap, 0xF, maxBlocks / BLOCKS_PER_BYTE);

    for (auto* entry = grubMemoryMap; (u32) entry < (u32) grubMemoryMap + grubMemoryMapSize;
         entry = (multiboot_mmap_entry*) ((u32) entry + entry->size + sizeof(entry->size))) {

        // TODO: add low and high together
        auto address = (PhysicalAddress) entry->addr_low;
        auto length = entry->len_low;

        switch (entry->type) {
            case MULTIBOOT_MEMORY_AVAILABLE:
                klog(0, "PMM: Found memory region of type Available, address 0x%x and length %d", address, length);
                setRegionFree(address, length);
                break;
            case MULTIBOOT_MEMORY_RESERVED:
                klog(0, "PMM: Found memory region of type Reserved, address 0x%x and length %d", address, length);
                setRegionUsed(address, length);
                break;
            case (MULTIBOOT_MEMORY_ACPI_RECLAIMABLE):
                klog(0, "PMM: Found memory region of type ACPI_RECLAIMABLE, address 0x%x and length %d", address,
                     length);
                setRegionUsed(address, length);
                break;
            case (MULTIBOOT_MEMORY_NVS):
                klog(0, "PMM: Found memory region of type NVS, address 0x%x and length %d", address, length);
                setRegionUsed(address, length);
                break;
            case (MULTIBOOT_MEMORY_BADRAM):
                klog(1, "PMM: Detected bad memory range, address 0x%x and length %d");
                setRegionUsed(address, length);
                break;
            default:
                klog(1, "PMM: Unknown range, address 0x%x and length %d");
                setRegionUsed(address, length);
                break;
        }
    }

    u32 kernelSize = (u32) &kernelEnd - (u32) &kernelStart;
    u32 memoryMapSize = memorySize / 8 / 4096;
    setRegionUsed((PhysicalAddress) 0x00000000, 2_MB);
}

void MemoryManager::setRegionUsed(PhysicalAddress start, u32 size) {
    u32 base = (u32) start / BLOCK_SIZE;
    u32 blocks = size / BLOCK_SIZE;

    klog(3, "PMM: Region 0x%x-0x%x is used", start, (u32) start + size);

    for (; blocks > 0; blocks--) {
        setBit(base++);
        usedBlocks--;
    }

    setBit(0);
}

void MemoryManager::setRegionFree(PhysicalAddress start, u32 size) {
    u32 base = (u32) start / BLOCK_SIZE;
    u32 blocks = size / BLOCK_SIZE;

    klog(3, "PMM: Region 0x%x-0x%x is free", start, (u32) start + size);

    for (; blocks > 0; blocks--) {
        unsetBit(base++);
        usedBlocks++;
    }
}

PhysicalAddress* MemoryManager::getFreePhysicalPage(u32 size) {
    if (getFreeBlockCount() <= 0) {
        klog(3, "PMM: Out of memory");
        return nullptr;
    }

    int frame = firstFreePhysicalPagesBits(size);
    if (frame == -1) {
        klog(3, "PMM: Out of memory");
        return nullptr;
    }

    klog(3, "PMM: Allocating %d blocks on address 0x%x", size, frame);

    for (u32 i = 0; i < size; i++) {
        setBit(frame + i);
    }

    auto* address = (PhysicalAddress*) (frame * BLOCK_SIZE);
    usedBlocks += size;

    return address;
}

void MemoryManager::freePage(PhysicalAddress* ptr, u32 size) {
    auto address = (u32) ptr;
    int frame = address / BLOCK_SIZE;

    klog(3, "PMM: Deallocating %d blocks on address 0x%x", size, frame);

    for (u32 i = 0; i < size; i++) {
        unsetBit(frame + i);
    }

    usedBlocks -= size;
}

u32 MemoryManager::getFreeBlockCount() {
    return maxBlocks - usedBlocks;
}

i32 MemoryManager::firstFreePhysicalPagesBits(u32 size) {
    if (size == 0) {
        return -1;
    }

    for (u32 i = 0; i < memorySize / 32 / 4096; i++) {
        if (memoryMap[i] != 0xFFFFFFFF) {
            for (int j = 0; j < 32; j++) {
                if (!(memoryMap[i] & (1 << j))) {
                    int currentBit = i * 32 + j;

                    u32 free = 0;
                    for (u32 k = 0; k < size; k++) {
                        if (!testBit(currentBit + k)) {
                            free++;
                        }

                        if (free == size) {
                            return currentBit;
                        }
                    }
                }
            }
        }
    }

    return -1;
}