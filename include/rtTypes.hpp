#if !defined  _ELROND_RUNTIME_TYPES_HPP
    #define _ELROND_RUNTIME_TYPES_HPP

    #include "elrond.hpp"

    #include <vector>
    #include <string>
    #include <memory>
    #include <functional>
    #include <algorithm>

    #include "config/json.hpp"

    class CustomConfigMapAllocator;
    class DynamicConfigMemory;
    class BaseRuntimeApp;
    class LoaderRuntimeApp;
    class RuntimeApp;
    class DebugOut;
    class ModuleFactory;
    template<class T> class InternalModuleFactory;
    class DlModuleFactory;
    class ModuleHandle;
    class RxChCollection;
    class Exception;

    using String = std::string;
    template <class T> using Vector = std::vector<T>;
    using Json = nlohmann::json;
    using ModuleFactoryP = std::shared_ptr<ModuleFactory>;
    using ModuleHandleP = std::shared_ptr<ModuleHandle>;
    using RxChCollectionP = std::unique_ptr<RxChCollection>;
    using signalCallbackT = std::function<void()>;
    using modCreateT = elrond::interfaces::ModuleInterface* (*)();
    using modDestroyT = void (*)(elrond::interfaces::ModuleInterface *);
    using modSetAppT = void (*)(elrond::interfaces::RuntimeInterface *);
    using modStringHandleT = const char * (*)();
    using modIntHandleT = int (*)();

    struct ModuleInfo {

        String mainClass = "";
        int apiVer = 0;
        int apiRevision = 0;

        String prettyName = "";
        String authorName = "";
        String authorEmail = "";
        String version = "";

        String about() const;
        String name() const;
    };

#endif
