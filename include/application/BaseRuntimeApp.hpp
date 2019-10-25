#if !defined  _ELROND_BASE_RUNTIME_APP_HPP
    #define _ELROND_BASE_RUNTIME_APP_HPP

    #include "rtTypes.hpp"
    #include "util/DebugOut.hpp"

    class BaseRuntimeApp : public elrond::interfaces::RuntimeInterface {

        protected:

            DebugOut dgo;
            static void jsonToCMA(Json &json, CustomConfigMapAllocator &cma);

        public:

            BaseRuntimeApp();
            virtual ~BaseRuntimeApp();

            elrond::modules::BaseGpioModule &getGpioService() const override;
            elrond::modules::BaseInputDriverModule &getInputService(const elrond::sizeT id) const override;
            const elrond::interfaces::DebugOutInterface &dout() const override;
            void onError(const char *error) override;
    };

#endif
