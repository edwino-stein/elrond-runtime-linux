#include "config/CustomConfigMapAllocator.hpp"

using elrond::runtime::CustomConfigMapAllocator;

using elrond::interfaces::ConfigMapMemoryInterface;
using elrond::config::ConfigMapAllocator;

CustomConfigMapAllocator::CustomConfigMapAllocator(ConfigMapMemoryInterface& dm):
ConfigMapAllocator(dm){}

CustomConfigMapAllocator::~CustomConfigMapAllocator(){}

void CustomConfigMapAllocator::push(const char* key, String value)
{
    this->strValues.push_back(value);
    ConfigMapAllocator::push(key, this->strValues.back().c_str());
}
