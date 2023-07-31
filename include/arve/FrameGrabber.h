#pragma once
#include "arve/Packets.h"
#include "arve/RawFrame.h"
#include "arve/UdpSocket.h"
#include <atomic>
#include <memory>
namespace arve {

// receive a single frame into either an allocated
// or provided buffer

template <typename PacketHeader, size_t PayloadSize, size_t NumPackets>
class FrameGrabber {
    UdpSocket sock;
    // std::atomic<bool> stopped_{false};
    bool stopped_{false};
    PacketBuffer<PacketHeader, PayloadSize> packet_buffer;
    bool first_packet_cached{false};

  public:
    FrameGrabber(const std::string &node, const std::string &port)
        : sock(node, port, sizeof(PacketHeader) + PayloadSize) {
        sock.setBufferSize(1024 * 1024 * 100);
    }

    void swap(FrameGrabber& other){
      sock.swap(other.sock);
      std::swap(stopped_, other.stopped_);
      std::swap(packet_buffer, other.packet_buffer);
      std::swap(first_packet_cached, other.first_packet_cached);
    }

    // No copy since the class manage the underlying socket
    FrameGrabber(const FrameGrabber &) = delete;
    FrameGrabber &operator=(FrameGrabber const &) = delete;

    //Move is fine though
    FrameGrabber(FrameGrabber&& other){
      other.swap(*this);
    }
    FrameGrabber &operator=(FrameGrabber&& other){
      other.swap(*this);
      return *this;
    }

    void request_stop(){
        stopped_ = true;
    }

    RawFrame<PacketHeader, PayloadSize, NumPackets> recv(){
        RawFrame<PacketHeader, PayloadSize, NumPackets> frame;
        recv_into(frame);
        return std::move(frame);
    }

    int recv_into(RawFrame<PacketHeader, PayloadSize, NumPackets> &raw_frame) {
        stopped_ = false;
        auto dst = &raw_frame[0];
        // it could be that we already have the first packet in cache
        // from the last call to recv_into
        if (first_packet_cached) {
            memcpy(dst, &packet_buffer, sizeof(packet_buffer));
        } else {
            // Wait for the first pkg, while loop to allow for timeout
            while (!sock.receivePacket(dst)) {
                if (stopped_) {
                    fmt::print("FrameGrabber stopped!\n");
                    return NumPackets; // All packets lost
                }
            }
        }
        first_packet_cached = false;
        uint64_t currentFrameNumber = dst->header.frameNumber;
        ++dst;

        size_t numPacketsReceived = 1; // We have the first pkg already
        while ((numPacketsReceived < NumPackets) && !stopped_) {
            while (!sock.receivePacket(dst) && !stopped_) {
                fmt::print("Timeout\n");
                stopped_ = true;
                break; //We timed out assume no more pkgs.
            }
            // Already next frame
            if (currentFrameNumber != dst->header.frameNumber) {
                memcpy(&packet_buffer, dst, sizeof(packet_buffer));
                memset(dst, 0, sizeof(packet_buffer));
                first_packet_cached = true;
                break;
            }
            ++numPacketsReceived;
            ++dst;
            // fmt::print("Got {} packets\n", numPacketsReceived);
        }
        return static_cast<int>(NumPackets - numPacketsReceived);
    }

    int multirecv_into(void *dst) { return 5; }
};

} // namespace arve
