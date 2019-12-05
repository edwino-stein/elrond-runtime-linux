#if !defined  _ELROND_RUNTIME_CUSTOM_CONFIG_MAP_ALLOCATOR_HPP
    #define _ELROND_RUNTIME_CUSTOM_CONFIG_MAP_ALLOCATOR_HPP

    #include "rtTypes.hpp"

    namespace elrond {
        namespace runtime {

            class CustomConfigMapAllocator : public elrond::config::ConfigMapAllocator {

                protected:
                    Vector<String> strValues;

                public:
                    CustomConfigMapAllocator(elrond::interfaces::ConfigMapMemoryInterface& dm);
                    virtual ~CustomConfigMapAllocator();

                    void push(const char* key, String value);

            };
        }
    }

#endif
