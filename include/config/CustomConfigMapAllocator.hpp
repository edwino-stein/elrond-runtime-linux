#if !defined  _ELROND_RUNTIME_CUSTOM_CONFIG_MAP_ALLOCATOR_HPP
    #define _ELROND_RUNTIME_CUSTOM_CONFIG_MAP_ALLOCATOR_HPP

    #include "elrond.hpp"
    #include "rtTypes.hpp"

    class CustomConfigMapAllocator : public elrond::config::ConfigMapAllocator {

        protected:
            Vector<String> strValues;

        public:
            CustomConfigMapAllocator(elrond::interfaces::ConfigMapMemoryInterface &dm);
            virtual ~CustomConfigMapAllocator();

            void push(const char *key, String value);

    };

#endif
