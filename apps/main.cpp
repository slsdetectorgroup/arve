
#include "arve/arve.h"
#include <fmt/format.h>
#include <memory>

#include <array>
#include <cerrno>






int main() {
    constexpr size_t num_packets = 128;
    constexpr size_t payload_size = 8192;
    constexpr size_t packet_size = payload_size+sizeof(arve::PacketHeader);

    // arve::UdpSocket sock("127.0.0.1", "50001", packet_size);

    char *buf = new char[num_packets * packet_size];

    arve::FrameGrabber fg("127.0.0.1", "50001", num_packets, packet_size);

    for (int i = 0; i!=3; ++i){
        auto rc = fg.recv_into(buf);
        fmt::print("rc: {}\n", rc);
    }

    auto ptr = reinterpret_cast<arve::PacketBuffer<payload_size>*>(buf);
    for(int i = 0; i!=num_packets; ++i){
        fmt::print("{} - {}\n", ptr->header.frameNumber, ptr->header.packetNumber);
        ++ptr;
    }

    delete[] buf;
}