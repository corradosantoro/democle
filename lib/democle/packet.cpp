/*
 * packet.cpp
 */

#include "democle.h"
#include "packet.h"

DEMOCLEPacket::DEMOCLEPacket()
{
    stream << 0xDEAF0001;
}

void DEMOCLEPacket::put(AtomicFormula a)
{
    stream << a.get_name();
}


