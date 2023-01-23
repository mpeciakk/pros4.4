#include <VGA.hpp>
#include <Lib/Log.hpp>
#include <Debug/Tests.hpp>
#include <Multiboot.hpp>
#include <Hardware/GDT.hpp>
#include <Hardware/IDT.hpp>

extern "C" [[noreturn]] void kmain(multiboot_info* physMbi, u32 multibootMagic) {
    VGA::instance().clear();
    log(0, "Kernel loaded!");

    GDT gdt;
    gdt.init();

    InterruptManager::instance().init();

    performTests();

//    (*(u32*) 0x2137420) = 2137;

    while (true) {}
}