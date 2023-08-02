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
    int sockfd_{-1};
    size_t packet_size_{0};

  public:
    UdpSocket(const std::string &node, const std::string &port,
              size_t packet_size);
    UdpSocket(const std::string &node, int port,
              size_t packet_size);
    UdpSocket();
    ~UdpSocket();
    bool receivePacket(void *dst);
    void setBufferSize(size_t size);
    size_t bufferSize() const;
    void shutdown();


    //How do we sync on frames in case of packet loss? 
    
    //Would have to use it with the non blocking trick otherwise
    //we risk hanging on packet loss... but how do we deal with the first packet?
    //redo redo redo...?
    //https://man7.org/linux/man-pages/man2/recvmmsg.2.html
    // The timeout argument does not work as intended.  The timeout is
    // checked only after the receipt of each datagram, so that if up to
    // vlen-1 datagrams are received before the timeout expires, but
    // then no further datagrams are received, the call will block
    // forever.

    //Github discussion: supplying the MSG_DONTWAIT flag and setting the 
    //timeout to NULL. When asking recvmmsg to retrieve you 20 packets and 
    //there is one available, it will just read this one packet into your buffers.

    
    template<size_t NumPackets>
    int multirecv(void *dst){
        struct mmsghdr msgs[NumPackets];
        struct iovec iovecs[NumPackets];
        struct timespec timeout{};
        timeout.tv_sec = 1; //TODO! Configurable, how does this relate to socket timeout
        timeout.tv_nsec = 0;
        memset(msgs, 0, sizeof(msgs));
        for (int i = 0; i < NumPackets; i++) {
            iovecs[i].iov_base         = static_cast<char*>(dst)+i*packet_size_;
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

    //Move is fine though
    UdpSocket(UdpSocket&& other);
    UdpSocket &operator=(UdpSocket&& other);

    void swap(UdpSocket& other);

};
} // namespace arve