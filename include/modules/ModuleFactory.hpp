#if !defined  _ELROND_RUNTIME_MODULE_FACTORY_HPP
    #define _ELROND_RUNTIME_MODULE_FACTORY_HPP

    #include "rtTypes.hpp"

    class ModuleFactory {

        protected:
            String _name;
            ModuleInfo _info;

        public:

            String const& name;
            ModuleInfo const& info;

            ModuleFactory(String name);
            virtual ~ModuleFactory();

            virtual elrond::interfaces::ModuleInterface* getInstance()=0;
            virtual void deleteInstance(elrond::interfaces::ModuleInterface* mod)=0;
    };


#endif
