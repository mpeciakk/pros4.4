#include <Driver/ATA.hpp>
#include <Lib/Log.hpp>
#include <Lib/String.hpp>

ATA::ATA(u16 portBase, bool master)
    : dataPort(portBase), errorPort(portBase + 1), sectorCountPort(portBase + 2), lbaLowPort(portBase + 3), lbaMidPort(portBase + 4),
      lbaHighPort(portBase + 5), devicePort(portBase + 6), commandPort(portBase + 7), controlPort(portBase + 0x206) {

    this->master = master;
}

void ATA::identify() {
    devicePort.write(master ? 0xA0 : 0xB0); // identify if device is master or slave
    sectorCountPort.write(0);               // clear sector count
    lbaLowPort.write(0);                    // clear lba
    lbaMidPort.write(0);                    // clear lba
    lbaHighPort.write(0);                   // clear lba
    commandPort.write(0xEC);                // send identify command

    u8 status = commandPort.read(); // read device status

    if (status == 0x00) {
        // no device on that bus
        return;
    }

    // is device busy?
    while (((status & 0x80) == 0x80) && ((status & 0x01) != 0x01)) {
        status = commandPort.read();
    }

    if (status & 0x01) {
        log(2, "ATA Error");
    }

    for (u16 i = 0; i < 256; i++) {
        u16 data = dataPort.read();

        char* text = "  \0";
        text[0] = (data >> 8) & 0xFF;
        text[1] = data & 0xFF;

        //        __log("%s", text);
    }
}

u32 ATA::read(VFSNode* file, u32 offset, u32 size, u8* buf) {
    u32 start = offset / ATA_SECTOR_SIZE;
    u32 startOffset = offset % ATA_SECTOR_SIZE;

    u32 end = (offset + size - 1) / ATA_SECTOR_SIZE;
    u32 endOffset = (offset + size - 1) % ATA_SECTOR_SIZE;

    u32 counter = start;
    u32 readSize;

    u8* currentBuffer = buf;

    u32 total = 0;

    while (counter <= end) {
        u32 sectorOffset = 0;
        readSize = ATA_SECTOR_SIZE;

        u8* ret = readSector(counter);

        if (counter == start) {
            sectorOffset = startOffset;
            readSize = ATA_SECTOR_SIZE - sectorOffset;
        }

        if (counter == end) {
            readSize = endOffset - sectorOffset + 1;
        }

        memcpy(currentBuffer, ret + sectorOffset, readSize);
        currentBuffer = currentBuffer + readSize;
        total = total + readSize;
        counter++;
    }

    return total;
}

u8* ATA::readSector(u32 lba) {
    u8* buf = (u8*) kmalloc(ATA_SECTOR_SIZE);

    devicePort.write((master ? 0xE0 : 0xF0) | ((lba & 0x0F000000) >> 24));
    errorPort.write(0);
    sectorCountPort.write(1);

    lbaLowPort.write(lba & 0x000000FF);
    lbaMidPort.write((lba & 0x0000FF00) >> 8);
    lbaHighPort.write((lba & 0x00FF0000) >> 16);
    commandPort.write(0x20);

    u8 status = commandPort.read();

    // is device busy?
    while (((status & 0x80) == 0x80) && ((status & 0x01) != 0x01)) {
        status = commandPort.read();
    }

    if (status & 0x01) {
        log(2, "ATA Error");
    }

    for (u32 i = 0; i < 512; i += 2) {
        u16 readData = dataPort.read();

        buf[i] = readData & 0x00FF;
        buf[i + 1] = (readData >> 8) & 0x00FF;
    }

    return buf;
}

VFSNode* ATA::findDirectory(VFSNode* node, char* name) {
    return nullptr;
}