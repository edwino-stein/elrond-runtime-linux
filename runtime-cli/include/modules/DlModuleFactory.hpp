#if !defined  _ELROND_RUNTIME_DL_MODULE_FACTORY_STANDALONE_HPP
    #define _ELROND_RUNTIME_DL_MODULE_FACTORY_STANDALONE_HPP

    #include "elrond-runtime.hpp"

    class DlModuleFactory : public elrond::runtime::ModuleFactory {

        protected:

            using dlModCreateT = elrond::interfaces::ModuleInterface* (*)();
            using dlModDestroyT = void (*)(elrond::interfaces::ModuleInterface*);
            using dlModSetAppT = void (*)(elrond::interfaces::RuntimeInterface*);
            using dlModStringHandleT = const char* (*)();
            using dlModIntHandleT = int (*)();

            void* dlHandle = nullptr;
            dlModCreateT _getInstance = nullptr;
            dlModDestroyT _deleteInstance = nullptr;

        public:

            DlModuleFactory(String name, elrond::interfaces::RuntimeInterface* app);
            virtual ~DlModuleFactory();

            elrond::interfaces::ModuleInterface* getInstance(String const& instName) override;
            void deleteInstance(elrond::interfaces::ModuleInterface* mod) override;
    };

#endif
