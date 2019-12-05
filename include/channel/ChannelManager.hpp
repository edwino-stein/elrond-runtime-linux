#if !defined  _ELROND_RUNTIME_CHANNEL_MANAGER_HPP
    #define _ELROND_RUNTIME_CHANNEL_MANAGER_HPP

    #include "rtTypes.hpp"

    namespace elrond {
        namespace runtime {

            class ChannelManager : public elrond::channel::BaseChannelManager {

                protected:

                    const elrond::sizeT totalTx;
                    const elrond::sizeT totalRx;
                    std::unique_ptr<RxChCollectionP[]> rxChannels;

                    std::unique_ptr<elrond::byte[]> txBuffer;
                    Mutex txBufferMtx;

                    void rxTrigger(const elrond::sizeT ch, elrond::word data) override;
                    elrond::byte* getTxBuffer() const override;

                    const int txFps;
                    bool running = false;
                    Thread thread;

                    static void entryPoint(ChannelManager* cm);

                public:

                    ChannelManager(elrond::modules::BaseTransportModule& transport, const elrond::sizeT totalTx, const elrond::sizeT totalRx, const unsigned int txFps);
                    ~ChannelManager();

                    void addRxListener(const elrond::sizeT ch, elrond::channel::RxChannel* rx) override;
                    void txTrigger(const elrond::sizeT ch, elrond::word data) override;
                    bool txSync(const bool force) override;

                    elrond::sizeT getTotalTx() const override;
                    elrond::sizeT getTotalRx() const override;

                    void run();
                    void stop(const bool join);
            };
        }
    }

#endif
