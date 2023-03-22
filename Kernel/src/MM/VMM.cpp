#include <MM/MemoryManager.hpp>
#include <Lib/Log.hpp>
#include <Lib/String.hpp>

#define SET_ATTRIBUTE(entry, attr) (*entry |= attr)
#define CLEAR_ATTRIBUTE(entry, attr) (*entry &= ~attr)
#define TEST_ATTRIBUTE(entry, attr) (*entry & attr)
#define SET_FRAME(entry, address) (*entry = (*entry & ~0x7FFFF000) | (address & 0xFFFFF000)) // Only set address/frame, not flags

typedef enum {
    PTE_PRESENT = 0x01,
    PTE_READ_WRITE = 0x02,
    PTE_USER = 0x04,
    PTE_WRITE_THROUGH = 0x08,
    PTE_CACHE_DISABLE = 0x10,
    PTE_ACCESSED = 0x20,
    PTE_DIRTY = 0x40,
    PTE_PAT = 0x80,
    PTE_GLOBAL = 0x100,
    PTE_FRAME = 0x7FFFF000, // bits 12+
} PAGE_TABLE_FLAGS;

typedef enum {
    PDE_PRESENT = 0x01,
    PDE_READ_WRITE = 0x02,
    PDE_USER = 0x04,
    PDE_WRITE_THROUGH = 0x08,
    PDE_CACHE_DISABLE = 0x10,
    PDE_ACCESSED = 0x20,
    PDE_DIRTY = 0x40,       // 4MB entry only
    PDE_PAGE_SIZE = 0x80,   // 0 = 4KB page, 1 = 4MB page
    PDE_GLOBAL = 0x100,     // 4MB entry only
    PDE_PAT = 0x2000,       // 4MB entry only
    PDE_FRAME = 0x7FFFF000, // bits 12+
} PAGE_DIR_FLAGS;

void MemoryManager::initVMM() {
    auto* table = (PageTable*) PHYS_2_VIRT(getFreePhysicalPage());
    memset(table, 0, sizeof(PageTable));

    auto* dir = (PageDirectory*) PHYS_2_VIRT(getFreePhysicalPage());
    memset(dir, 0, sizeof(PageDirectory));

    klog(3, "VMM: Kernel page directory at 0x%x, kernel page table at 0x%x", (u32) dir, (u32) table);

    for (u32 i = 0, physicalAddress = 0, virtualAddress = 0xC0000000; i < 1024; i++, physicalAddress += PAGE_SIZE, virtualAddress += PAGE_SIZE) {
        int page = 0;
        SET_ATTRIBUTE(&page, PTE_PRESENT);
        SET_ATTRIBUTE(&page, PTE_READ_WRITE);
        SET_FRAME(&page, physicalAddress);

        table->entries[PAGE_TABLE_INDEX(virtualAddress)] = page;
    }

    int* entry = &dir->entries[PAGE_DIRECTORY_INDEX(0xC0000000)];
    SET_ATTRIBUTE(entry, PDE_PRESENT);
    SET_ATTRIBUTE(entry, PDE_READ_WRITE);
    SET_FRAME(entry, (u32) VIRT_2_PHYS(table));


    setPageDirectory(dir);
}

void MemoryManager::setPageDirectory(PageDirectory* pageDirectory) {
    currentPageDirectory = pageDirectory;
    asm("movl %0, %%cr3" : : "r"(VIRT_2_PHYS(pageDirectory)));
    //asm("invlpg 0");
}

void MemoryManager::mapPage(PhysicalAddress phys, VirtualAddress virt) {
    //auto* pd = currentPageDirectory;
    //
    //// Get page directory entry for virtual address
    //auto* entry = &pd->entries[PAGE_DIRECTORY_INDEX(virt)];
    //
    //if (!entry->present) {
    //    auto* table = (PageTable*) PHYS_2_VIRT(getFreePhysicalPage());
    //    memset(table, 0, sizeof(PageTable));
    //    auto* entry = &pd->entries[PAGE_DIRECTORY_INDEX(virt)];
    //
    //    entry->present = true;
    //    entry->write = true;
    //    entry->frame = VIRT_2_PHYS(table);
    //}
    //
    //auto* table = (PageTable*) PAGE_GET_PHYSICAL_ADDRESS((u32*) entry);
    //
    //auto* page = (PageTableEntry*) PHYS_2_VIRT(&table->entries[PAGE_TABLE_INDEX(virt)]);
    //
    //auto physicalAddress = getFreePhysicalPage();
    //
    //page->present = true;
    //page->write = true;
    //page->frame = (u32) physicalAddress;
}
