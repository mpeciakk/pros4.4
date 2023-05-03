#ifndef PROS_VFS_HPP
#define PROS_VFS_HPP

#include <Lib/Types.hpp>
#include <Lib/GenericTree.hpp>

#define FS_FILE 0x01
#define FS_DIRECTORY 0x02
#define FS_CHARDEVICE 0x03
#define FS_BLOCKDEVICE 0x04
#define FS_PIPE 0x05
#define FS_SYMLINK 0x06
#define FS_MOUNTPOINT 0x08

class Storage;

struct VFSNode {
    char name[256];
    u32 flags;

    Storage* storage;
};

struct VFSMount {
    char* name;
    VFSNode* file;
};

class VFS {
public:
    static VFS& instance() {
        static VFS vfs;
        return vfs;
    };

    void init();

    bool mount(const char* path, VFSNode* file);
    VFSMount* getMount(const char* path);
    VFSMount* getMount(char** path);

    VFSNode* openFile(const char* path, u32 flags);

    VFSNode* findDirectory(VFSNode* node, char* name);

    void dump();
    void dumpRecursive(GenericTreeNode* node, u32 offset);

    u32 read(VFSNode* node, u32 offset, u32 size, u8* buffer);

private:
    VFSNode* root;
    GenericTreeNode* tree;

    bool mountRecursive(char* path, GenericTreeNode* subroot, VFSNode* file);
    VFSMount* getMountRecursive(char** path, GenericTreeNode* subroot);
};

#endif
