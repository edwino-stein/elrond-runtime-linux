#if !defined  _ELROND_RUNTIME_INTERNAL_MODULE_FACTORY_IPP
    #define _ELROND_RUNTIME_INTERNAL_MODULE_FACTORY_IPP

    #include "InternalModuleFactory.hpp"

    namespace elrond {
        namespace runtime {

            template<class T>
            InternalModuleFactory<T>::InternalModuleFactory(String name):
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
            InternalModuleFactory<T>::~InternalModuleFactory(){}

            template<class T>
            elrond::interfaces::ModuleInterface* InternalModuleFactory<T>::getInstance(String const& instName)
            {
                return new T();
            }

            template<class T>
            void InternalModuleFactory<T>::deleteInstance(elrond::interfaces::ModuleInterface* mod)
            {
                delete mod;
            }
        }
    }


#endif
