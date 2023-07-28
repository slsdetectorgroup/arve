#pragma once
#include "arve/UdpSocket.h"
#include "arve/Packets.h"
#include "arve/RawFrame.h"
#include <atomic>
#include <memory>
namespace arve {

// receive a single frame into either an allocated
// or provided buffer

template<typename PacketHeader, size_t PayloadSize, size_t NumPackets>
class FrameGrabber {
    UdpSocket sock;
    size_t num_packets;
    std::atomic<bool> stopped_{false};
    PacketBuffer<PacketHeader, PayloadSize> packet_buffer; 
    bool first_packet_cached{false};

  public:
    FrameGrabber(const std::string &node, const std::string &port,
                 size_t num_packets, size_t packet_size): sock(node, port, packet_size), num_packets(num_packets) {
        sock.setBufferSize(1024*1024*100);
    }
    
int recv_into(RawFrame<PacketHeader, PayloadSize, NumPackets>& raw_frame) {

    auto dst = &raw_frame[0];
    // it could be that we already have the first packet in cache
    // from the last call to recv_into
    if (first_packet_cached){
      memcpy(dst, &packet_buffer, sizeof(packet_buffer));
    }else{
        // Wait for the first pkg, while loop to allow for timeout
        while (!sock.receivePacket(dst)) {
            if (stopped_) {
                fmt::print("FrameGrabber stopped!\n");
                return NumPackets; //All packets lost
            }
        }
    }
    first_packet_cached = false;
    uint64_t currentFrameNumber = dst->header.frameNumber;
    ++dst;


    size_t numPacketsReceived = 1; //We have the first pkg already
    while((numPacketsReceived < num_packets) && !stopped_){
        while (!sock.receivePacket(dst) && !stopped_) {
            fmt::print("Timeout\n");
        }
        //Already next frame
        if (currentFrameNumber != dst->header.frameNumber){
            memcpy(&packet_buffer, dst, sizeof(packet_buffer));
            memset(dst, 0, sizeof(packet_buffer));
            first_packet_cached = true;
            break;
        }
        // memcpy(dst++, &packet_buffer, sizeof(packet_buffer));
        ++numPacketsReceived;
        ++dst;
        fmt::print("Got {} packets\n", numPacketsReceived);
    }
    return static_cast<int>(num_packets-numPacketsReceived);
}

int multirecv_into(void* dst){
    return 5;
}
};

} // namespace arve
