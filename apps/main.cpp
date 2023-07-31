
#include "arve/arve.h"
#include <fmt/format.h>
#include <memory>

#include <array>
#include <cerrno>
#include <chrono>



int main() {
    constexpr size_t num_packets = 128;
    constexpr size_t payload_size = 8192;

    // arve::RawFrame<arve::PacketHeader, payload_size, num_packets> data;
    // arve::FrameGrabber<arve::PacketHeader, payload_size, num_packets> fg("127.0.0.1", "50001");

    // for (int i = 0; i!=3; ++i){
    //     auto rc = fg.recv_into(data);
    //     fmt::print("rc: {}\n", rc);
    // }

    // for(int i = 0; i!=num_packets; ++i){
    //     fmt::print("{} - {}\n", data[i].header.frameNumber, data[i].header.packetNumber);
    // }
    using Frame = arve::RawFrame<arve::slsPacketHeader, payload_size, num_packets>;
    using Fifo = arve::CircularFifo<Frame>;

    arve::UdpReceiver<arve::slsPacketHeader, payload_size, num_packets> r("127.0.0.1", "50001");;
    r.start();

    arve::FrameAssembler<Fifo> a;
    a.push_back(r.fifo_ptr());
    a.start();

    using namespace std::chrono_literals;
    std::this_thread::sleep_for(10s);
    r.stop();
    a.stop();
}