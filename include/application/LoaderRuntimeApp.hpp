#if !defined  _ELROND_LOADER_RUNTIME_APP_HPP
    #define _ELROND_LOADER_RUNTIME_APP_HPP

    #include "rtTypes.hpp"
    #include "application/BaseRuntimeApp.hpp"
    #include "modules/ModuleFactory.hpp"

    class LoaderRuntimeApp : public BaseRuntimeApp {

        protected:

            static void initInternalFactories(Vector<ModuleFactoryP> &factories);
            static ModuleFactoryP findFactory(String name, Vector<ModuleFactoryP> &factories, elrond::interfaces::RuntimeInterface *app);

            void parseModules(Json &cfg);

        public:

            LoaderRuntimeApp();
            virtual ~LoaderRuntimeApp();
    };


#endif
