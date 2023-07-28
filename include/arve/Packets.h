#pragma once
// copy of slsDetectorDefs::sls_detector_header; be careful with versions!

namespace arve{

struct PacketHeader {
    uint64_t frameNumber;
    uint32_t expLength; /* Eiger: numsubframes, exptime x 100ns others) */
    uint32_t packetNumber;
    uint64_t bunchId;
    uint64_t timestamp;
    uint16_t modId;
    uint16_t row;
    uint16_t column;
    uint16_t reserved;
    uint32_t debug;
    uint16_t roundRNumber;
    uint8_t detType;
    uint8_t version;
};

template<size_t DataSize>
struct PacketBuffer{
        PacketHeader header;
        char data[DataSize];
};

}