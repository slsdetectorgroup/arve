#pragma once
#include "arve/UdpSocket.h"
#include <atomic>
#include <memory>
namespace arve {

class FrameGrabber {
    UdpSocket sock;
    size_t num_packets;
    std::atomic<bool> stopped_{false};

  public:
    FrameGrabber(const std::string &node, const std::string &port,
                 size_t num_packets, size_t packet_size);
    bool recv_into(void* dst);
};

} // namespace arve
