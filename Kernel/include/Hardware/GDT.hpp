#ifndef PROS_GDT_HPP
#define PROS_GDT_HPP

#include <Lib/Types.hpp>

#define GDT_MAX_ENTRIES 5

struct GDTEntry {
    u16 limitLow;
    u16 baseLow;
    u8 baseMiddle;
    u8 access;
    u8 granularity;
    u8 baseHigh;
} __attribute__((packed));

struct GDTPointer {
    u16 limit;
    u32 base;
} __attribute__((packed));

class GDT {
public:
    void init();

private:
    GDTEntry entries[GDT_MAX_ENTRIES];

    void setEntry(u32 i, u32 base, u32 limit, u8 access, u8 gran);
};

#endif
