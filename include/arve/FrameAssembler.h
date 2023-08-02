#pragma once

#include "arve/utils.h"

namespace arve {

// Assemble frames from multiple UdpReceiver and copies all data to
// the right place
template <typename RawFrameType> class FrameAssembler {
  public:
    using fifo_type = CircularFifo<RawFrameType>;

  private:
    std::vector<fifo_type *> fifos;
    std::atomic<bool> stopped = false;
    std::thread processing_thread;

  public:
    FrameAssembler(){};
    FrameAssembler(UdpReceiver<RawFrameType>& r){
        fifos.push_back(r.fifo_ptr());
    }

    void assemble() {
        std::vector<int64_t> frame_numbers(
            fifos.size()); // template frame numbers
        std::vector<RawFrameType> frames(fifos.size());
        using namespace std::chrono_literals;
        auto wait = 5000ns;
        while (!stopped) {
            // copy out all frame numbers
            for (size_t i = 0; i < fifos.size(); ++i) {
                frames[i] = fifos[i]->pop_value(wait, stopped);
                if (stopped)
                    return;
                frame_numbers[i] = frames[i].frameNumber();
            }
            // Drop the lowest frame until we are at the same position
            while (!allEqual(frame_numbers)) {
                auto i = std::min_element(frame_numbers.begin(),
                                          frame_numbers.end()) -
                         frame_numbers.begin();

                // push pop and frame number
                fifos[i]->push_free(std::move(frames[i]));
                frames[i] = fifos[i]->pop_value();
                frame_numbers[i] = frames[i].frameNumber();
            }

            fmt::print("Processing frame: {}\n", frame_numbers[0]);

            for (size_t i = 0; i < fifos.size(); ++i) {
                // Do something with the frame
                fifos[i]->push_free(std::move(frames[i]));
            }
        }
    }

    void push_back(fifo_type *ptr) { fifos.push_back(ptr); }
    void stop() {
        stopped = true;
        processing_thread.join();
    }
    void start() {
        processing_thread = std::thread(&FrameAssembler::assemble, this);
    }
};

} // namespace arve