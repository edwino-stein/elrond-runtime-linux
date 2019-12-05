#if !defined  _ELROND_RT_EXCEPTION_HPP
    #define _ELROND_RT_EXCEPTION_HPP

    #include "rtTypes.hpp"

    #include <exception>

    namespace elrond {
        namespace runtime {

            class Exception {

                protected:

                    std::unique_ptr<Exception> _prev;
                    String _message;

                    Exception();

                    static void dumpStack(std::ostream &oss, const Exception &e, elrond::sizeT &i, const Exception *&le);

                public:

                    String const &message;

                    Exception(const Exception &e);
                    Exception(String message, const Exception &prev);
                    Exception(String message);
                    Exception(const std::exception &e);

                    virtual ~Exception();
                    virtual String what() const;
                    virtual void what(std::ostream &oss) const;
            };
        }
    }

#endif
