#if !defined  _ELROND_RUNTIME_UDP_MODULE_HPP
    #define _ELROND_RUNTIME_UDP_MODULE_HPP

    #include "rtTypes.hpp"
    #include <arpa/inet.h>

    class Udp : public elrond::modules::BaseTransportModule {

        protected:

            class UdpSocket {

                protected:

                    String _host = "";
                    int _port = -1;

                    struct sockaddr_in serverAddr;
                    struct sockaddr_in clientAddr;
                    int sck = -1;

                public:

                    String const &host;
                    int const &port;

                    UdpSocket();

                    void init(const int port, String host);

                    void stop();

                    bool receive(elrond::byte data[], const elrond::sizeT len);
                    bool send(elrond::byte data[], const elrond::sizeT len);

                    bool isServer() const;
                    bool isRunnig() const;

                    static bool isValidIpv4(String ip);
                    static bool isValidPort(const int port);
            };

            elrond::channel::BaseChannelManager *cm = nullptr;
            UdpSocket socket;

        public:

            virtual ~Udp();

            void onInit(elrond::config::ConfigMap &cfg) override;
            void onStart() override;
            void onStop() override;
            void loop() override;
            void send(elrond::byte data[], const elrond::sizeT length) override;
            void setChannelManager(elrond::channel::BaseChannelManager *cm) override;

            static const char *_getInternalName();
            static const char *_infoMainClassName();
            static int _infoApiVersion();
            static int _infoApiRevision();
            static const char *_infoPrettyName();
            static const char *_infoAuthorName();
            static const char *_infoAuthorEmail();
            static const char *_infoVersion();
    };


#endif
