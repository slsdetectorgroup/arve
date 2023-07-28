#pragma once
#include <vector>

namespace arve {

// Receiver to take data from one or several FrameGrabbers and
template <typename PacketHeader, size_t PayloadSize, size_t NumPackets>
class Receiver {
    std::vector<FrameGrabber<PacketHeader, PayloadSize, NumPackets>> grabbers;

  public:
    Receiver(){};

    template <class... Args> auto emplace_back(Args &&...args) {
        grabbers.emplace_back(std::forward<Args>(args)...);
        // grabbers.emplace_back("127.0.0.1", "50001");
    }

    void start(){
        RawFrame<PacketHeader, PayloadSize, NumPackets> raw_frame;
        while(true){
            grabbers[0].recv_into(raw_frame);
            fmt::print("Got frame: {}\n", raw_frame[0].header.frameNumber);
        }
    }


};

} // namespace arve