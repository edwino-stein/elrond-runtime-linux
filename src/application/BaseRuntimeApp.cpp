#include "application/BaseRuntimeApp.hpp"

#include "exceptions/Exception.hpp"
#include "config/DynamicConfigMemory.hpp"
#include "config/CustomConfigMapAllocator.hpp"
#include "modules/ModuleHandle.hpp"

using elrond::interfaces::RuntimeInterface;
using elrond::interfaces::ModuleInterface;
using elrond::interfaces::DebugOutInterface;
using elrond::modules::BaseGpioModule;
using elrond::modules::BaseInputDriverModule;
using elrond::config::ConfigMapAllocator;
using elrond::config::ConfigMap;

BaseRuntimeApp::BaseRuntimeApp(){}
BaseRuntimeApp::~BaseRuntimeApp(){}

void BaseRuntimeApp::initModules(Json &cfg) {

    std::cout << " * Initializing modules instances (" << this->modules.size() << ")..." << std::endl;

    elrond::sizeT i = 1;
    std::for_each(
        this->modules.begin(),
        this->modules.end(),
        [&cfg, &i](ModuleHandleP mh){

            std::cout << "  #" << i++ << ": Initializing instance \"" <<  mh->name << "\"..." << std::endl;

            DynamicConfigMemory dcm;
            CustomConfigMapAllocator cma(dcm);
            BaseRuntimeApp::jsonToCMA(cfg[mh->name], cma);

            try{
                mh->module->onInit(cma);
            }
            catch(Exception &e){
                throw Exception(
                    "An error occurred when initializing the instance \"" + mh->name +"\"",
                    e
                );
            }
        }
    );
}

void BaseRuntimeApp::startModules(){
    std::cout << " * Starting all modules instances..." << std::endl;
    std::for_each(
        this->modules.begin(),
        this->modules.end(),
        [](ModuleHandleP mh){

            try{
                mh->module->onStart();
            }
            catch(Exception &e){
                throw Exception(
                    "An error occurred when starting the instance \"" + mh->name +"\"",
                    e
                );
            }

            mh->started = true;
        }
    );
}

void BaseRuntimeApp::stopModules(){

    std::cout << " * Stopping all modules instances..." << std::endl;

    std::for_each(
        this->modules.begin(),
        this->modules.end(),
        [](ModuleHandleP mh){
            try{
                if(mh->started){
                    mh->started = false;
                    mh->module->onStop();
                }
            }
            catch(Exception &e){

                Exception exp(
                    "An error occurred when stopping the instance \"" + mh->name +"\"",
                    e
                );

                std::cout << " * An error occurred" << std::endl;
                std::cout << exp.what() << '\n';
            }
        }
    );
}

ModuleHandleP BaseRuntimeApp::findModule(String name) const {

    auto it = std::find_if(
        this->modules.begin(),
        this->modules.end(),
        [&name](ModuleHandleP mh){ return mh->name == name; }
    );

    return it != this->modules.end() ? *it : nullptr;
}

BaseGpioModule &BaseRuntimeApp::getGpioService() const {

    auto it = std::find_if(
        this->modules.begin(),
        this->modules.end(),
        [](ModuleHandleP mh){ return mh->module->getType() == elrond::ModuleType::GPIO; }
    );

    if(it == this->modules.end()){
        throw Exception(
            "Invalid gpio service",
            Exception("No gpio service defined")
        );
    }

    return *((BaseGpioModule *) (*it)->module);
}

BaseInputDriverModule &BaseRuntimeApp::getInputService(const elrond::sizeT id) const {

    elrond::sizeT i = 0;
    auto it = std::find_if(
        this->modules.begin(),
        this->modules.end(),
        [&id, &i](ModuleHandleP mod){
            if(mod->module->getType() != elrond::ModuleType::INPUT) return false;
            return (i++) == id;
        }
    );

    if(it != this->modules.end()) return (BaseInputDriverModule &) *((*it)->module);

    throw Exception(
        "Invalid input service",
        Exception("The input service with index " + std::to_string(id) + " not found")
    );
}

const DebugOutInterface &BaseRuntimeApp::dout() const {
    return this->dgo;
}

void BaseRuntimeApp::onError(const char *error){
    throw Exception(error, 2);
}

void BaseRuntimeApp::jsonToCMA(Json &json, CustomConfigMapAllocator &cma){

    if(!json.is_object()) return;

    for(auto& el : json.items()){

        if(el.value().is_array() || el.value().is_object()) continue;

        const char *key = el.key().c_str();

        if(el.value().is_number_integer())
            ((ConfigMapAllocator&) cma).push(key, el.value().get<int>());

        if(el.value().is_number_float())
            ((ConfigMapAllocator&) cma).push(key, el.value().get<double>());

        if(el.value().is_boolean())
            ((ConfigMapAllocator&) cma).push(key, el.value().get<bool>());

        if(el.value().is_string()){
            String str = el.value().get<String>();
            if(str.size() == 1) ((ConfigMapAllocator&) cma).push(key, str[0]);
            else cma.push(key, str);
        }
    }
}
