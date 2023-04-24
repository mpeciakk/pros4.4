#include "Lib/String.hpp"
#include "MM/kmalloc.hpp"
#include <VGA.hpp>
#include <Lib/Log.hpp>
#include <Debug/Tests.hpp>
#include <Multiboot.hpp>
#include <Hardware/GDT.hpp>
#include <Hardware/IDT.hpp>
#include <MM/MemoryManager.hpp>
#include <Lib/Types.hpp>

extern "C" [[noreturn]] void kmain(PhysicalAddress* mbi, u32 multibootMagic) {
    VGA::instance().clear();

    GDT gdt;
    gdt.init();

    InterruptManager::instance().init();

    MemoryManager::instance().init(mbi);

    log(0, "Kernel loaded!");

    performTests();

    while (true) {}
}
