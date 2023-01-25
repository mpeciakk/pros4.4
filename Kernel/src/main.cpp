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
    log(0, "Kernel loaded!");

    klog(3, "%d", multibootMagic);

    GDT gdt;
    gdt.init();

    InterruptManager::instance().init();

    MemoryManager::instance().init(mbi);

    performTests();

    while (true) {}
}