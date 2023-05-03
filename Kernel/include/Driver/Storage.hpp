#ifndef PROS_STORAGE_HPP
#define PROS_STORAGE_HPP

#include <Lib/Types.hpp>
#include <FS/VFS.hpp>

class Storage {
public:
    virtual u32 read(VFSNode* file, u32 offset, u32 size, u8* buf) = 0;
    virtual VFSNode* findDirectory(VFSNode* node, char* name) = 0;
};

//class PartitionStorage : public Storage {
//public:
//    PartitionStorage(Storage* parent, u32 offset);
//
//    u32 read(u8 *buffer, u32 lba, u32 count) override;
//
//private:
//    Storage* storage;
//    u32 offset;
//};

#endif
