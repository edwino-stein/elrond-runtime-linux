#include "application/LoaderRuntimeApp.hpp"

#include "exceptions/Exception.hpp"
#include "modules/InternalModuleFactory.hpp"
#include "modules/DlModuleFactory.hpp"
#include "modules/ModuleHandle.hpp"

#include "modules/VirtualGpio.hpp"
#include "modules/Serial.hpp"
#include "modules/Udp.hpp"

using elrond::interfaces::RuntimeInterface;
using elrond::modules::Example;
using elrond::modules::Loopback;
using elrond::modules::InputToChannel;
using elrond::modules::DigitalLed;
using elrond::modules::Servo;

LoaderRuntimeApp::LoaderRuntimeApp() : BaseRuntimeApp(){}
LoaderRuntimeApp::~LoaderRuntimeApp(){}

void LoaderRuntimeApp::parseModules(Json &cfg){

    std::cout << " * Loading modules (" << cfg.size() << ")..." << std::endl;

    Vector<ModuleFactoryP> factories;
    LoaderRuntimeApp::initInternalFactories(factories);

    elrond::sizeT i = 1;
    for (auto& el : cfg.items()){

        std::cout << "  #" << i++ << ": Loading \"" << el.key();
        std::cout << "\" from \"" << String(el.value()) << "\"..." << std::endl;

        ModuleFactoryP fac = LoaderRuntimeApp::findFactory(el.value(), factories, this);
        std::cout << "      Loaded: " << fac->info.about() << std::endl;

        this->modules.push_back(std::make_shared<ModuleHandle>(el.key(), fac));

        std::cout << "      Created instance \"" << el.key();
        std::cout << "\" of module \"" << fac->info.name() << "\"" << std::endl;
    }
}

ModuleFactoryP LoaderRuntimeApp::findFactory(String name, Vector<ModuleFactoryP> &factories, RuntimeInterface *app){

    auto it = std::find_if(
        factories.begin(),
        factories.end(),
        [&name](ModuleFactoryP f){ return f->name == name; }
    );

    if(it != factories.end()) return *it;

    ModuleFactoryP f = std::make_shared<DlModuleFactory>(name, app);
    factories.push_back(f);
    return f;
}

void LoaderRuntimeApp::initInternalFactories(Vector<ModuleFactoryP> &factories){

    factories.push_back(
        std::make_shared<InternalModuleFactory<Example>>(
            Example::_getInternalName()
        )
    );

    factories.push_back(
        std::make_shared<InternalModuleFactory<Loopback>>(
            Loopback::_getInternalName()
        )
    );

    factories.push_back(
        std::make_shared<InternalModuleFactory<InputToChannel>>(
            InputToChannel::_getInternalName()
        )
    );

    factories.push_back(
        std::make_shared<InternalModuleFactory<DigitalLed>>(
            DigitalLed::_getInternalName()
        )
    );

    factories.push_back(
        std::make_shared<InternalModuleFactory<Servo>>(
            Servo::_getInternalName()
        )
    );

    factories.push_back(
        std::make_shared<InternalModuleFactory<VirtualGpio>>(
            VirtualGpio::_getInternalName()
        )
    );

    factories.push_back(
        std::make_shared<InternalModuleFactory<Serial>>(
            Serial::_getInternalName()
        )
    );

    factories.push_back(
        std::make_shared<InternalModuleFactory<Udp>>(
            Udp::_getInternalName()
        )
    );

}
