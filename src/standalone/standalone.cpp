#include <fstream>

#include "standalone/standalone.hpp"
#include "standalone/OStremDebugOut.hpp"
#include "standalone/Signal.hpp"
#include "standalone/Stacktrace.hpp"

using elrond::runtime::RuntimeApp;
using elrond::config::ConfigMapAllocator;

int main(int argc, char const* argv[]){

    OStremDebugOut dout(std::cout);
    RuntimeApp app(dout);

    Signal::attach(SIG::INT, [&app](){
        std::cout << "\b\b * Received Signal INT (" << (int) SIG::INT << "): ";
        std::cout << "Interrupt" << std::endl;
        stopApplication(app, false, 0);
    });

    Signal::attach(SIG::TERM, [&app](){
        std::cout << "\b\b * Received Signal TERM (" << (int) SIG::TERM << "): ";
        std::cout << "Terminate" << std::endl;
        stopApplication(app, true, 128 + (int) SIG::TERM);
    });

    Signal::attach(SIG::SEGV, [&app](){
        std::cout << std::endl << " * Received Signal SEGV (" << (int) SIG::SEGV << "): ";
        std::cout << "Segmentation fault" << std::endl << std::endl;

        Stacktrace::dump(std::cout, 4);
        std::cout << std::endl;

        stopApplication(app, true, 128 + (int) SIG::SEGV);
    });

    try{
        std::cout << " * Starting " << saInfo.about() << "..." << std::endl;
        loadApplication(argc, argv, app);

        std::cout << " * Starting all modules instances..." << std::endl;
        app.start();

        std::cout << " * Application running (CTRL+C to stop)..." << '\n';
        app.loop();
    }
    catch(Exception &e){
        std::cout << std::endl << " * An error occurred" << std::endl;
        e.what(std::cout);
        std::cout << std::endl;
    }

    stopApplication(app, false, 0);
    return 0;
}

void loadApplication(int argc, char const* argv[], RuntimeApp& app){

    if(argc <= 1) throw Exception("Missing JSON config file");

    Json cfg;
    readJsonFromFile(argv[1], cfg);

    if(!cfg["modules"].is_object()) throw Exception("JSON error", Exception("Missing \"modules\" JSON object"));
    if(!cfg["init"].is_object()) throw Exception("JSON error", Exception("Missing \"init\" JSON object"));

    parseModules(app, cfg["modules"]);
    parseChmgrs(app, cfg["options"]["chmgrs"]);
    initModules(app, cfg);
}

void stopApplication(RuntimeApp& app, bool force, int code)
{
    std::cout << " * Stopping all modules instances..." << std::endl;
    app.stop(force);
    std::exit(code);
}

void parseModules(RuntimeApp& app, Json& cfg){

    std::cout << " * Creating instance modules (" << cfg.size() << ")..." << std::endl;
    ModulesFactories factories = RuntimeApp::newModulesFactories();

    elrond::sizeT i = 1;
    for (auto& el : cfg.items()){

        String name(el.key());
        String type(el.value());
        std::cout << "\t#" << i++ << " Define instance \"" << name;
        std::cout << "\" from \"" << type << "\"" << std::endl;

        auto info = app.defineModule(name, type, factories);
        std::cout << "\t   Created instance \"" << name;
        std::cout << "\" of " << info.about() << std::endl;
    }
}

void initModules(RuntimeApp& app, Json& cfg)
{

    Json& modules = cfg["modules"];
    Json& init = cfg["init"];

    std::cout << " * Initializing modules instances (" << modules.size() << ")..." << std::endl;

    elrond::sizeT i = 1;
    for (auto& el : modules.items()){

        String name(el.key());

        std::cout << "\t#" << i++ << ": Initializing instance \"" <<  name << "\"..." << std::endl;

        Json &jc = init[name];
        DynamicConfigMemory dcm;
        CustomConfigMapAllocator cma(dcm);

        jsonToCMA(jc, cma);
        app.initModule(name, cma);
    }
}

void parseChmgrs(RuntimeApp& app, Json &cfg)
{

    elrond::sizeT i = 0;

    if(cfg.is_array()){

        std::cout << " * Initializing channel managers (" << cfg.size() << ")..." << std::endl;

        for(auto& el : cfg.items()){
            if(!el.value().is_object()) continue;
            Json &chmCfg = el.value();

            String transport = chmCfg["transport"];

            elrond::sizeT tx = chmCfg["tx"].get<int>();
            elrond::sizeT rx = chmCfg["rx"].get<int>();
            elrond::sizeT fps = 0;
            if(chmCfg["tx-fps"].is_number_integer()) fps = chmCfg["tx-fps"].get<int>();

            ChannelManagerP chmgr = nullptr;

            std::cout << "\t#" << (i + 1) << " Define channel manager using instance \"" << transport << "\"" << std::endl;
            try{
                chmgr = app.defineChannelManager(transport, tx, rx, fps);
            }
            catch(Exception &e){
                std::cout << "\t    WARNING: " + e.message << ". Ignoring this channel manager!" << std::endl;
                continue;
            }

            std::cout << "\t   TX Channels: " << tx;
            std::cout << " [" << chmgr->getTxBufferSize() << " Bytes";
            std::cout << (fps == 0 ? "" : "; " + std::to_string(fps) + " fps");
            std::cout << (fps == 0 ? "]" : "; " + std::to_string(chmgr->getTxBufferSize() * fps) + " Bps]") << std::endl;
            std::cout << "\t   RX Channels: " << rx << " [" << chmgr->getRxBufferSize() << " Bytes]" << std::endl;

            ++i;
        }
    }
    else std::cout << " * WARNING: Missing \"chmgrs\" key in JSON file!" << std::endl;

    if(i == 0) std::cout << " * WARNING: No channel managers defined!" << std::endl;
}

void readJsonFromFile(String file, Json& json)
{
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

void jsonToCMA(Json &json, CustomConfigMapAllocator &cma)
{

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
