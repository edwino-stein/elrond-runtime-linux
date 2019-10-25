#include "channel/ChannelManager.hpp"
#include "channel/RxChCollection.hpp"

using elrond::interfaces::RuntimeInterface;
using elrond::modules::BaseTransportModule;
using elrond::channel::BaseChannelManager;
using elrond::channel::RxChannel;

ChannelManager::ChannelManager(BaseTransportModule &transport, const elrond::sizeT totalTx, const elrond::sizeT totalRx, const unsigned long timout):
BaseChannelManager(transport), totalTx(totalTx), totalRx(totalRx),
rxChannels(new RxChCollectionP[totalRx]), txBuffer(new elrond::byte[ELROND_PROTOCOL_CALC_BUFFER(totalTx)]), timout(timout){
    for(elrond::sizeT i = 0; i < totalRx; ++i) this->rxChannels[i] = nullptr;
}

ChannelManager::~ChannelManager(){}

elrond::byte *ChannelManager::getTxBuffer() const {
    return this->txBuffer.get();
}

void ChannelManager::rxTrigger(const elrond::sizeT ch, elrond::word data){
    if(ch >= this->totalRx) return;
    if(this->rxChannels[ch] == nullptr) return;
    this->rxChannels[ch]->trigger(data);
}

void ChannelManager::addRxListener(const elrond::sizeT ch, RxChannel *rx){
    if(ch >= this->totalRx) return;
    if(this->rxChannels[ch] == nullptr) this->rxChannels[ch] = RxChCollectionP(new RxChCollection());
    this->rxChannels[ch]->push(rx);
}

elrond::sizeT ChannelManager::getTotalTx() const {
    return this->totalTx;
}

elrond::sizeT ChannelManager::getTotalRx() const {
    return this->totalRx;
}

void ChannelManager::onSend(){
    this->hasUpdate = true;
}

void ChannelManager::sync(){

    if(!this->hasUpdate && this->tries++ <= 10) return;

    this->transport.send(this->getTxBuffer(), this->getTxBufferSize());

    this->hasUpdate = false;
    this->tries = 0;
}

void ChannelManager::run(){
    if(this->running) return;
    this->running = true;
    this->thread = Thread(ChannelManager::entryPoint, this);
}

void ChannelManager::stop(bool join){
    if(!this->running) return;

    this->running = false;
    elrond::delay(this->timout);

    if(join && this->thread.joinable()) this->thread.join();
    else this->thread.detach();
}

void ChannelManager::entryPoint(ChannelManager *cm){
    while(cm->running){
        cm->sync();
        if(cm->timout > 0) elrond::delay(cm->timout);
    }
}
