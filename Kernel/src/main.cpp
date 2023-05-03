#include <Debug/Tests.hpp>
#include <Driver/ATA.hpp>
#include <FS/PartitionTable.hpp>
#include <FS/VFS.hpp>
#include <Hardware/GDT.hpp>
#include <Hardware/IDT.hpp>
#include <Lib/Log.hpp>
#include <Lib/String.hpp>
#include <Lib/Types.hpp>
#include <MM/MemoryManager.hpp>
#include <MM/kmalloc.hpp>
#include <Multiboot.hpp>
#include <VGA.hpp>

int strcmp2(const char *dst, char *src)
{
    int i = 0;

    while ((dst[i] == src[i])) {
        if (src[i++] == 0)
            return 0;
    }

    return 1;
}

extern "C" [[noreturn]] void kmain(PhysicalAddress* mbi, u32 multibootMagic) {
    // Stage 1
    VGA::instance().clear();

    log(0, "Initializing GDT");
    GDT gdt;
    gdt.init();

    log(0, "Initializing InterruptManager");
    InterruptManager::instance().init();

    log(0, "Initializing MemoryManager");
    MemoryManager::instance().init(mbi);

//    log(0, "Initializing DeviceManager");
//    DeviceManager::instance().init();

    // Stage 2
    log(0, "Initializing VFS");
    VFS::instance().init();

    log(0, "Initializing ATA Primary Master");
    ATA* ata0m = new ATA(0x1F0, true);
    ata0m->identify();

    log(0, "Initializing ATA Primary Slave");
    ATA* ata0s = new ATA(0x1F0, false);
    ata0s->identify();

    auto node = new VFSNode();
    strcpy(node->name, "ata device");
    node->storage = ata0s;
    VFS::instance().mount("/dev/hda", node);

//    auto* file = new File();
//    file->name = "ata";
//    file->storage = ata0s;

//    DeviceManager::instance().add(file);

//    VFS::instance().setMount("/dev/hda", file);

//    auto mbr = PartitionTable::readPartitions(ata0s);
//
//    for (int i = 0; i < 4; i++) {
//        if (mbr.primaryPartition[i].type == 0x00) {
//            continue;
//        }
//
//        char partitionNumber[countDigit(i)];
//        itos(i, partitionNumber, 10);
//
//        const char* ata = "hda";
//        char fileName[3 + countDigit(i)];
//
//        strcpy(fileName, ata);
//        strcat(fileName, partitionNumber);
//
////        file = new File();
////        file->name = fileName;
////        file->storage = new PartitionStorage(ata0s, mbr.primaryPartition[i].startLba);
//
//        const char* hda = "/dev/hda";
//        char mountName[8 + countDigit(i)];
//
//        strcpy(mountName, hda);
//        strcat(mountName, partitionNumber);
//    }



//    VFS::instance().tryMount(DeviceManager::instance().get(12), "/test");

    auto* n = new VFSNode();
    strcpy(n->name, "root");
    VFS::instance().mount("/", n);

    n = new VFSNode();
    strcpy(n->name, "test");
    VFS::instance().mount("/test", n);

    n = new VFSNode();
    strcpy(n->name, "asd");
    VFS::instance().mount("/asd", n);

    n = new VFSNode();
    strcpy(n->name, "another test");
    VFS::instance().mount("/test/another", n);

    n = new VFSNode();
    strcpy(n->name, "test/another/sus");
    VFS::instance().mount("/test/another/sus", n);

    n = new VFSNode();
    strcpy(n->name, "amogus");
    VFS::instance().mount("/asd/amogus", n);

//    VFS::instance().dump();

//    VFS::instance().getMount("/asd/amogus");

    MasterBootRecord mbr;
    VFS::instance().read(VFS::instance().openFile("/dev/hda", 0), 0, sizeof(MasterBootRecord), (u8*) &mbr);

    klog(3, "0x%x", mbr.magicNumber);

    log(0, "Kernel loaded!");

//    char* str = "dev/hda";
//    char* currentToken = strsep((char**) &str, "/");
//
//    klog(3, "%s", currentToken);
//
//    currentToken = strsep((char**) &str, "/");
//
//    klog(3, "%s", currentToken);

    // performTests();

    while (true) {
    }
}
