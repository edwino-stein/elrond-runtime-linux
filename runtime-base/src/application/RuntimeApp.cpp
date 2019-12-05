#include "application/RuntimeApp.hpp"

#include "exceptions/Exception.hpp"
#include "modules/ModuleHandle.hpp"
#include "channel/ChannelManager.hpp"
#include "modules/InternalModuleFactory.hpp"
#include "modules/ModuleHandle.hpp"

using elrond::runtime::RuntimeApp;
using elrond::runtime::ChannelManager;
using elrond::runtime::ChannelManagerP;
using elrond::runtime::ModuleHandle;
using elrond::runtime::ModuleHandleP;
using elrond::runtime::ModuleFactory;
using elrond::runtime::ModuleFactoryP;
using elrond::runtime::ModulesFactoriesV;
using elrond::runtime::ModuleInfo;
using elrond::runtime::InternalModuleFactory;
using elrond::runtime::Exception;

using elrond::interfaces::RuntimeInterface;
using elrond::interfaces::ModuleInterface;
using elrond::interfaces::DebugOutInterface;
using elrond::modules::BaseGpioModule;
using elrond::modules::BaseInputDriverModule;
using elrond::config::ConfigMap;
using elrond::channel::BaseChannelManager;

RuntimeInterface* elrond::__rtInstance__ = nullptr;

RuntimeApp::RuntimeApp(DebugOutInterface& dout):
_dout(dout),
_loop(false)
{
    elrond::__rtInstance__ = this;
}

RuntimeApp::~RuntimeApp(){}

ModuleInfo const& RuntimeApp::defineModule(String name, String type, ModulesFactoriesV& factories)
{
    ModuleFactoryP fac = RuntimeApp::findFactory(type, factories, this);
    this->modules.push_back(std::make_shared<ModuleHandle>(name, fac));
    return fac->info;
}

void RuntimeApp::initModule(String name, elrond::config::ConfigMap &cm) const {
    try{
        ModuleHandleP mh = this->findModule(name);
        mh->module->onInit(cm);
    }
    catch(Exception &e){
        throw Exception(
            "An error occurred when initializing the instance \"" + name +"\"",
            e
        );
    }
}

void RuntimeApp::startModules(){

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

void RuntimeApp::stopModules(){

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

ModuleHandleP RuntimeApp::findModule(String name) const {

    auto it = std::find_if(
        this->modules.begin(),
        this->modules.end(),
        [&name](ModuleHandleP mh){ return mh->name == name; }
    );

    if(it != this->modules.end()) return *it;
    throw Exception("No such module instance");
}

void RuntimeApp::start()
{

    std::for_each(
        this->chmgrs.begin(),
        this->chmgrs.end(),
        [](ChannelManagerP chm){ chm->run(); }
    );

    this->startModules();
    this->_loop = true;
}

void RuntimeApp::loop()
{

    Vector<ModuleHandleP> syncLoopMods;

    std::for_each(
        this->modules.begin(),
        this->modules.end(),
        [&syncLoopMods](ModuleHandleP mh){

            if(!mh->module->getLoopControl().allow) return;

            if(mh->module->getLoopControl().async){
                mh->asyncRun();
                return;
            }

            syncLoopMods.push_back(mh);
            mh->syncLoop();
        }
    );

    while(this->_loop){
        std::for_each(
            syncLoopMods.begin(),
            syncLoopMods.end(),
            [](ModuleHandleP mh){ mh->syncLoop(); }
        );
    }
}

void RuntimeApp::stop(bool force){

    if(force) this->_loop = true;
    if(!this->_loop) return;

    this->stopModules();
    this->_loop = false;

    std::for_each(
        this->chmgrs.begin(),
        this->chmgrs.end(),
        [&force](ChannelManagerP chm){ chm->stop(!force); }
    );

    std::for_each(
        this->modules.begin(),
        this->modules.end(),
        [&force](ModuleHandleP mh){ mh->asyncStop(!force); }
    );
}

ChannelManagerP RuntimeApp::defineChannelManager(String transport, const elrond::sizeT tx, const elrond::sizeT rx, const elrond::sizeT fps)
{

    ModuleHandleP mod = this->findModule(transport);
    if(mod->module->getType() != elrond::ModuleType::TRANSPORT)
        throw Exception("Instance is not a transport module");

    ChannelManagerP chmgr = std::make_shared<ChannelManager>(
        (elrond::modules::BaseTransportModule &) *(mod->module),
        tx,
        rx,
        fps
    );

    chmgr->init();
    this->chmgrs.push_back(chmgr);

    return chmgr;
}

BaseGpioModule& RuntimeApp::getGpioService() const
{

    auto it = std::find_if(
        this->modules.begin(),
        this->modules.end(),
        [](ModuleHandleP mh){ return mh->module->getType() == elrond::ModuleType::GPIO; }
    );

    if(it != this->modules.end()) return *((BaseGpioModule*) (*it)->module);

    throw Exception(
        "Invalid gpio service",
        Exception("No gpio service defined")
    );
}

BaseInputDriverModule& RuntimeApp::getInputService(const elrond::sizeT id) const
{

    elrond::sizeT i = 0;
    auto it = std::find_if(
        this->modules.begin(),
        this->modules.end(),
        [&id, &i](ModuleHandleP mod){
            if(mod->module->getType() != elrond::ModuleType::INPUT) return false;
            return (i++) == id;
        }
    );

    if(it != this->modules.end()) return (BaseInputDriverModule&) *((*it)->module);

    throw Exception(
        "Invalid input service",
        Exception("The input service with index " + std::to_string(id) + " not found")
    );
}

const DebugOutInterface& RuntimeApp::dout() const
{
    return (DebugOutInterface&) this->_dout;
}

void RuntimeApp::onError(const char* error)
{
    throw Exception(error);
}

BaseChannelManager &RuntimeApp::getChannelManager(const elrond::sizeT id) const
{

    if(id >= this->chmgrs.size()){
        throw Exception(
            "Invalid channel manager",
            Exception("The channel manager with index " + std::to_string(id) + " has not defined")
        );
    }

    return *this->chmgrs[id];
}


ModuleFactoryP RuntimeApp::findFactory(String name, ModulesFactoriesV &factories, RuntimeInterface *app)
{

    auto it = std::find_if(
        factories.begin(),
        factories.end(),
        [&name](ModuleFactoryP f){ return f->name == name; }
    );

    if(it != factories.end()) return *it;

    throw Exception("No module instance`s factory defined");
}

ModulesFactoriesV RuntimeApp::newModulesFactories()
{

    ModulesFactoriesV factories;

    factories.push_back(
        std::make_shared<InternalModuleFactory<elrond::Example>>(
            elrond::Example::_getInternalName()
        )
    );

    factories.push_back(
        std::make_shared<InternalModuleFactory<elrond::Loopback>>(
            elrond::Loopback::_getInternalName()
        )
    );

    factories.push_back(
        std::make_shared<InternalModuleFactory<elrond::InputToChannel>>(
            elrond::InputToChannel::_getInternalName()
        )
    );

    factories.push_back(
        std::make_shared<InternalModuleFactory<elrond::DigitalLed>>(
            elrond::DigitalLed::_getInternalName()
        )
    );

    factories.push_back(
        std::make_shared<InternalModuleFactory<elrond::AnalogLed>>(
            elrond::AnalogLed::_getInternalName()
        )
    );

    factories.push_back(
        std::make_shared<InternalModuleFactory<elrond::Servo>>(
            elrond::Servo::_getInternalName()
        )
    );

    return std::move(factories);
}
