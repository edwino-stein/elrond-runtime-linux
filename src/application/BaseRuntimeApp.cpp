#include "application/BaseRuntimeApp.hpp"

#include "exceptions/Exception.hpp"
#include "config/DynamicConfigMemory.hpp"
#include "config/CustomConfigMapAllocator.hpp"

using elrond::interfaces::RuntimeInterface;
using elrond::interfaces::ModuleInterface;
using elrond::interfaces::DebugOutInterface;
using elrond::modules::BaseGpioModule;
using elrond::modules::BaseInputDriverModule;
using elrond::config::ConfigMapAllocator;
using elrond::config::ConfigMap;

BaseRuntimeApp::BaseRuntimeApp(){}
BaseRuntimeApp::~BaseRuntimeApp(){}


BaseGpioModule &BaseRuntimeApp::getGpioService() const {

    return *((BaseGpioModule *) nullptr;
}

BaseInputDriverModule &BaseRuntimeApp::getInputService(const elrond::sizeT id) const {
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
