#include "application/RuntimeApp.hpp"

#include <fstream>

#include "exceptions/Exception.hpp"

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
    std::cout << " * Application running (CTRL+C to stop)..." << '\n';
    while(this->loop){}
}

void RuntimeApp::stop(bool force){

    if(force) this->loop = true;
    if(!this->loop) return;

    this->loop = false;
}

void RuntimeApp::init(int argc, char const *argv[]){

    std::cout << " * Starting " << this->info.about() << "..." << std::endl;

    if(argc <= 1) throw Exception("Missing JSON config file");

    Json cfg;
    RuntimeApp::readJsonFromFile(argv[1], cfg);
}

BaseChannelManager &RuntimeApp::getChannelManager(const elrond::sizeT id) const {

    throw Exception(
        "Invalid channel manager",
        Exception("The channel manager with index " + std::to_string(id) + " has not defined")
    );
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
