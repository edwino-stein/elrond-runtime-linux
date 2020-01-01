#include "modules/Udp.hpp"

#include <cstring>
#include <sys/socket.h>
#include <unistd.h>

using elrond::runtime::Exception;
using elrond::interfaces::ConfigMapInterface;
using elrond::channel::BaseChannelManager;

Udp::UdpSocket::UdpSocket():
host(_host), port(_port){}

void Udp::UdpSocket::init(const int port, String host)
{

    if(this->isRunnig()) return;

    this->_host = host;
    this->_port = port;

    if(!Udp::UdpSocket::isValidPort(this->port))
        throw Exception("Invalid port number.");

    if(!this->isServer()){
        if(!Udp::UdpSocket::isValidIpv4(this->host))
            throw Exception("Invalid IP address.");
    }

    // Create server socket
    if((this->sck = ::socket(AF_INET, SOCK_DGRAM, 0)) == -1)
        throw Exception(strerror(errno));

    ::memset((char *) &this->serverAddr, 0, sizeof(this->clientAddr));
    ::memset((char *) &this->clientAddr, 0, sizeof(this->clientAddr));

    if(this->isServer()){

        this->serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);                   // Any adress available on system
        this->serverAddr.sin_family = AF_INET;                                  // For IPV4 internet protocol
        this->serverAddr.sin_port = htons(this->port);                          // Assign port on which server will listen

        // Bind name & address to the socket
        if((::bind(this->sck, (struct sockaddr *) &this->serverAddr, sizeof(this->serverAddr))) == -1)
            throw Exception(strerror(errno));
    }
    else{
        this->clientAddr.sin_family = AF_INET;                                  //For IPV4 internet protocol
        this->clientAddr.sin_port = htons((int) this->port);                    //Assign port number on which server listening
        this->clientAddr.sin_addr.s_addr = inet_addr(this->host.c_str());       //Assign server machine address
    }

    // Defines timeout
    struct timeval tv;
    tv.tv_sec = 1;
    tv.tv_usec = 0;
    ::setsockopt(this->sck, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof (tv));
}

void Udp::UdpSocket::stop()
{
    if(!this->isRunnig()) return;
    ::close(this->sck);
    this->sck = -1;
}

bool Udp::UdpSocket::receive(elrond::byte data[], const elrond::sizeT len)
{
    if(!this->isRunnig()) return false;

    elrond::sizeT clientSize = sizeof(this->clientAddr);
    return ::recvfrom(
        this->sck,
        data,
        len,
        0,
        (struct sockaddr*) &this->clientAddr,
        (socklen_t*) &clientSize
    ) != -1;
}

bool Udp::UdpSocket::send(elrond::byte data[], const elrond::sizeT len)
{
    if(!this->isRunnig()) return false;

    const elrond::sizeT clientSize = sizeof(this->clientAddr);
    return ::sendto(
        this->sck,
        data,
        len,
        0,
        (struct sockaddr*) &this->clientAddr,
        clientSize
    ) == -1;
}

bool Udp::UdpSocket::isServer() const { return this->host == ""; }
bool Udp::UdpSocket::isRunnig() const { return this->sck != -1; }

bool Udp::UdpSocket::isValidIpv4(String ip)
{
    return ::inet_addr(ip.c_str()) != (in_addr_t)(-1);
}

bool Udp::UdpSocket::isValidPort(const int port)
{
    return port > 0 && port < 65535;
}

Udp::~Udp(){}

void Udp::onInit(ConfigMapInterface& cfg)
{

    if(!cfg.isInt("port")) elrond::error("Invalid or missing key \"port\".");
    int port = cfg.asInt("port");

    String host = "";
    if(cfg.isString("host"))
        host = cfg.asString("host");

    try{
        this->socket.init(port, host);
    }
    catch(Exception &e){
        throw Exception("Unable to initialize the UDP socket", e);
    }

    this->getLoopControl().allow = true;
    this->getLoopControl().async = true;
    this->getLoopControl().time = 10;
}

void Udp::onStart()
{
    if(this->socket.isServer()){
        elrond::dout().put(" * Waiting for connection (127.0.0.0:")
                      .put(this->socket.port)
                      .putLn(")...");
    }
    else{
        elrond::dout().put(" * Trying connection with \"")
                      .put(this->socket.host.c_str())
                      .put(":")
                      .put(this->socket.port)
                      .putLn("\"...");
    }
}

void Udp::onStop()
{
    this->socket.stop();
    elrond::delay(this->getLoopControl().time * 5);
}

void Udp::loop()
{
    if(this->cm == nullptr) return;

    const elrond::sizeT length = this->cm->getRxBufferSize();
    elrond::byte buffer[length];

    if(!this->socket.receive(buffer, length)) return;
    this->cm->onReceive(buffer, length);
}


void Udp::send(elrond::byte data[], const elrond::sizeT length)
{
    this->socket.send(data, length);
}

void Udp::setChannelManager(BaseChannelManager* cm)
{
    if(this->cm == nullptr) this->cm = cm;
}

const char* Udp::_getInternalName()
{
    return "elrond::runtime::Udp";
}

const char* Udp::_infoMainClassName()
{
    return "Udp";
}

int Udp::_infoApiVersion()
{
    return ELROND_API_VERSION;
}

int Udp::_infoApiRevision()
{
    return ELROND_API_REVISION;
}

const char* Udp::_infoPrettyName()
{
    return "UDP Transport";
}

const char* Udp::_infoAuthorName()
{
    return "Edwino Stein";
}

const char* Udp::_infoAuthorEmail()
{
    return "edwino.stein@gmail.com";
}

const char* Udp::_infoVersion()
{
    return "1.0";
}
