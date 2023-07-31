#pragma once

#include "arve/Packets.h"
#include <fmt/format.h>
#include <memory>
#include <cstring>


namespace arve{

template<typename Header, size_t PayloadSize, size_t NumPackets>
class RawFrame{
    PacketBuffer<Header, PayloadSize>* packets{};
    int64_t frame_number{-1};

public: 
    RawFrame():packets(new PacketBuffer<Header, PayloadSize>[NumPackets]){}
    ~RawFrame(){
        delete[] packets;
    }
    RawFrame(const RawFrame& other):RawFrame(){
        fmt::print("RawFrame was copy constructed!\n");
        print_addr();
        memcpy(packets, other.packets, (sizeof(Header)+PayloadSize)*NumPackets);
    }

    RawFrame& operator=(const RawFrame& other){
        fmt::print("RawFrame was copy assigned!\n");
        memcpy(packets, other.packets, (sizeof(Header)+PayloadSize)*NumPackets);
        return *this;
    }

    int64_t frameNumber() const{
        return frame_number;
    }

    // Move construction, no allocation just swap the pointer
    // with the other object. The moved from object will have
    // a nullptr and not owning any data 
    RawFrame(RawFrame&& other){
        fmt::print("RawFrame was move constructed!\n");
        std::swap(packets, other.packets);
    }

    RawFrame& operator=(RawFrame&& other){
        fmt::print("RawFrame was move assigned!\n");
        std::swap(packets, other.packets);
        return *this;
    }

    PacketBuffer<Header, PayloadSize>* data(){
        return packets;
    }
    PacketBuffer<Header, PayloadSize>& operator[](size_t i){
        return packets[i];
    }
    void print_addr(){
        fmt::print("RawFrame::packets: {}\n", fmt::ptr(packets));
    }


};

}