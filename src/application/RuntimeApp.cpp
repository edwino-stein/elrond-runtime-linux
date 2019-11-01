#include "application/RuntimeApp.hpp"

#include <fstream>

#include "exceptions/Exception.hpp"
#include "modules/ModuleHandle.hpp"
#include "channel/ChannelManager.hpp"

using elrond::channel::BaseChannelManager;

RuntimeApp::RuntimeApp() : LoaderRuntimeApp(){
    this->info.mainClass = "RuntimeApp";
    this->info.apiVer = ELROND_API_VERSION;
    this->info.apiRevision = ELROND_API_REVISION;
    this->info.prettyName = "Elrond Runtime for Linux";
    this->info.authorName = "Edwino Stein";
    this->info.authorEmail = "edwino.stein@gmail.com";
    this->info.version = std::to_string(ELROND_API_VERSION) + "." + std::to_string(ELROND_API_REVISION) + "-alpha";
}

RuntimeApp::~RuntimeApp(){}

void RuntimeApp::run(){

    std::for_each(
        this->chmgrs.begin(),
        this->chmgrs.end(),
        [](ChannelManagerP chm){ chm->run(); }
    );

    this->startModules();

    std::cout << " * Application running (CTRL+C to stop)..." << '\n';

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

    while(this->loop){
        std::for_each(
            syncLoopMods.begin(),
            syncLoopMods.end(),
            [](ModuleHandleP mh){ mh->syncLoop(); }
        );
    }
}

void RuntimeApp::stop(bool force){

    if(force) this->loop = true;
    if(!this->loop) return;

    this->stopModules();
    this->loop = false;

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

void RuntimeApp::init(int argc, char const *argv[]){

    std::cout << " * Starting " << this->info.about() << "..." << std::endl;

    if(argc <= 1) throw Exception("Missing JSON config file");

    Json cfg;
    RuntimeApp::readJsonFromFile(argv[1], cfg);

    if(!cfg["modules"].is_object()) throw Exception("JSON error", Exception("Missing \"modules\" JSON object"));
    if(!cfg["init"].is_object()) throw Exception("JSON error", Exception("Missing \"init\" JSON object"));

    this->parseModules(cfg["modules"]);
    this->parseOptions(cfg["options"]);
    this->initModules(cfg["init"]);
}

void RuntimeApp::parseOptions(Json &cfg){
    if(cfg.is_object()){
        this->parseChmgrs(cfg["chmgrs"]);
    }
}

void RuntimeApp::parseChmgrs(Json &cfg){

    if(cfg.is_array()){

        std::cout << " * Initializing channel managers (" << cfg.size() << ")..." << std::endl;
        elrond::sizeT i = 1;

        for(auto& el : cfg.items()){

            if(!el.value().is_object()) continue;
            Json &chmCfg = el.value();

            ModuleHandleP mod = this->findModule(chmCfg["transport"]);
            if(mod == nullptr){
                std::cout << "  WARNING: The \"" << String(chmCfg["transport"]) << "\" instance not found.";
                std::cout << "  Ignoring this channel manager!" << std::endl;
                continue;
            }

            if(mod->module->getType() != elrond::ModuleType::TRANSPORT){
                std::cout << "  WARNING: The \"" << String(chmCfg["transport"]) << "\" instance is not a transport module!.";
                std::cout << "  Ignoring this channel manager!" << std::endl;
                continue;
            }

            unsigned int fps = 0;
            if(chmCfg["tx-fps"].is_number_integer()) fps = chmCfg["tx-fps"].get<int>();

            ChannelManagerP chmgr = std::make_shared<ChannelManager>(
                (elrond::modules::BaseTransportModule &) *(mod->module),
                (elrond::sizeT) chmCfg["tx"].get<int>(),
                (elrond::sizeT) chmCfg["rx"].get<int>(),
                fps
            );

            std::cout << "  #" << i << ": Using instance \"" << mod->name << "\"..." << std::endl;
            std::cout << "      TX Channels: " << chmCfg["tx"].get<int>();
            std::cout << " [" << chmgr->getTxBufferSize() << " Bytes";
            std::cout << (fps == 0 ? "" : "; " + std::to_string(fps) + " fps");
            std::cout << (fps == 0 ? "]" : "; " + std::to_string(chmgr->getTxBufferSize() * fps) + " Bps]") << std::endl;
            std::cout << "      RX Channels: " << chmCfg["rx"].get<int>() << " [" << chmgr->getRxBufferSize() << " Bytes]" << std::endl;

            chmgr->init();
            this->chmgrs.push_back(chmgr);
        }
    }
    else std::cout << "  WARNING: Missing \"chmgrs\" key in JSON file!" << std::endl;

    if(this->chmgrs.size() <= 0) std::cout << "  WARNING: No channel managers defined!" << std::endl;
}

BaseChannelManager &RuntimeApp::getChannelManager(const elrond::sizeT id) const {

    if(id >= this->chmgrs.size()){
        throw Exception(
            "Invalid channel manager",
            Exception("The channel manager with index " + std::to_string(id) + " has not defined")
        );
    }

    return *this->chmgrs[id];
}

void RuntimeApp::readJsonFromFile(String file, Json &json){

    try{

        std::ifstream ifs(file);
        if(!ifs.good()) throw Exception("\"" + file + "\": No such file or directory");

        try{
            ifs >> json;
        }
        catch(std::exception &e){
            throw Exception(e);
        }
    }
    catch(Exception &e){
        throw Exception("Unable to read config file", e);
    }
}
