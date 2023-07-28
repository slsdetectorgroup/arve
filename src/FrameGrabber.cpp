#include "arve/FrameGrabber.h"

#include "arve/UdpSocket.h"

namespace arve {

FrameGrabber::FrameGrabber(const std::string &node, const std::string &port,
                           size_t num_packets, size_t packet_size)
    : sock(node, port, packet_size), num_packets(num_packets) {
        sock.setBufferSize(1024*1024*100);
    }

int FrameGrabber::recv_into(void *frame_buffer) {
    //Could we receive to the correct position???
    char* dst = static_cast<char*>(frame_buffer);
    // it could be that we already have the first packet in cache
    // from the last call to recv_into
    if (!first_packet_cached){
        // Wait for the first pkg, while loop to allow for timeout
        while (!sock.receivePacket(&packet_buffer)) {
            if (stopped_) {
                fmt::print("FrameGrabber stopped!\n");
                return false;
            }
        }
    }
    first_packet_cached = false;
    uint64_t currentFrameNumber = packet_buffer.header.frameNumber;

    memcpy(dst, &packet_buffer, sizeof(packet_buffer));
    dst += sizeof(packet_buffer);

    //Fill
    size_t numPacketsReceived = 1; //We have the first pkg already
    while((numPacketsReceived < num_packets) && !stopped_){
        while (!sock.receivePacket(&packet_buffer) && !stopped_) {
            fmt::print("Timeout\n");
        }
        //Already next frame
        if (currentFrameNumber != packet_buffer.header.frameNumber){
            first_packet_cached = true;
            break;
        }
        memcpy(dst, &packet_buffer, sizeof(packet_buffer));
        dst += sizeof(packet_buffer);
        ++numPacketsReceived;
        fmt::print("Got {} packets\n", numPacketsReceived);
    }
    return static_cast<int>(num_packets-numPacketsReceived);
}

int FrameGrabber::multirecv_into(void* dst){
    return 5;
}

} // namespace arve