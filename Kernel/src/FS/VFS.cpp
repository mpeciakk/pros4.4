/*
 * Some theory for future me
 * VFS is made of generic tree structure, where root (/) is at the top
 * it branches into subdirectories (mounts), where mount is usually filesystem
 * when we're looking for a file we're looking for it in vfs tree
 * when it hits to the point where next subdirectory is filesystem it passes control to the filesystem class (storage class)
 * then filesystem class (ie ext2) is looking for this file in its subdirectories
 *
 * TODO: \/
 * basically i see no sensible scenario where i have path under root which is not filesystem
 * so control is always passed to the filesystem class
 * so whole vfs seems kinda pointless?
 * also for now, each folder in the root directory needs to be filesystem which is also weird
 * root dir should also be filesystem
 *
 * so it seems like the VFS should control everything, regardless of whether something is filesystem or not
 * although sources that inspired this VFS are mounting root dir as normal ext2 filesystem
 * so i need to dig deeper into the source code and understand what is going on in there
 *
 * as for now i think that VFS is just keeping track of mounted filesystems
 * like if / is ext2, /test is also ext2 and /test/another is fat32
 * it's looking for filesystem mounted as /test/another and is skipping all previous filesystem
 * it's just looking for correct mount point, regardless of partial filesystems
 */

#include <FS/Ext2.hpp>
#include <FS/VFS.hpp>
#include <Lib/String.hpp>
#include <MM/kmalloc.hpp>

void VFS::init() {
    auto mount = new VFSMount();
    mount->name = strdup("root");
    mount->file = nullptr;

    tree = new GenericTreeNode(mount);
}

bool VFS::mount(const char* path, VFSNode* file) {
    if (path[0] == '/' && strlen(path) == 1) {
        auto entry = (VFSMount*) tree->value;

        if (entry->file) {
            klog(2, "Root dir is already mounted!");

            return false;
        }

        root = file;
        entry->file = file;

        return true;
    }

    auto dupPath = strdup(path + 1);
    mountRecursive(dupPath, tree, file);
    // TODO: free dupPath

    return true;
}

bool VFS::mountRecursive(char* path, GenericTreeNode* subroot, VFSNode* file) {
    auto currentToken = strsep((char**) &path, "/");

    if (currentToken == nullptr || strcmp(currentToken, "") == 0) {
        auto entry = (VFSMount*) subroot->value;

        if (entry->file) {
            klog(2, "/%s is already mounted", path);
        }

        entry->file = file;

        return true;
    }

    bool found = false;

    auto child = subroot->children->head;

    while (child != nullptr) {
        auto treeNode = (GenericTreeNode*) child->value;
        auto entry = (VFSMount*) treeNode->value;

        if (strcmp(entry->name, currentToken) == 0) {
            subroot = treeNode;
            found = true;
        }

        child = child->next;
    }

    if (!found) {
        auto entry = new VFSMount();
        entry->name = strdup(currentToken);
        subroot = (GenericTreeNode*) subroot->children->insert(new GenericTreeNode(entry))->value;
    }

    return mountRecursive(path, subroot, file);
}

VFSMount* VFS::getMount(const char* path) {
    if (path[0] == '/' && strlen(path) == 1) {
        auto* entry = (VFSMount*) tree->value;

        return entry;
    }

    auto pathDup = strdup(path + 1);
    auto result = getMountRecursive(&pathDup, tree);
    // TODO: free pathDup

    return result;
}

VFSMount* VFS::getMount(char** path) {
    if (*path[0] == '/' && strlen(*path) == 1) {
        auto* entry = (VFSMount*) tree->value;

        return entry;
    }

    (*path)++;
    auto result = getMountRecursive(path, tree);

    return result;
}

VFSMount* VFS::getMountRecursive(char** path, GenericTreeNode* subroot) {
    auto currentToken = strsep(path, "/");

    if (currentToken == nullptr || strcmp(currentToken, "") == 0) {
        auto entry = (VFSMount*) subroot->value;

        return entry;
    }

    bool found = false;

    auto child = subroot->children->head;

    while (child != nullptr) {
        auto treeNode = (GenericTreeNode*) child->value;
        auto entry = (VFSMount*) treeNode->value;

        if (strcmp(entry->name, currentToken) == 0) {
            subroot = treeNode;
            found = true;
            break;
        }

        child = child->next;
    }

    if (!found) {
        return (VFSMount*) subroot->value;
    }

    return getMountRecursive(path, subroot);
}

void VFS::dump() {
    dumpRecursive(tree, 0);
}

void VFS::dumpRecursive(GenericTreeNode* node, u32 offset) {
    if (!node) {
        return;
    }

    auto* tmp = (char*) kmalloc(32);
    memset(tmp, 0, 32);
    int len = 0;
    for (u32 i = 0; i < offset; ++i) {
        strcat(tmp, " ");
    }

    auto* entry = (VFSMount*) node->value;

    if (entry->file) {
        klog(3, "%s%s(0x%x, %s)", tmp, entry->name, (unsigned int) entry->file, entry->file->name);
    } else {
        klog(3, "%s%s(empty)", tmp, entry->name);
    }

    len = strlen(entry->name);
    // TODO: free tmp

    auto* child = node->children->head;

    while (child != nullptr) {
        dumpRecursive((GenericTreeNode*) child->value, offset + len + 1);

        child = child->next;
    }
}



VFSNode* VFS::openFile(const char* path, u32 flags) {
    char* filename = strdup(path);
    char* filenameStart = strdup(filename);
    VFSMount* mount = getMount(&filename);
    VFSNode* start = mount->file;
    VFSNode* next = nullptr;

    // we must save initial address to free it later as filename will be changed
    char* freeFilename = filename;

    char* currentToken;

    while (filename != nullptr && ((currentToken = strsep(&filenameStart, "/")) != nullptr)) {
        next = findDirectory(start, currentToken);

        if (next == nullptr) {
            return nullptr;
        }

        start = next;
    }

    if (!next) {
        next = start;
    }

    // TODO: vfs open next
    // TODO: free freeFilename
    // TODO: free filenameStart

    return next;
}




u32 VFS::read(VFSNode* node, u32 offset, u32 size, u8* buffer) {
    if (node->storage) {
        return node->storage->read(node, offset, size, buffer);
    } else {
        return 0;
    }
}

VFSNode* VFS::findDirectory(VFSNode* node, char* name) {
    if (node && (node->flags & FS_DIRECTORY) && node->storage) {
        return node->storage->findDirectory(node, name);
    }

    return nullptr;
}
