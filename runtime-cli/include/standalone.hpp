#if !defined  _ELROND_RUNTIME_STANDALONE_HPP
    #define _ELROND_RUNTIME_STANDALONE_HPP

    #include "elrond-runtime.hpp"
    #include "json.hpp"

    using Json = nlohmann::json;

    elrond::runtime::ModuleInfo saInfo = {
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
    void pushStandaloneModules(elrond::runtime::ModulesFactoriesV& factories);
    void parseModules(elrond::runtime::RuntimeApp& app, Json& cfg);
    void initModules(elrond::runtime::RuntimeApp& app, Json& cfg);
    void parseChmgrs(elrond::runtime::RuntimeApp& app, Json &cfg);
    void readJsonFromFile(String file, Json& json);
    int main(int argc, char const* argv[]);

#endif
