#pragma once

#include <string>
#include <unistd.h>

//Used for testing

namespace arve {

class PacketSource {
    int sockfd = -1;

  public:
    PacketSource(const std::string &node, const std::string &port);
    ~PacketSource();

    template<typename T>
    int send(T& packet){
        return ::write(sockfd, &packet, sizeof(packet));
    }
};

} // namespace arve