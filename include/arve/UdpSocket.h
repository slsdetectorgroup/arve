#pragma once
#include <string>
#include <cstring>
#include <fmt/format.h>
// #include <netdb.h>
// #include <netinet/in.h>
#include <sys/socket.h>
// #include <unistd.h>
namespace arve {
class UdpSocket {
    int sockfd_ = -1;
    size_t packet_size_;

  public:
    UdpSocket(const std::string &node, const std::string &port,
              size_t packet_size);
    ~UdpSocket();
    bool receivePacket(void *dst);
    void setBufferSize(size_t size);
    size_t bufferSize() const;
    void shutdown();

    //TODO! Does templates really bring anything? (stack allocation of headers...)
    template<size_t NumPackets>
    int multirecv(void *dst){
        struct mmsghdr msgs[NumPackets];
        struct iovec iovecs[NumPackets];
        struct timespec timeout{};
        timeout.tv_sec = 1; //TODO! Configurable, how does this relate to socket timeout
        timeout.tv_nsec = 0;
        memset(msgs, 0, sizeof(msgs));
        for (int i = 0; i < NumPackets; i++) {
            iovecs[i].iov_base         = dst+i*packet_size_;
            iovecs[i].iov_len          = NumPackets;
            msgs[i].msg_hdr.msg_iov    = &iovecs[i];
            msgs[i].msg_hdr.msg_iovlen = 1;
        }

        int rc = recvmmsg(sockfd_, msgs, NumPackets, 0, &timeout);
        return rc;
    }

    // No copy since the class manage the underlying socket
    UdpSocket(const UdpSocket &) = delete;
    UdpSocket &operator=(UdpSocket const &) = delete;

};
} // namespace arve