#include <MM/MemoryManager.hpp>
#include <Lib/String.hpp>
#include <Multiboot.hpp>
#include <Lib/Log.hpp>

extern "C" u32* kernelStart;
extern "C" u32* kernelEnd;

MemoryManager::MemoryManager() {

}


void MemoryManager::init(PhysicalAddress* physMbi) {
    auto* mbi = (multiboot_info*) phys2virt(physMbi);
    auto* grubMemoryMap = (multiboot_mmap_entry*) phys2virt(mbi->mmap_addr);
    auto grubMemoryMapSize = mbi->mmap_length;

    klog(3, "MM: GRUB memory map at 0x%x, with length 0x%x", (u32) grubMemoryMap, grubMemoryMapSize);

    u32 kernelSize = (u32) &kernelEnd - (u32) &kernelStart;

    klog(3, "MM: Kernel start: 0x%x", (u32) &kernelStart);
    klog(3, "MM: Kernel end: 0x%x", (u32) &kernelEnd);
    klog(3, "MM: Kernel size: 0x%x", kernelSize);

    for (auto* entry = grubMemoryMap; (u32) entry < (u32) grubMemoryMap + grubMemoryMapSize;
         entry = (multiboot_mmap_entry*) ((u32) entry + entry->size + sizeof(entry->size))) {

        auto length = entry->len_low;

        memorySize += length;
    }

    klog(0, "MM: Memory size: %d KB", memorySize / 1024);

    initPMM(grubMemoryMap, grubMemoryMapSize);
}