#pragma once
#include <thread>
#include <vector>

namespace arve {

// Receiver to take data from one or several FrameGrabbers and
template <typename RawFrameType> class UdpReceiver {

    using Grabber = FrameGrabber<RawFrameType>;
    using Fifo = CircularFifo<RawFrameType>;

    Grabber grabber;
    Fifo fifo;
    std::atomic<bool> stopped = false;
    std::thread receiving_thread;

    std::atomic<int64_t> total_lost_packets_{};
    std::atomic<int64_t> complete_frames_{};
    std::atomic<int64_t> last_frame_{};

  public:
    UdpReceiver(const std::string &node, const std::string &port)
        : grabber(node, port){};
    UdpReceiver(const std::string &node, int port)
        : grabber(node, port){};

    ~UdpReceiver() = default;

    auto *fifo_ptr() { return &fifo; }

    void receivePackets() {
        while (!stopped) {
            auto raw_frame = fifo.pop_free();
            auto missing_packets = grabber.recv_into(raw_frame);
            total_lost_packets_ += missing_packets;
            fmt::print("UdpReceiver: Got frame: {}, mp: {}\n",
                       raw_frame[0].header.frameNumber, missing_packets);
            if (!missing_packets) {
                complete_frames_ += 1;
                last_frame_ = raw_frame.frameNumber();
                fifo.push_value(std::move(raw_frame));
                
            }
        }
    }

    int64_t lost_packets(){ return total_lost_packets_;}
    int64_t complete_frames() { return complete_frames_;}
    int64_t last_frame(){return last_frame_;}
    void start() {
        receiving_thread = std::thread(&UdpReceiver::receivePackets, this);
        // &Receiver::receivePackets, r.get(), cpu++);
    }

    void stop() {
        fmt::print("UdpReceiver::request_stop()\n");
        stopped = true;
        grabber.request_stop();
        receiving_thread.join();
        fmt::print("UdpReceiver::stopped!\n");
    }
};

} // namespace arve