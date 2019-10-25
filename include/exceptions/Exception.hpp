#if !defined  _ELROND_RT_EXCEPTION_HPP
    #define _ELROND_RT_EXCEPTION_HPP

    #include <exception>
    #include "rtTypes.hpp"
    #include "util/Stacktrace.hpp"

    class Exception {

        protected:

            std::unique_ptr<Exception> _prev;
            String _message;
            Stacktrace::SymbolCollectionT stacktrace;

            Exception();

            static void dumpStack(std::ostream &oss, const Exception &e, elrond::sizeT &i, const Exception *&le);

        public:

            String const &message;

            Exception(const Exception &e);
            Exception(String message, const Exception &prev);
            Exception(String message, const elrond::sizeT skipSt = 0);
            Exception(const std::exception &e, const elrond::sizeT skipSt = 0);

            virtual ~Exception();
            virtual String what() const;
            virtual void what(std::ostream &oss) const;
    };

#endif
