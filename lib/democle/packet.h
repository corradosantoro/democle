/*
 * packet.h
 */

#ifndef __PACKET_H
#define __PACKET_H

#include "democle.h"
#include "binary_stream.h"

class DEMOCLEPacket {
    BinaryStream stream;
 public:
    DEMOCLEPacket();
    void put(AtomicFormula a);
    void dump() { stream.dump(); };
};



#endif
