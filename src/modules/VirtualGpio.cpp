#include "modules/VirtualGpio.hpp"

using elrond::modules::BaseGpioModule;
using elrond::config::ConfigMap;
using elrond::gpio::BaseGpioPin;
using elrond::gpio::DOutPin;
using elrond::gpio::ServoPin;
using elrond::gpio::PwmPin;

VirtualGpio::~VirtualGpio(){}

void VirtualGpio::onInit(ConfigMap &cfg){
    this->getLoopControl().allow = false;
    this->getLoopControl().async = false;
    this->getLoopControl().time = 0;
}

void VirtualGpio::attach(BaseGpioPin &pin){
    switch (pin.getType()) {

        case elrond::GpioType::DOUT:

            pin.setReadHandle([](BaseGpioPin &pin){
                return ((DOutPin &) pin).value;
            });

            pin.setWriteHandle([](elrond::gpio::BaseGpioPin &pin, elrond::word &data){
                ((DOutPin &) pin).value = data;
                std::cout << "#" << pin.getNumber() << " = " << (data > 0 ? "HIGH" : "LOW" ) << std::endl;
            });

        break;

        case elrond::GpioType::PWM:

            pin.setReadHandle([](BaseGpioPin &pin){
                return ((elrond::gpio::PwmPin &) pin).value;
            });

            pin.setWriteHandle([](BaseGpioPin &pin, elrond::word &data){
                ((elrond::gpio::PwmPin &) pin).value = data;
                std::cout << "#" << pin.getNumber() << " = " << data << std::endl;
            });

        break;

        case elrond::GpioType::SERVO:

            pin.setReadHandle([](BaseGpioPin &pin){
                return ((elrond::gpio::ServoPin &) pin).value;
            });

            pin.setWriteHandle([](BaseGpioPin &pin, elrond::word &data){
                ((elrond::gpio::ServoPin &) pin).value = data;
                std::cout << "#" << pin.getNumber() << " = " << elrond::map(data, LOW_VALUE, HIGH_VALUE, 0, 180)  << std::endl;
            });

        break;

        case elrond::GpioType::UNKNOWN:
        default:
            elrond::error("GPIO type error");
    }
}

const char *VirtualGpio::_getInternalName(){
    return "elrond::runtime::VirtualGpio";
}

const char *VirtualGpio::_infoMainClassName(){
    return "VirtualGpio";
}

int VirtualGpio::_infoApiVersion(){
    return ELROND_API_VERSION;
}

int VirtualGpio::_infoApiRevision(){
    return ELROND_API_REVISION;
}

const char *VirtualGpio::_infoPrettyName(){
    return "Virtual Gpio";
}

const char *VirtualGpio::_infoAuthorName(){
    return "Edwino Stein";
}

const char *VirtualGpio::_infoAuthorEmail(){
    return "edwino.stein@gmail.com";
}

const char *VirtualGpio::_infoVersion(){
    return "0.1";
}
