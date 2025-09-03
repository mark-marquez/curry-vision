#pragma once

#include "types.hpp"
#include <vector>

class VideoStream {
    public:
        explicit VideoStream(int camera_index = 0, int resolution = FULLHD);        
        ~VideoStream();

        bool start();
        void stop();
        bool is_running() const; 

        Frame get_frame();

        void show(bool enable);
        void display(const Frame& frame);
        

    private:
        int resolution_;
        bool running_;
        bool display_enabled_;
        uint64_t frame_counter_;
};