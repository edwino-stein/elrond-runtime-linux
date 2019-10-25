#if !defined  _ELROND_RUNTIME_CHANNEL_MANAGER_HPP
    #define _ELROND_RUNTIME_CHANNEL_MANAGER_HPP

    #include "rtTypes.hpp"

    class ChannelManager : public elrond::channel::BaseChannelManager {

        protected:

            const elrond::sizeT totalTx;
            const elrond::sizeT totalRx;
            std::unique_ptr<RxChCollectionP[]> rxChannels;
            std::unique_ptr<elrond::byte[]> txBuffer;
            const unsigned long timout;

            void rxTrigger(const elrond::sizeT ch, elrond::word data) override;
            elrond::byte *getTxBuffer() const override;

            bool hasUpdate = false;
            unsigned int tries = 0;
            bool running = false;
            Thread thread;

            static void entryPoint(ChannelManager *cm);
            void sync();

        public:

            ChannelManager(elrond::modules::BaseTransportModule &transport, const elrond::sizeT totalTx, const elrond::sizeT totalRx, const unsigned long timout);
            ~ChannelManager();

            void addRxListener(const elrond::sizeT ch, elrond::channel::RxChannel *rx) override;
            void onSend() override;

            elrond::sizeT getTotalTx() const override;
            elrond::sizeT getTotalRx() const override;

            void run();
            void stop(bool join);

    };

#endif
