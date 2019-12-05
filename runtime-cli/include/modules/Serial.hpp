#if !defined  _ELROND_SERIAL_TRANSPORT_STANDALONE_MODULE_HPP
    #define _ELROND_SERIAL_TRANSPORT_STANDALONE_MODULE_HPP

    #include "elrond-runtime.hpp"
    #include <termios.h>

    class Serial : public elrond::modules::BaseTransportModule {

        protected:

            struct termios tty;
            int port = -1;
            elrond::channel::BaseChannelManager* cm = nullptr;

            static speed_t getSpeed(unsigned int speed);

        public:

            virtual ~Serial();

            void onInit(elrond::config::ConfigMap& cfg) override;
            void onStart() override;
            void onStop() override;
            void loop() override;
            void send(elrond::byte data[], const elrond::sizeT length) override;
            void setChannelManager(elrond::channel::BaseChannelManager* cm) override;

            static const char* _getInternalName();
            static const char* _infoMainClassName();
            static int _infoApiVersion();
            static int _infoApiRevision();
            static const char* _infoPrettyName();
            static const char* _infoAuthorName();
            static const char* _infoAuthorEmail();
            static const char* _infoVersion();
    };

#endif
