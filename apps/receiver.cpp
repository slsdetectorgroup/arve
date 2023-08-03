
#include "arve/arve.h"
#include <fmt/format.h>
#include <memory>

#include <array>
#include <cerrno>
#include <chrono>
#include <iostream>

// Single module Jungfrau (maybe try with two ports?)

using namespace arve;


int main() {
    DBG(fmt::print("hello\n"));
    direct_input();
    try {
        // Create the UDP receiver including the fifo for free and filled
        UdpReceiver<JungfrauRawFrame> r("127.0.0.1", 50001);


        // Frame assembler to go with the UdpReceiver
        // in case of multiple sources construct from vector of UdpReceivers
        FrameAssembler a(r);

        r.start();
        a.start();

        // Listen for 'q'
        while (true) {
            auto key = std::cin.get();
            if (key == 'q') {
                fmt::print(fg(fmt::terminal_color::red),
                           "Received \'q\' aborting!\n");
                r.stop();
                a.stop();
                break;
            } else if (key == 's') {
                fmt::print("Complete frames: {}, last frame: {}, lost packets: {}\n",
                           r.complete_frames(), r.last_frame(),
                           r.lost_packets());
            }
        }
    } catch (const std::runtime_error &e) {
        fmt::print(fg(fmt::terminal_color::red), "ERROR: {}\n", e.what());
    }

    reset_terminal();
}