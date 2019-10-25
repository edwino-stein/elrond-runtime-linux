#if !defined  _ELROND_LOADER_RUNTIME_APP_HPP
    #define _ELROND_LOADER_RUNTIME_APP_HPP

    #include "rtTypes.hpp"
    #include "application/BaseRuntimeApp.hpp"

    class LoaderRuntimeApp : public BaseRuntimeApp {
        public:
            LoaderRuntimeApp();
            virtual ~LoaderRuntimeApp();
    };


#endif
