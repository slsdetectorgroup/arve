
#include "arve/UdpSocket.h"
#include <fmt/format.h>
#include <memory>

#include <array>
#include <cerrno>

int main() {
    constexpr size_t num_packets = 10;
    constexpr size_t packet_size = 1024;

    arve::UdpSocket sock("127.0.0.1", "50556", packet_size);

    char *buf = new char[num_packets * packet_size];

    while (true) {
        auto rc = sock.multirecv<num_packets>(buf);
        if (rc == -1) {
            fmt::print("errno: {}:{}\n", errno, std::strerror(errno));
        }else{
            fmt::print("Got: {} packets\n");
        }
    }

    delete buf;
}