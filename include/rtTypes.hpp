#if !defined  _ELROND_RUNTIME_TYPES_HPP
    #define _ELROND_RUNTIME_TYPES_HPP

    #include "elrond.hpp"

    #include <vector>
    #include <string>
    #include <memory>
    #include <functional>
    #include <algorithm>
    #include <thread>
    #include <mutex>

    namespace elrond {
        namespace runtime {
            class RuntimeApp;
            class ChannelManager;
            using ChannelManagerP = std::shared_ptr<ChannelManager>;
        }
    }

    class CustomConfigMapAllocator;
    class DynamicConfigMemory;
    class ModuleFactory;
    template<class T> class InternalModuleFactory;
    class DlModuleFactory;
    class ModuleHandle;
    class RxChCollection;
    class Exception;

    class VirtualGpio;
    class Serial;
    class Udp;

    using String = std::string;
    template <class T> using Vector = std::vector<T>;
    using Thread = std::thread;
    using Mutex = std::mutex;
    using MtxLockGuard = std::lock_guard<std::mutex>;
    using ModuleHandleP = std::shared_ptr<ModuleHandle>;
    using RxChCollectionP = std::unique_ptr<RxChCollection>;

    using ModuleFactoryP = std::shared_ptr<ModuleFactory>;
    using ModulesFactories = Vector<ModuleFactoryP>;
    using modCreateT = elrond::interfaces::ModuleInterface* (*)();
    using modDestroyT = void (*)(elrond::interfaces::ModuleInterface *);
    using modSetAppT = void (*)(elrond::interfaces::RuntimeInterface *);
    using modStringHandleT = const char * (*)();
    using modIntHandleT = int (*)();

    struct ModuleInfo {

        String mainClass;
        int apiVer;
        int apiRevision;

        String prettyName;
        String authorName;
        String authorEmail;
        String version;

        String about() const;
        String name() const;
    };

#endif
