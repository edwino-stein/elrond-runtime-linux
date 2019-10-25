#if !defined  _ELROND_RUNTIME_MODULE_HANDLE_HPP
    #define _ELROND_RUNTIME_MODULE_HANDLE_HPP

    #include "rtTypes.hpp"

    class ModuleHandle {

        protected:
            String _name;
            ModuleFactoryP _factory;
            elrond::interfaces::ModuleInterface *_module = nullptr;

            bool running = false;
            Thread thread;
            unsigned long timout = 0;

            static void entryPoint(ModuleHandle * const mh);

        public:

            String const &name;
            elrond::interfaces::ModuleInterface * const &module;
            ModuleFactoryP const &factory;
            bool started = false;

            ModuleHandle(String name, ModuleFactoryP factory);
            virtual ~ModuleHandle();

            void asyncRun();
            void asyncStop(bool join = true);

            void syncLoop();
    };

#endif
