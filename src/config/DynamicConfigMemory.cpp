#include "config/DynamicConfigMemory.hpp"

#include <cstdlib>

DynamicConfigMemory::DynamicConfigMemory(){
    this->data = new elrond::byte[this->capacity];
}

DynamicConfigMemory::~DynamicConfigMemory(){

    if(this->data != nullptr){
        delete[] this->data;
        this->data = nullptr;
    }

    this->capacity = 0;
    this->index = 0;
    this->length = 0;
}

bool DynamicConfigMemory::realloc(const elrond::sizeT capacity){

    elrond::sizeT newCap = this->capacity;
    while(capacity >= newCap) newCap *= 2;

    elrond::byte *newArray = (elrond::byte *) ::realloc(this->data, newCap * sizeof(elrond::byte));
    if(newArray == nullptr) return false;

    this->data = newArray;
    this->capacity = newCap;
    return true;
}

bool DynamicConfigMemory::alloc(const elrond::sizeT length){

    if(this->length + length >= this->capacity){
        if(!this->realloc(this->length + length)) return false;
    }

    this->length += length;
    return true;
}

bool DynamicConfigMemory::write(const elrond::byte data) {
    if(this->index >= this->length) return false;
    this->data[this->index++] = data;
    return true;
}

bool DynamicConfigMemory::read(const elrond::sizeT index, elrond::byte &data) const {
    if(index >= this->length) return false;
    data = this->data[index];
    return true;
}
