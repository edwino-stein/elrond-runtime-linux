#include "modules/ModuleFactory.hpp"

#include <sstream>

using elrond::runtime::ModuleFactory;
using elrond::runtime::ModuleInfo;
using elrond::interfaces::ModuleInterface;

ModuleFactory::ModuleFactory(String name):
_name(name), name(_name), info(_info){}

ModuleFactory::~ModuleFactory(){}

String ModuleInfo::about() const
{

    std::ostringstream oss;

    oss << this->name();

    if(this->version != "") oss << " v" << this->version;

    oss << " (API " << std::to_string(this->apiVer)
        << "." << std::to_string(this->apiRevision) << ")";

    if(this->authorName != ""){
        oss << " by " << this->authorName;
        if(this->authorEmail != "") oss << " <" << this->authorEmail << ">";
    }

    return oss.str();
}

String ModuleInfo::name() const
{
    return this->prettyName != "" ? this->prettyName : this->mainClass;
}
