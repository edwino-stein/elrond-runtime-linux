#include "application/RuntimeApp.hpp"
#include "util/Signal.hpp"
#include "util/Stacktrace.hpp"
#include "exceptions/Exception.hpp"

elrond::interfaces::RuntimeInterface *elrond::__rtInstance__ = nullptr;

int main(int argc, char const *argv[]) {

    RuntimeApp app;
    elrond::__rtInstance__ = &app;

    Signal::attach(SIG::INT, [&app](){
        std::cout << "\b\b * Received Signal INT (" << (int) SIG::INT << "): ";
        std::cout << "Interrupt" << std::endl;
        app.stop(false);
    });

    Signal::attach(SIG::TERM, [&app](){
        std::cout << "\b\b * Received Signal TERM (" << (int) SIG::TERM << "): ";
        std::cout << "Terminate" << std::endl;
        app.stop(true);
        std::exit(128 + (int) SIG::TERM);
    });

    Signal::attach(SIG::SEGV, [&app](){
        std::cout << std::endl << " * Received Signal SEGV (" << (int) SIG::SEGV << "): ";
        std::cout << "Segmentation fault" << std::endl << std::endl;

        Stacktrace::dump(std::cout, 4);
        std::cout << std::endl;

        app.stop(true);
        std::exit(128 + (int) SIG::SEGV);
    });

    try{
        app.init(argc, argv);
        app.run();
    }
    catch(Exception &e){
        std::cout << std::endl << " * An error occurred" << std::endl;
        e.what(std::cout);
        std::cout << std::endl;
    }

    app.stop(false);
    return 0;
}
