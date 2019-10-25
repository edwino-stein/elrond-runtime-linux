#if !defined  _ELROND_RUNTIME_MODULE_HANDLE_HPP
    #define _ELROND_RUNTIME_MODULE_HANDLE_HPP

    #include "rtTypes.hpp"

    class ModuleHandle {

        protected:
            String _name;
            ModuleFactoryP _factory;
            elrond::interfaces::ModuleInterface *_module = nullptr;


        public:

            String const &name;
            elrond::interfaces::ModuleInterface * const &module;
            ModuleFactoryP const &factory;

            ModuleHandle(String name, ModuleFactoryP factory);
            virtual ~ModuleHandle();

    };

#endif
