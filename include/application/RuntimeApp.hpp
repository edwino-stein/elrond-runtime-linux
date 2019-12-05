#if !defined  _ELROND_RUNTIME_APP_HPP
    #define _ELROND_RUNTIME_APP_HPP

    #include "rtTypes.hpp"

    namespace elrond {
        namespace runtime {

            class RuntimeApp : public elrond::interfaces::RuntimeInterface {

                protected:

                    elrond::interfaces::DebugOutInterface &_dout;
                    Vector<elrond::runtime::ModuleHandleP> modules;
                    Vector<elrond::runtime::ChannelManagerP> chmgrs;
                    bool _loop;

                    ModuleHandleP findModule(String name) const;
                    void startModules();
                    void stopModules();

                    static ModuleFactoryP findFactory(String name, ModulesFactories& factories, elrond::interfaces::RuntimeInterface* app);

                public:

                    RuntimeApp(elrond::interfaces::DebugOutInterface& dout);
                    virtual ~RuntimeApp();

                    ModuleInfo const& defineModule(String name, String type, ModulesFactories& factories);
                    void initModule(String name, elrond::config::ConfigMap &cm) const;
                    elrond::runtime::ChannelManagerP defineChannelManager(String transport, const elrond::sizeT tx, const elrond::sizeT rx, const elrond::sizeT fps);

                    void start();
                    void loop();
                    void stop(bool force = false);

                    elrond::modules::BaseGpioModule &getGpioService() const override;
                    elrond::modules::BaseInputDriverModule &getInputService(const elrond::sizeT id) const override;
                    elrond::channel::BaseChannelManager& getChannelManager(const elrond::sizeT id) const override;
                    const elrond::interfaces::DebugOutInterface &dout() const override;
                    void onError(const char *error) override;

                    static ModulesFactories newModulesFactories();
            };
        }
    }

#endif
