#if !defined  _ELROND_RUNTIME_INTERNAL_MODULE_FACTORY_HPP
    #define _ELROND_RUNTIME_INTERNAL_MODULE_FACTORY_HPP

    #include "rtTypes.hpp"
    #include "ModuleFactory.hpp"

    template<class T>
    class InternalModuleFactory : public elrond::runtime::ModuleFactory {

        public:

            InternalModuleFactory(String name);
            virtual ~InternalModuleFactory();

            elrond::interfaces::ModuleInterface *getInstance() override;
            void deleteInstance(elrond::interfaces::ModuleInterface *mod) override;
    };

    template<class T>
    InternalModuleFactory<T>::InternalModuleFactory(String name): ModuleFactory(name){
        this->_info.mainClass = T::_infoMainClassName();
        this->_info.apiVer = T::_infoApiVersion();
        this->_info.apiRevision = T::_infoApiRevision();
        this->_info.prettyName = T::_infoPrettyName();
        this->_info.authorName = T::_infoAuthorName();
        this->_info.authorEmail = T::_infoAuthorEmail();
        this->_info.version = T::_infoVersion();
    }

    template<class T>
    InternalModuleFactory<T>::~InternalModuleFactory(){}

    template<class T>
    elrond::interfaces::ModuleInterface *InternalModuleFactory<T>::getInstance(){
        return new T();
    }

    template<class T>
    void InternalModuleFactory<T>::deleteInstance(elrond::interfaces::ModuleInterface *mod){
        delete mod;
    }

#endif
