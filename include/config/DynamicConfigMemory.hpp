#if !defined  _ELROND_RUNTIME_DYNAMIC_CONFIG_MEMORY_HPP
    #define _ELROND_RUNTIME_DYNAMIC_CONFIG_MEMORY_HPP

    #include "elrond.hpp"

    class DynamicConfigMemory : public elrond::interfaces::ConfigMapMemoryInterface {

        protected:
            elrond::byte *data = nullptr;
            elrond::sizeT capacity = 128;
            elrond::sizeT index = 0;
            elrond::sizeT length = 0;

            bool realloc(const elrond::sizeT capacity);

        public:
            DynamicConfigMemory();
            virtual ~DynamicConfigMemory();

            bool alloc(const elrond::sizeT length) override;
            bool write(const elrond::byte data) override;
            bool read(const elrond::sizeT index, elrond::byte &data) const override;
    };

#endif
