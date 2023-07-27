#include "arve/FrameGrabber.h"
#include "arve/Packets.h"
#include "arve/UdpSocket.h"

namespace arve {

FrameGrabber::FrameGrabber(const std::string &node, const std::string &port,
                           size_t num_packets, size_t packet_size)
    : sock(node, port, packet_size), num_packets(num_packets) {}

bool FrameGrabber::recv_into(void *dst) {
    PacketBuffer<8192> packet_buffer; //TODO! not hardcode of course

    // Wait for the first pkg
    while (!sock.receivePacket(&packet_buffer)) {
        if (stopped_) {
            fmt::print("Exit\n");
            return false;
        }
    }

    uint64_t currentFrameNumber = packet_buffer.header.frameNumber;
    int64_t totalFramesReceived = 0;

    //Fill

    return true;
}

} // namespace arve