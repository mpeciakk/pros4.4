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

    PhysicalAddress* a = MemoryManager::instance().getFreePhysicalPage();
    PhysicalAddress* b = MemoryManager::instance().getFreePhysicalPage();
    PhysicalAddress* c = MemoryManager::instance().getFreePhysicalPage();

    klog(3, "0x%x", (u32) a);
    klog(3, "0x%x", (u32) b);
    klog(3, "0x%x", (u32) c);


    performTests();

    while (true) {}
}