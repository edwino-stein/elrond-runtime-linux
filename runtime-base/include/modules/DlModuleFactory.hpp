#if !defined  _ELROND_RUNTIME_DL_MODULE_FACTORY_HPP
    #define _ELROND_RUNTIME_DL_MODULE_FACTORY_HPP

    #include "rtTypes.hpp"
    #include "ModuleFactory.hpp"

    namespace elrond {
        namespace runtime {

            class DlModuleFactory : public elrond::runtime::ModuleFactory {

                protected:

                    void* dlHandle = nullptr;
                    dlModCreateT _getInstance = nullptr;
                    dlModDestroyT _deleteInstance = nullptr;

                public:

                    DlModuleFactory(String name, elrond::interfaces::RuntimeInterface* app);
                    virtual ~DlModuleFactory();

                    elrond::interfaces::ModuleInterface* getInstance() override;
                    void deleteInstance(elrond::interfaces::ModuleInterface* mod) override;
            };
        }
    }

#endif
