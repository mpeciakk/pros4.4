#include "MM/kmalloc.hpp"
#include <MM/MemoryManager.hpp>
#include <Lib/Log.hpp>
#include <Lib/String.hpp>

void MemoryManager::initVMM() {
    auto* table = (PageTable*) PHYS_2_VIRT(getFreePhysicalPage());
    memset(table, 0, sizeof(PageTable));

    auto* dir = (PageDirectory*) PHYS_2_VIRT(getFreePhysicalPage());
    memset(dir, 0, sizeof(PageDirectory));

    klog(3, "VMM: Kernel page directory at 0x%x, kernel page table at 0x%x", (u32) dir, (u32) table);

    for (u32 i = 0, physicalAddress = 0, virtualAddress = 0xC0000000; i < 1024; i++, physicalAddress += PAGE_SIZE, virtualAddress += PAGE_SIZE) {
        PageTableEntry page{};
        page.present = true;
        page.write = true;
        page.frame = GET_FRAME_ADDRESS(physicalAddress);

        table->entries[PAGE_TABLE_INDEX(virtualAddress)] = *((int*) &page);
    }

    auto entry = &dir->entries[PAGE_DIRECTORY_INDEX(0xC0000000)];
    entry->present = true;
    entry->write = true;
    entry->frame = GET_FRAME_ADDRESS(VIRT_2_PHYS(table));

    setPageDirectory(dir);

    auto physicalAddress = getFreePhysicalPage();
    for (u32 virtualAddress = KERNEL_HEAP_START; virtualAddress < KERNEL_HEAP_START + KERNEL_HEAP_SIZE; physicalAddress = getFreePhysicalPage(), virtualAddress += PAGE_SIZE) {
        mapPage((PhysicalAddress) (u32) physicalAddress, (VirtualAddress) virtualAddress);
    }
}

void MemoryManager::setPageDirectory(PageDirectory* pageDirectory) {
    currentPageDirectory = pageDirectory;
    asm("movl %0, %%cr3" : : "r"(VIRT_2_PHYS(pageDirectory)));
    asm("invlpg 0");
}

void MemoryManager::mapPage(PhysicalAddress phys, VirtualAddress virt) {
    auto* pd = currentPageDirectory;
    
    auto* entry = &pd->entries[PAGE_DIRECTORY_INDEX(virt)];
    
    if (!entry->present) {
        auto* table = (PageTable*) PHYS_2_VIRT(getFreePhysicalPage());
        memset(table, 0, sizeof(PageTable));
    
        entry->present = true;
        entry->write = true;
        entry->frame = GET_FRAME_ADDRESS(VIRT_2_PHYS(table));
    }
    
    auto* table = (PageTable*) PAGE_GET_PHYSICAL_ADDRESS((u32*) entry);
    
    auto* page = (PageTableEntry*) PHYS_2_VIRT(&table->entries[PAGE_TABLE_INDEX(virt)]);
    
    page->present = true;
    page->write = true;
    page->frame = GET_FRAME_ADDRESS((u32) phys);
}
