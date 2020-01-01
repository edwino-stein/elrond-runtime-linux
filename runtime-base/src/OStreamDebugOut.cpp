#include "OStreamDebugOut.hpp"

using elrond::runtime::OStreamDebugOut;
using elrond::interfaces::DebugOutInterface;

OStreamDebugOut::OStreamDebugOut(std::ostream& os) : os(os){}

OStreamDebugOut::~OStreamDebugOut(){}

const DebugOutInterface& OStreamDebugOut::put(const char c[]) const
{
    this->os << c;
    return *this;
}

const DebugOutInterface& OStreamDebugOut::put(char c) const
{
    this->os << c;
    return *this;
}

const DebugOutInterface& OStreamDebugOut::put(unsigned char c) const
{
    this->os << c;
    return *this;
}

const DebugOutInterface& OStreamDebugOut::put(int i) const
{
    this->os << i;
    return *this;
}

const DebugOutInterface& OStreamDebugOut::put(unsigned int i) const
{
    this->os << i;
    return *this;
}

const DebugOutInterface& OStreamDebugOut::put(long l ) const
{
    this->os << l;
    return *this;
}

const DebugOutInterface& OStreamDebugOut::put(unsigned long l ) const
{
    this->os << l;
    return *this;
}

const DebugOutInterface& OStreamDebugOut::put(double d) const
{
    this->os << d;
    return *this;
}

const DebugOutInterface& OStreamDebugOut::put(std::string str) const
{
    this->os << str;
    return *this;
}

const DebugOutInterface& OStreamDebugOut::putLn(const char c[]) const
{
    return this->put(c).putLn();
}

const DebugOutInterface& OStreamDebugOut::putLn(char c) const
{
    return this->put(c).putLn();
}

const DebugOutInterface& OStreamDebugOut::putLn(unsigned char c) const
{
    return this->put(c).putLn();
}

const DebugOutInterface& OStreamDebugOut::putLn(int i) const
{
    return this->put(i).putLn();
}

const DebugOutInterface& OStreamDebugOut::putLn(unsigned int i) const
{
    return this->put(i).putLn();
}

const DebugOutInterface& OStreamDebugOut::putLn(long l) const
{
    return this->put(l).putLn();
}

const DebugOutInterface& OStreamDebugOut::putLn(unsigned long l) const
{
    return this->put(l).putLn();
}

const DebugOutInterface& OStreamDebugOut::putLn(double d) const
{
    return this->put(d).putLn();
}

const DebugOutInterface& OStreamDebugOut::putLn(void) const
{
    this->os << std::endl;
    return *this;
}

const DebugOutInterface& OStreamDebugOut::putLn(std::string str) const
{
    return this->put((std::string &) str).putLn();
}

const DebugOutInterface& OStreamDebugOut::flush() const
{
    this->os << std::flush;
    return *this;
}
