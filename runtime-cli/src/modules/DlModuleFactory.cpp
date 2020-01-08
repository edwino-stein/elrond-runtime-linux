#include "modules/DlModuleFactory.hpp"

#include <dlfcn.h>

using elrond::runtime::ModuleFactory;

using elrond::runtime::Exception;
using elrond::interfaces::RuntimeInterface;
using elrond::interfaces::ModuleInterface;

DlModuleFactory::DlModuleFactory(String name, RuntimeInterface* app):
ModuleFactory(name)
{

    dlModSetAppT setAppInstance = nullptr;

    try{

        this->dlHandle = dlopen(name.c_str(), RTLD_NOW);
        if(!this->dlHandle){
            throw Exception("Invalid shared object (.so) file", Exception(dlerror()));
        }

        dlModIntHandleT getApiVersion = (dlModIntHandleT) dlsym(this->dlHandle, "_infoApiVersion");
        if(getApiVersion == nullptr){
            throw Exception("Invalid shared object (.so) file", Exception(dlerror()));
        }

        dlModIntHandleT getApiRevision = (dlModIntHandleT) dlsym(this->dlHandle, "_infoApiRevision");
        if(getApiRevision == nullptr){
            throw Exception("Invalid shared object (.so) file", Exception(dlerror()));
        }

        dlModStringHandleT getMainClass = (dlModStringHandleT) dlsym(this->dlHandle, "_infoMainClassName");
        if(getMainClass == nullptr){
            throw Exception("Invalid shared object (.so) file", Exception(dlerror()));
        }

        this->_info.apiVer = getApiVersion();
        this->_info.apiRevision = getApiRevision();
        this->_info.mainClass = getMainClass();

        if(this->_info.apiVer != ELROND_API_VERSION){
            throw Exception("Incompatible Module");
        }

        this->_getInstance = (dlModCreateT) dlsym(this->dlHandle, "_getInstance");
        if(this->_getInstance == nullptr){
            throw Exception("Invalid shared object (.so) file", Exception(dlerror()));
        }

        this->_deleteInstance = (dlModDestroyT) dlsym(this->dlHandle, "_deleteInstance");
        if(this->_deleteInstance == nullptr){
            throw Exception("Invalid shared object (.so) file", Exception(dlerror()));
        }

        setAppInstance = (dlModSetAppT) dlsym(this->dlHandle, "_setAppInstance");
        if(setAppInstance == nullptr){
            throw Exception("Invalid shared object (.so) file", Exception(dlerror()));
        }
    }
    catch(Exception &e){
        throw Exception("Unable to load the module from \"" + name + "\"", e);
    }

    dlModStringHandleT getPrettyName = (dlModStringHandleT) dlsym(this->dlHandle, "_infoPrettyName");
    if(getPrettyName != nullptr) this->_info.prettyName = getPrettyName();

    dlModStringHandleT getAuthorName = (dlModStringHandleT) dlsym(this->dlHandle, "_infoAuthorName");
    if(getAuthorName != nullptr) this->_info.authorName = getAuthorName();

    dlModStringHandleT getAuthorEmail = (dlModStringHandleT) dlsym(this->dlHandle, "_infoAuthorEmail");
    if(getAuthorEmail != nullptr) this->_info.authorEmail = getAuthorEmail();

    dlModStringHandleT getVersion = (dlModStringHandleT) dlsym(this->dlHandle, "_infoVersion");
    if(getVersion != nullptr) this->_info.version = getVersion();

    setAppInstance(app);
}

DlModuleFactory::~DlModuleFactory()
{
    if(this->dlHandle != nullptr){
        dlclose(this->dlHandle);
        this->dlHandle = nullptr;
        this->_getInstance = nullptr;
        this->_deleteInstance = nullptr;
    }
}

ModuleInterface* DlModuleFactory::getInstance(String const& instName)
{
    return this->_getInstance();
}

void DlModuleFactory::deleteInstance(ModuleInterface* mod)
{
    this->_deleteInstance(mod);
}
