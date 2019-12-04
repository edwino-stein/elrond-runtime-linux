#if !defined  _ELROND_RUNTIME_STANDALONE_DEBUG_OUT_HPP
    #define _ELROND_RUNTIME_STANDALONE_DEBUG_OUT_HPP

    #include "elrond.hpp"

    class OStremDebugOut : public elrond::interfaces::DebugOutInterface {
        protected:
            std::ostream& os;

        public:

            OStremDebugOut(std::ostream& os);
            virtual ~OStremDebugOut();

            const elrond::interfaces::DebugOutInterface& put(const char c[]) const  override;
            const elrond::interfaces::DebugOutInterface& put(char c) const  override;
            const elrond::interfaces::DebugOutInterface& put(unsigned char c) const  override;
            const elrond::interfaces::DebugOutInterface& put(int i) const  override;
            const elrond::interfaces::DebugOutInterface& put(unsigned int i) const  override;
            const elrond::interfaces::DebugOutInterface& put(long l) const  override;
            const elrond::interfaces::DebugOutInterface& put(unsigned long l) const  override;
            const elrond::interfaces::DebugOutInterface& put(double d) const  override;

            const elrond::interfaces::DebugOutInterface& putLn(const char c[]) const override;
            const elrond::interfaces::DebugOutInterface& putLn(char c) const override;
            const elrond::interfaces::DebugOutInterface& putLn(unsigned char c) const override;
            const elrond::interfaces::DebugOutInterface& putLn(int i) const override;
            const elrond::interfaces::DebugOutInterface& putLn(unsigned int i) const override;
            const elrond::interfaces::DebugOutInterface& putLn(long l) const override;
            const elrond::interfaces::DebugOutInterface& putLn(unsigned long l) const override;
            const elrond::interfaces::DebugOutInterface& putLn(double d) const override;
            const elrond::interfaces::DebugOutInterface& putLn(void) const override;

            const elrond::interfaces::DebugOutInterface& flush() const override;
    };


#endif
