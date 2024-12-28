/*
 * packet.cpp
 */

#include "democle.h"
#include "packet.h"

#define MAGIC 0xDEAF0001

DEMOCLEPacket::DEMOCLEPacket()
{
    stream << MAGIC;
}

DEMOCLEPacket::DEMOCLEPacket(uint8_t * ptr, int len)
{
    stream.fill(ptr, len);
    int magic_;
    stream >> magic_;
    if (magic_ != MAGIC)
        throw new InvalidMagicNumber;
}


void DEMOCLEPacket::put(AtomicFormula a)
{
    stream << a.get_name();
}

void DEMOCLEPacket::put(std::string s)
{
    stream << s;
}

void DEMOCLEPacket::get(AtomicFormula & a)
{
    std::string n;
    stream >> n;
    a.set_name(n);
}

void DEMOCLEPacket::get(std::string & s)
{
    stream >> s;
}


