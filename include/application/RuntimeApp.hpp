#if !defined  _ELROND_RUNTIME_APP_HPP
    #define _ELROND_RUNTIME_APP_HPP

    #include "rtTypes.hpp"
    #include "application/LoaderRuntimeApp.hpp"

    class RuntimeApp : public LoaderRuntimeApp {

        protected:
            ModuleInfo info;
            bool loop = true;
            Vector<ChannelManagerP> chmgrs;

            static void readJsonFromFile(String file, Json &json);

            void parseOptions(Json &cfg);
            void parseChmgrs(Json &cfg);

        public:

            RuntimeApp();
            virtual ~RuntimeApp();

            void init(int argc, char const *argv[]);
            void run();
            void stop(bool force = false);

            elrond::channel::BaseChannelManager &getChannelManager(const elrond::sizeT id) const override;
    };

#endif
