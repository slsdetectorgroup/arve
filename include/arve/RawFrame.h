#pragma once
#include <memory>

namespace arve{

template<typename Header, size_t PayloadSize, size_t NumPackets>
class RawFrame{
    PacketBuffer<Header, PayloadSize>* packets{};

public: 
    RawFrame():packets(new PacketBuffer<Header, PayloadSize>[NumPackets]){}
    ~RawFrame(){
        delete[] packets;
    }
    PacketBuffer<Header, PayloadSize>* data(){
        return packets;
    }
    PacketBuffer<Header, PayloadSize>& operator[](size_t i){
        return packets[i];
    }
};

}