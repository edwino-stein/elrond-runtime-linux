#if !defined  _ELROND_RUNTIME_MODULE_FACTORY_HPP
    #define _ELROND_RUNTIME_MODULE_FACTORY_HPP

    #include "rtTypes.hpp"

    namespace elrond {
        namespace runtime {

            class ModuleFactory {

                protected:

                    String _name;
                    elrond::runtime::ModuleInfo _info;

                public:

                    String const& name;
                    elrond::runtime::ModuleInfo const& info;

                    ModuleFactory(String name);
                    virtual ~ModuleFactory();

                    virtual elrond::interfaces::ModuleInterface* getInstance()=0;
                    virtual void deleteInstance(elrond::interfaces::ModuleInterface* mod)=0;
            };
        }
    }

#endif
