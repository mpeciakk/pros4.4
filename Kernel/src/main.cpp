#include <VGA.hpp>
#include <Lib/Log.hpp>
#include <Debug/Tests.hpp>
#include <Multiboot.hpp>

extern "C" [[noreturn]] void kmain(multiboot_info* physMbi, u32 multibootMagic) {
    VGA::instance().clear();
    log(0, "Kernel loaded!");

    performTests();

    while (true) {}
}