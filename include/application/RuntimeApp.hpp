#if !defined  _ELROND_RUNTIME_APP_HPP
    #define _ELROND_RUNTIME_APP_HPP

    #include "rtTypes.hpp"
    #include "application/LoaderRuntimeApp.hpp"

    class RuntimeApp : public LoaderRuntimeApp {

        protected:
            ModuleInfo info;
            bool loop = true;
            static void readJsonFromFile(String file, Json &json);

        public:

            RuntimeApp();
            virtual ~RuntimeApp();

            void init(int argc, char const *argv[]);
            void run();
            void stop(bool force = false);

            elrond::channel::BaseChannelManager &getChannelManager(const elrond::sizeT id) const override;
    };

#endif
