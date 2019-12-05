#include "channel/ChannelManager.hpp"

#include "channel/RxChCollection.hpp"

using elrond::runtime::ChannelManager;
using elrond::runtime::RxChCollection;
using elrond::runtime::RxChCollectionP;

using elrond::interfaces::RuntimeInterface;
using elrond::modules::BaseTransportModule;
using elrond::channel::BaseChannelManager;
using elrond::channel::RxChannel;

ChannelManager::ChannelManager(BaseTransportModule& transport, const elrond::sizeT totalTx, const elrond::sizeT totalRx, const unsigned int txFps):
BaseChannelManager(transport), totalTx(totalTx), totalRx(totalRx), rxChannels(new RxChCollectionP[totalRx]),
txBuffer(new elrond::byte[ELROND_PROTOCOL_CALC_BUFFER(totalTx)]), txFps(txFps)
{
    for(elrond::sizeT i = 0; i < totalRx; ++i) this->rxChannels[i] = nullptr;
}

ChannelManager::~ChannelManager(){}

elrond::byte *ChannelManager::getTxBuffer() const
{
    return this->txBuffer.get();
}

void ChannelManager::rxTrigger(const elrond::sizeT ch, elrond::word data)
{
    if(ch >= this->totalRx) return;
    if(this->rxChannels[ch] == nullptr) return;
    this->rxChannels[ch]->trigger(data);
}

void ChannelManager::addRxListener(const elrond::sizeT ch, RxChannel *rx)
{
    if(ch >= this->totalRx) return;
    if(this->rxChannels[ch] == nullptr) this->rxChannels[ch] = RxChCollectionP(new RxChCollection());
    this->rxChannels[ch]->push(rx);
}

elrond::sizeT ChannelManager::getTotalTx() const
{
    return this->totalTx;
}

elrond::sizeT ChannelManager::getTotalRx() const
{
    return this->totalRx;
}

void ChannelManager::txTrigger(const elrond::sizeT ch, elrond::word data)
{
    MtxLockGuard mlg(this->txBufferMtx);
    BaseChannelManager::txTrigger(ch, data);
}

bool ChannelManager::txSync(const bool force)
{
    MtxLockGuard mlg(this->txBufferMtx);
    return BaseChannelManager::txSync(force);
}

void ChannelManager::run()
{
    if(this->running) return;
    this->running = true;
    this->thread = Thread(ChannelManager::entryPoint, this);
}

void ChannelManager::stop(const bool join)
{
    if(!this->running) return;

    this->running = false;
    if(this->txFps > 0) elrond::delay(1000/this->txFps);

    if(join && this->thread.joinable()) this->thread.join();
    else this->thread.detach();
}

void ChannelManager::entryPoint(ChannelManager *cm)
{

    const unsigned long delay = cm->txFps > 0 ? 1000/cm->txFps : 0;
    unsigned long lastSync = elrond::millis();

    while(cm->running){
        if(cm->txSync(elrond::millis() - lastSync >= 1000)) lastSync = elrond::millis();
        if(delay > 0) elrond::delay(delay);
    }
}
