
#include "arve/arve.h"
#include <fmt/format.h>
#include <memory>

#include <array>
#include <cerrno>




int main() {
    constexpr size_t num_packets = 128;
    constexpr size_t payload_size = 8192;

    arve::RawFrame<arve::PacketHeader, payload_size, num_packets> data;
    arve::FrameGrabber<arve::PacketHeader, payload_size, num_packets> fg("127.0.0.1", "50001");

    for (int i = 0; i!=3; ++i){
        auto rc = fg.recv_into(data);
        fmt::print("rc: {}\n", rc);
    }

    for(int i = 0; i!=num_packets; ++i){
        fmt::print("{} - {}\n", data[i].header.frameNumber, data[i].header.packetNumber);
    }

}