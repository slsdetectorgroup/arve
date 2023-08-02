#pragma once
#include <vector>
#include <thread>

namespace arve {

// Receiver to take data from one or several FrameGrabbers and
template <typename PacketHeader, size_t PayloadSize, size_t NumPackets>
class UdpReceiver {
    using Frame = RawFrame<PacketHeader, PayloadSize, NumPackets>;
    using Grabber = FrameGrabber<Frame>;
    using Fifo = CircularFifo<RawFrame<PacketHeader, PayloadSize, NumPackets>>;
    
    Grabber grabber;
    Fifo fifo;
    std::atomic<bool> stopped = false;
    std::thread receiving_thread;

  public:
    UdpReceiver(const std::string& node, const std::string& port):grabber(node, port){

    };
    ~UdpReceiver() = default;

    auto* fifo_ptr()  {return &fifo;}
    void receivePackets(){
        while(!stopped){
            auto raw_frame = fifo.pop_free();
            auto missing_packets = grabber.recv_into(raw_frame);
            fmt::print("Got frame: {}, mp: {}\n", raw_frame[0].header.frameNumber, missing_packets);
            if (!missing_packets){
                fifo.push_value(std::move(raw_frame));
            }
            
        }
    }
    void start(){
        receiving_thread = std::thread(&UdpReceiver::receivePackets, this);
        // &Receiver::receivePackets, r.get(), cpu++);
    }

    void stop(){
        fmt::print("UdpReceiver::request_stop()\n");
        stopped = true;
        grabber.request_stop();
        receiving_thread.join();
        fmt::print("UdpReceiver::stopped!\n");
    }


};

} // namespace arve