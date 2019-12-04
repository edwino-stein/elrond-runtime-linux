#include "standalone/OStremDebugOut.hpp"

using elrond::interfaces::DebugOutInterface;

OStremDebugOut::OStremDebugOut(std::ostream& os) : os(os){}

OStremDebugOut::~OStremDebugOut(){}

const DebugOutInterface& OStremDebugOut::put(const char c[]) const {
    this->os << c;
    return *this;
}

const DebugOutInterface& OStremDebugOut::put(char c) const {
    this->os << c;
    return *this;
}

const DebugOutInterface& OStremDebugOut::put(unsigned char c) const {
    this->os << c;
    return *this;
}

const DebugOutInterface& OStremDebugOut::put(int i) const {
    this->os << i;
    return *this;
}

const DebugOutInterface& OStremDebugOut::put(unsigned int i) const {
    this->os << i;
    return *this;
}

const DebugOutInterface& OStremDebugOut::put(long l ) const {
    this->os << l;
    return *this;
}

const DebugOutInterface& OStremDebugOut::put(unsigned long l ) const {
    this->os << l;
    return *this;
}

const DebugOutInterface& OStremDebugOut::put(double d) const {
    this->os << d;
    return *this;
}

const DebugOutInterface& OStremDebugOut::putLn(const char c[]) const { return this->put(c).putLn(); }
const DebugOutInterface& OStremDebugOut::putLn(char c) const { return this->put(c).putLn(); }
const DebugOutInterface& OStremDebugOut::putLn(unsigned char c) const { return this->put(c).putLn(); }
const DebugOutInterface& OStremDebugOut::putLn(int i) const { return this->put(i).putLn(); }
const DebugOutInterface& OStremDebugOut::putLn(unsigned int i) const { return this->put(i).putLn(); }
const DebugOutInterface& OStremDebugOut::putLn(long l) const { return this->put(l).putLn(); }
const DebugOutInterface& OStremDebugOut::putLn(unsigned long l) const { return this->put(l).putLn(); }
const DebugOutInterface& OStremDebugOut::putLn(double d) const { return this->put(d).putLn(); }

const DebugOutInterface& OStremDebugOut::putLn(void) const {
    this->os << std::endl;
    return *this;
}

const DebugOutInterface& OStremDebugOut::flush() const {
    this->os << std::flush;
    return *this;
}
