#include <FS/Ext2.hpp>
#include <Lib/Log.hpp>
#include <MM/kmalloc.hpp>

//bool Ext2::probe(File* device) {
//    klog(0, "Probing device %s for Ext2 filesystem", device->name);
//
//    auto* buf = (u8*) kmalloc(sizeof(Superblock));
//    device->storage->read(buf, 2, 500);
//    auto* sb = (Superblock*) buf;
//
//    if (sb->magic != EXT2_SIGNATURE) {
//        klog(1, "Invalid EXT2 signature (0x%x)", sb->magic);
//
//        return false;
//    }
//
//    return true;
//}
//
//bool Ext2::mount(File* device) {
//    klog(0, "Mounting device %s as Ext2 filesystem", device->name);
//
//    return false;
//}
//
//void Ext2::init() { }

void Ext2::init() {
//    auto node = new VFSNode();
//    node->read = &Ext2::read;
}

u32 Ext2::read(VFSNode *file, u32 offset, u32 size, u8 *buf)  {
    return 0;
}
