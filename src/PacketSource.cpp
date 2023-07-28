#include "arve/PacketSource.h"
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <stdexcept>
#include <cstring>
namespace arve {
PacketSource::PacketSource(const std::string &node, const std::string &port) {

    // Create a socket for sending
    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_protocol = 0;
    hints.ai_flags = AI_PASSIVE | AI_ADDRCONFIG;
    struct addrinfo *res = nullptr;


    if (getaddrinfo(node.c_str(), port.c_str(), &hints, &res)) {
        throw std::runtime_error("Failed at getaddrinfo with " + node);
    }
    sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (sockfd == -1) {
        throw std::runtime_error("Failed to create UDP sending socket");
    }

    if (connect(sockfd, res->ai_addr, res->ai_addrlen)) {
        throw std::runtime_error("Failed to connect socket");
    }
    freeaddrinfo(res);
}

PacketSource::~PacketSource() {
    ::shutdown(sockfd, SHUT_RDWR);
    ::close(sockfd);
    sockfd = -1;
}

} // namespace arve