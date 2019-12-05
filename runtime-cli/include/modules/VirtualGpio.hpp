#if !defined  _ELROND_RUNTIME_VIRTUAL_GPIO_HPP
    #define _ELROND_RUNTIME_VIRTUAL_GPIO_HPP

    #include "elrond-runtime.hpp"

    class VirtualGpio : public elrond::modules::BaseGpioModule {

        public:

            virtual ~VirtualGpio();

            void onInit(elrond::config::ConfigMap& cfg) override;
            void attach(elrond::gpio::BaseGpioPin& pin) override;

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
