#if !defined  _ELROND_RUNTIME_STANDALONE_HPP
    #define _ELROND_RUNTIME_STANDALONE_HPP

    #include "elrond-runtime.hpp"

    #include "standalone/json.hpp"

    using Json = nlohmann::json;

    ModuleInfo saInfo = {
        .mainClass = "RuntimeApp",
        .apiVer = ELROND_API_VERSION,
        .apiRevision = ELROND_API_REVISION,
        .prettyName = "Elrond Runtime for Linux",
        .authorName = "Edwino Stein",
        .authorEmail = "edwino.stein@gmail.com",
        .version = std::to_string(ELROND_API_VERSION) + "." + std::to_string(ELROND_API_REVISION) + "-alpha"
    };

    void loadApplication(int argc, char const* argv[], elrond::runtime::RuntimeApp& app);
    void stopApplication(elrond::runtime::RuntimeApp& app, bool force, int code);
    void parseModules(elrond::runtime::RuntimeApp& app, Json& cfg);
    void initModules(elrond::runtime::RuntimeApp& app, Json& cfg);
    void parseChmgrs(elrond::runtime::RuntimeApp& app, Json &cfg);
    void readJsonFromFile(String file, Json& json);
    void jsonToCMA(Json &json, elrond::runtime::CustomConfigMapAllocator &cma);
    int main(int argc, char const* argv[]);

#endif
