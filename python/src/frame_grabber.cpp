#include "arve/FrameGrabber.h"
#include "arve/Packets.h"
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/numpy.h>
#include <string>
namespace py = pybind11;

constexpr size_t JF_NumPackets = 128;
constexpr size_t JF_PayloadSize = 8192;


//Feels very convoluted to get access to packets
using JFG =
    arve::FrameGrabber<arve::slsPacketHeader, JF_PayloadSize, JF_NumPackets>;
using JFRF =
    arve::RawFrame<arve::slsPacketHeader, JF_PayloadSize, JF_NumPackets>;

using JFPB = arve::PacketBuffer<arve::slsPacketHeader, JF_PayloadSize>;
void init_frame_grabber(py::module &m) {

    PYBIND11_NUMPY_DTYPE(arve::slsPacketHeader, frameNumber, expLength, packetNumber,
                         bunchId, timestamp, modId, row, column, reserved,
                         debug, roundRNumber, detType, version);
    PYBIND11_NUMPY_DTYPE(JFPB, header, data);
    
    py::class_<JFRF> JungfrauRawFrame(m, "JungfrauRawFrame");
    JungfrauRawFrame
        .def(py::init<>())
        .def("packet", [](JFRF &a, int i){
            return py::array_t<JFPB>{JF_NumPackets};
        });
        // .def("recv", [](JFG &g) { return g.recv(); });


    py::class_<JFG> JungfrauFrameGrabber(m, "JungfrauFrameGrabber");
    JungfrauFrameGrabber
        .def(py::init<const std::string &, const std::string &>())
        .def("recv", [](JFG &g) { 
            auto frame = g.recv(); 
            py::array_t<JFPB>arr{JF_NumPackets};
            // arr[0] = frame[0];
            return arr;
            });
}