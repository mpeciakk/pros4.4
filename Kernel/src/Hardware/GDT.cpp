#include <Hardware/GDT.hpp>
#include <Lib/String.hpp>

extern "C" void GDTFlush(u32 pointer);

void GDT::setEntry(u32 i, u32 base, u32 limit, u8 access, u8 gran) {
    memset((void*) &entries[i], 0, sizeof(GDTEntry));

    entries[i].baseLow = (base & 0xFFFF);
    entries[i].baseMiddle = (base >> 16) & 0xFF;
    entries[i].baseHigh = (base >> 24) & 0xFF;

    entries[i].limitLow = (limit & 0xFFFF);
    entries[i].granularity = (limit >> 16) & 0x0F;
    entries[i].granularity |= gran & 0xF0;
    entries[i].access = access;
}

void GDT::init() {
    GDTPointer pointer{};
    pointer.limit = (sizeof(GDTEntry) * GDT_MAX_ENTRIES) - 1;
    pointer.base = (u32) &entries;

    setEntry(0, 0, 0, 0, 0);                // Null segment
    setEntry(1, 0, 0xFFFFFFFF, 0x9A, 0xCF); // Code segment
    setEntry(2, 0, 0xFFFFFFFF, 0x92, 0xCF); // Data segment
    setEntry(3, 0, 0xFFFFFFFF, 0xFA, 0xCF); // User mode code segment
    setEntry(4, 0, 0xFFFFFFFF, 0xF2, 0xCF); // User mode data segment

    GDTFlush((u32) &pointer);
}
