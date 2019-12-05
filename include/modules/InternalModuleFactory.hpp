#if !defined  _ELROND_RUNTIME_INTERNAL_MODULE_FACTORY_HPP
    #define _ELROND_RUNTIME_INTERNAL_MODULE_FACTORY_HPP

    #include "rtTypes.hpp"
    #include "ModuleFactory.hpp"

    namespace elrond {
        namespace runtime {

            template<class T>
            class InternalModuleFactory : public elrond::runtime::ModuleFactory {

                public:

                    InternalModuleFactory(String name);
                    virtual ~InternalModuleFactory();

                    elrond::interfaces::ModuleInterface* getInstance() override;
                    void deleteInstance(elrond::interfaces::ModuleInterface* mod) override;
            };
        }
    }

    template<class T>
    elrond::runtime::InternalModuleFactory<T>::InternalModuleFactory(String name):
    ModuleFactory(name)
    {
        this->_info.mainClass = T::_infoMainClassName();
        this->_info.apiVer = T::_infoApiVersion();
        this->_info.apiRevision = T::_infoApiRevision();
        this->_info.prettyName = T::_infoPrettyName();
        this->_info.authorName = T::_infoAuthorName();
        this->_info.authorEmail = T::_infoAuthorEmail();
        this->_info.version = T::_infoVersion();
    }

    template<class T>
    elrond::runtime::InternalModuleFactory<T>::~InternalModuleFactory(){}

    template<class T>
    elrond::interfaces::ModuleInterface* elrond::runtime::InternalModuleFactory<T>::getInstance()
    {
        return new T();
    }

    template<class T>
    void elrond::runtime::InternalModuleFactory<T>::deleteInstance(elrond::interfaces::ModuleInterface* mod)
    {
        delete mod;
    }

#endif
