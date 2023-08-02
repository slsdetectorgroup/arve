#include "arve/arve.h"
#include <chrono>
#include <cstdlib>
#include <fmt/chrono.h>
#include <fmt/color.h>
#include <fmt/format.h>
// Simple test that receives frames with a custom packet and payload size
// (this time Jungfrau packets) and displays some statistics

int main(int argc, char** argv) {

    size_t n_frames = 10;

    //Rudimentary parsing
    if(argc == 1){}
    else if(argc == 2){
        n_frames = std::atoi(argv[1]);
    }else{
        fmt::print("Too many arguments\n");
        exit(1);
    }

    // Specify the type and number of packets that makes up the frame
    constexpr size_t num_packets = 128;
    using JungfrauRawFrame =
        arve::RawFrame<arve::JungfrauPacket::header_type,
                       arve::JungfrauPacket::payload_size, num_packets>;

    // Where to listen for packets
    const std::string node = "127.0.0.1";
    const std::string port = "50001";
    arve::FrameGrabber<JungfrauRawFrame> grabber(node, port);

    JungfrauRawFrame frame;
    size_t frames_received{};
    size_t total_missing_packets{};

    fmt::print("Listening to: {} frames\n", n_frames);

    std::chrono::time_point<std::chrono::system_clock> t0;
    while (frames_received < n_frames) {
        int mp = grabber.recv_into(frame);
        auto now = std::chrono::system_clock::now();
        auto sse = now.time_since_epoch();
        if (frames_received == 0)
            t0 = now;
        fmt::print(
            "{:%H:%M:}{:%S} - Received frame: {} with {} missing packets\n",
            now, sse, frame.frameNumber(), mp);
        ++frames_received;
        total_missing_packets += mp;
    }
    auto t1 =
        std::chrono::system_clock::now(); // Could use the value from the loop?
    std::chrono::duration<double> elapsed_seconds = t1 - t0;
    fmt::print(fg(fmt::terminal_color::green),
               "\nReceived a total of {} frames with {} missing packets\n",
               frames_received, total_missing_packets);
    fmt::print("Time between first and last frame: {} -> Average FPS: {}\n",
               elapsed_seconds,
               static_cast<double>(frames_received) / elapsed_seconds.count());
}