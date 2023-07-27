
#include "arve/arve.h"
#include <fmt/format.h>
#include <memory>

#include <array>
#include <cerrno>






int main() {
    constexpr size_t num_packets = 128;
    constexpr size_t payload_size = 8192;
    constexpr size_t packet_size = payload_size+sizeof(PacketHeader);

    // arve::UdpSocket sock("127.0.0.1", "50001", packet_size);

    char *buf = new char[num_packets * packet_size];

    arve::FrameGrabber fg("127.0.0.1", "50001", packet_size, num_packets);
    // while (true) {
    //     auto rc = sock.multirecv<num_packets>(buf);
    //     if (rc == -1) {
    //         fmt::print("errno: {}:{}\n", errno, std::strerror(errno));
    //     }else{
    //         fmt::print("Got: {} packets\n", rc);
    //         auto ptr = reinterpret_cast<PacketBuffer<payload_size>*>(buf);
    //         for(int i = 0; i<rc; i++){
                
    //             fmt::print("{}:{}\n", ptr->header.frameNumber,ptr->header.packetNumber);
    //             ++ptr;
    //         }
    //     }
    // }

    delete buf;
}