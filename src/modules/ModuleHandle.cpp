#include "modules/ModuleHandle.hpp"
#include "modules/ModuleFactory.hpp"

using elrond::interfaces::ModuleInterface;

ModuleHandle::ModuleHandle(String name, ModuleFactoryP factory):
_name(name), _factory(factory), name(_name), module(_module), factory(_factory){
    this->_module = this->factory->getInstance();
}

ModuleHandle::~ModuleHandle(){
    if(this->_module != nullptr) this->factory->deleteInstance(this->_module);
}

