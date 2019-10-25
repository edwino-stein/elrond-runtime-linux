#include "modules/DlModuleFactory.hpp"
#include "exceptions/Exception.hpp"

#include <dlfcn.h>

using elrond::interfaces::RuntimeInterface;
using elrond::interfaces::ModuleInterface;

DlModuleFactory::DlModuleFactory(String name, RuntimeInterface *app): ModuleFactory(name){

    try{

        this->dlHandle = dlopen(name.c_str(), RTLD_NOW);
        if(!this->dlHandle){
            throw Exception("Invalid shared object (.so) file", Exception(dlerror()));
        }

        modIntHandleT getApiVersion = (modIntHandleT) dlsym(this->dlHandle, "_infoApiVersion");
        if(getApiVersion == nullptr){
            throw Exception("Invalid shared object (.so) file", Exception(dlerror()));
        }

        modIntHandleT getApiRevision = (modIntHandleT) dlsym(this->dlHandle, "_infoApiRevision");
        if(getApiRevision == nullptr){
            throw Exception("Invalid shared object (.so) file", Exception(dlerror()));
        }

        modStringHandleT getMainClass = (modStringHandleT) dlsym(this->dlHandle, "_infoMainClassName");
        if(getMainClass == nullptr){
            throw Exception("Invalid shared object (.so) file", Exception(dlerror()));
        }

        this->_info.apiVer = getApiVersion();
        this->_info.apiRevision = getApiRevision();
        this->_info.mainClass = getMainClass();

        if(this->_info.apiVer != ELROND_API_VERSION){
            throw Exception("Incompatible Module");
        }

        this->_getInstance = (modCreateT) dlsym(this->dlHandle, "_getInstance");
        if(this->_getInstance == nullptr){
            throw Exception("Invalid shared object (.so) file", Exception(dlerror()));
        }

        this->_deleteInstance = (modDestroyT) dlsym(this->dlHandle, "_deleteInstance");
        if(this->_deleteInstance == nullptr){
            throw Exception("Invalid shared object (.so) file", Exception(dlerror()));
        }

        this->_setAppInstance = (modSetAppT) dlsym(this->dlHandle, "_setAppInstance");
        if(this->_setAppInstance == nullptr){
            throw Exception("Invalid shared object (.so) file", Exception(dlerror()));
        }
    }
    catch(Exception &e){
        throw Exception("Unable to load the module from \"" + name + "\"", e);
    }

    modStringHandleT getPrettyName = (modStringHandleT) dlsym(this->dlHandle, "_infoPrettyName");
    if(getPrettyName != nullptr) this->_info.prettyName = getPrettyName();

    modStringHandleT getAuthorName = (modStringHandleT) dlsym(this->dlHandle, "_infoAuthorName");
    if(getAuthorName != nullptr) this->_info.authorName = getAuthorName();

    modStringHandleT getAuthorEmail = (modStringHandleT) dlsym(this->dlHandle, "_infoAuthorEmail");
    if(getAuthorEmail != nullptr) this->_info.authorEmail = getAuthorEmail();

    modStringHandleT getVersion = (modStringHandleT) dlsym(this->dlHandle, "_infoVersion");
    if(getVersion != nullptr) this->_info.version = getVersion();

    this->_setAppInstance(app);
}

DlModuleFactory::~DlModuleFactory(){
    if(this->dlHandle != nullptr){
        dlclose(this->dlHandle);
        this->dlHandle = nullptr;
        this->_getInstance = nullptr;
        this->_deleteInstance = nullptr;
        this->_setAppInstance = nullptr;
    }
}

ModuleInterface *DlModuleFactory::getInstance(){
    return this->_getInstance();
}

void DlModuleFactory::deleteInstance(ModuleInterface *mod){
    this->_deleteInstance(mod);
}
