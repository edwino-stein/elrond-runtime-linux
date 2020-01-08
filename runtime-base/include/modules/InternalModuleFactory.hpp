#if !defined  _ELROND_RUNTIME_INTERNAL_MODULE_FACTORY_HPP
    #define _ELROND_RUNTIME_INTERNAL_MODULE_FACTORY_HPP

    #include "rtTypes.hpp"
    #include "ModuleFactory.hpp"

    namespace elrond {
        namespace runtime {

            template<class T>
            class InternalModuleFactory : public elrond::runtime::ModuleFactory {

                public:

                    InternalModuleFactory(String name);
                    virtual ~InternalModuleFactory();

                    elrond::interfaces::ModuleInterface* getInstance(String const& instName) override;
                    void deleteInstance(elrond::interfaces::ModuleInterface* mod) override;
            };
        }
    }

#endif
