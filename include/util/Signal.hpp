#if !defined  _ELROND_SIGNAL_HPP
    #define _ELROND_SIGNAL_HPP

    #include "rtTypes.hpp"
    #include <csignal>
    #include <map>


    enum class SIG : int {
        ABRT = SIGABRT,
        FPE = SIGFPE,
        ILL = SIGILL,
        INT = SIGINT,
        SEGV = SIGSEGV,
        TERM = SIGTERM
    };

    class Signal {
        private:
            Signal();

        protected:

            static std::map<enum SIG, signalCallbackT> callbacks;
            static void entryPoint(int signum);

        public:

            static void attach(enum SIG sig, signalCallbackT task);
            static void detach(enum SIG sig);
            static void trigger(enum SIG sig);
            static bool hasAttached(enum SIG sig);

    };

#endif
