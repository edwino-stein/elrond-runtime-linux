#if !defined  _ELROND_RUNTIME_TYPES_HPP
    #define _ELROND_RUNTIME_TYPES_HPP

    #include "elrond.hpp"

    #include <vector>
    #include <string>
    #include <memory>
    #include <functional>
    class Exception;

    using String = std::string;
    template <class T> using Vector = std::vector<T>;
    using signalCallbackT = std::function<void()>;

#endif
