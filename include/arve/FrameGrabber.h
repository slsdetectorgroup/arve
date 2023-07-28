#pragma once
#include "arve/UdpSocket.h"
#include "arve/Packets.h"
#include <atomic>
#include <memory>
namespace arve {

// receive a single frame into either an allocated
// or provided buffer

class FrameGrabber {
    UdpSocket sock;
    size_t num_packets;
    std::atomic<bool> stopped_{false};
    PacketBuffer<8192> packet_buffer; //TODO! not hardcode of course
    bool first_packet_cached{false};

  public:
    FrameGrabber(const std::string &node, const std::string &port,
                 size_t num_packets, size_t packet_size);
    int recv_into(void* frame_buffer);
    int multirecv_into(void* dst);
};

} // namespace arve
