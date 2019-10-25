#include "util/DebugOut.hpp"

using elrond::interfaces::DebugOutInterface;

DebugOut::~DebugOut(){}

const DebugOutInterface& DebugOut::put(const char c[]) const {
    std::cout << c;
    return *this;
}

const DebugOutInterface& DebugOut::put(char c) const {
    std::cout << c;
    return *this;
}

const DebugOutInterface& DebugOut::put(unsigned char c) const {
    std::cout << c;
    return *this;
}

const DebugOutInterface& DebugOut::put(int i) const {
    std::cout << i;
    return *this;
}

const DebugOutInterface& DebugOut::put(unsigned int i) const {
    std::cout << i;
    return *this;
}

const DebugOutInterface& DebugOut::put(long l ) const {
    std::cout << l;
    return *this;
}

const DebugOutInterface& DebugOut::put(unsigned long l ) const {
    std::cout << l;
    return *this;
}

const DebugOutInterface& DebugOut::put(double d) const {
    std::cout << d;
    return *this;
}

const DebugOutInterface& DebugOut::putLn(const char c[]) const { return this->put(c).putLn(); }
const DebugOutInterface& DebugOut::putLn(char c) const { return this->put(c).putLn(); }
const DebugOutInterface& DebugOut::putLn(unsigned char c) const { return this->put(c).putLn(); }
const DebugOutInterface& DebugOut::putLn(int i) const { return this->put(i).putLn(); }
const DebugOutInterface& DebugOut::putLn(unsigned int i) const { return this->put(i).putLn(); }
const DebugOutInterface& DebugOut::putLn(long l) const { return this->put(l).putLn(); }
const DebugOutInterface& DebugOut::putLn(unsigned long l) const { return this->put(l).putLn(); }
const DebugOutInterface& DebugOut::putLn(double d) const { return this->put(d).putLn(); }

const DebugOutInterface& DebugOut::putLn(void) const {
    std::cout << std::endl;
    return *this;
}

const DebugOutInterface& DebugOut::flush() const {
    std::cout << std::flush;
    return *this;
}
