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

void ModuleHandle::entryPoint(ModuleHandle * const mh){
    while (mh->running){
        if(mh->started){
            mh->module->loop();
            elrond::delay(mh->module->getLoopControl().time);
        }
    }
}

void ModuleHandle::asyncRun(){
    if(this->running) return;
    this->running = true;
    this->thread = Thread(ModuleHandle::entryPoint, this);
}

void ModuleHandle::asyncStop(bool join){

    if(!this->running) return;

    this->running = false;
    elrond::delay(this->module->getLoopControl().time);

    if(join && this->thread.joinable()) this->thread.join();
    else this->thread.detach();
}

void ModuleHandle::syncLoop(){

    if(!this->started) return;

    if(this->timout > elrond::millis()) return;
    this->module->loop();
    this->timout = elrond::millis() + this->module->getLoopControl().time;
}
