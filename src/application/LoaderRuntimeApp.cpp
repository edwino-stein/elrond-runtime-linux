#include "application/LoaderRuntimeApp.hpp"

#include "exceptions/Exception.hpp"

using elrond::interfaces::RuntimeInterface;

LoaderRuntimeApp::LoaderRuntimeApp() : BaseRuntimeApp(){}
LoaderRuntimeApp::~LoaderRuntimeApp(){}

void LoaderRuntimeApp::parseModules(Json &cfg){

    std::cout << " * Loading modules (" << cfg.size() << ")..." << std::endl;

    Vector<ModuleFactoryP> factories;
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
