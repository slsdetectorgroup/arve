// #include <catch2/catch_all.hpp>
// #include "arve/Packets.h"
// #include "arve/PacketSource.h"
// #include "arve/FrameGrabber.h"
// #include "arve/RawFrame.h"
// #include <array>
// TEST_CASE("Send single packet frame"){
//     struct CustomHeader{
//         int frameNumber{};
//         int packetNumber{};
//     };

//     std::array<int, 25> data;
//     arve::Packet<CustomHeader, sizeof(data)> pbuf;
//     pbuf.header.frameNumber = 73;
//     pbuf.header.packetNumber = 89;
//     constexpr size_t num_packets = 1;
//     const std::string node = "127.0.0.1";
//     const std::string port = "50503";

    
//     //Set up the receiving
//     using Frame = arve::RawFrame<CustomHeader, sizeof(data), num_packets>;
//     arve::RawFrame<CustomHeader, sizeof(data), num_packets> raw_frame;
//     arve::FrameGrabber<CustomHeader, sizeof(data), num_packets> fg(node, port);

//     //Do the sending
//     arve::PacketSource src(node, port);
//     src.send(pbuf);

//     //Did I get my frame? 
//     auto missing_packets = fg.recv_into(raw_frame);
//     CHECK(missing_packets == 0);
//     CHECK(raw_frame[0].header.frameNumber == 73);
//     CHECK(raw_frame[0].header.packetNumber == 89);
// }

// TEST_CASE("Sending 3 packet frame with 1 missing packet"){
//     struct CustomHeader{
//         int frameNumber{};
//         int packetNumber{};
//     };

//     std::array<int, 25> data;
//     arve::Packet<CustomHeader, sizeof(data)> pbuf;
//     pbuf.header.frameNumber = 73;
    
//     constexpr size_t num_packets = 3;
//     const std::string node = "127.0.0.1";
//     const std::string port = "50503";

    
//     //Set up the receiving
//     arve::RawFrame<CustomHeader, sizeof(data), num_packets> raw_frame;
//     arve::FrameGrabber<CustomHeader, sizeof(data), num_packets> fg(node, port);

//     //Do the sending
//     for (size_t i = 0; i!=num_packets-1; ++i){
//         pbuf.header.packetNumber = i;
//         arve::PacketSource src(node, port);
//         src.send(pbuf);
//     }
    

//     //Did I get my frame? 
//     auto missing_packets = fg.recv_into(raw_frame);
//     CHECK(missing_packets == 1);
//     CHECK(raw_frame[0].header.frameNumber == 73);
//     CHECK(raw_frame[0].header.packetNumber == 0);
//     CHECK(raw_frame[1].header.packetNumber == 1);
//     CHECK(raw_frame[2].header.packetNumber == 0);
// }