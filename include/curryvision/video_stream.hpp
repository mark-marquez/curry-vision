#pragma once

#include "types.hpp"

class VideoStream {
    public:
        explicit VideoStream(int camera_index = 0);        
        ~VideoStream();

        bool start();
        void stop();
        bool is_running() const; 

        Frame get_frame();

        void show(bool enable);
        

    private:
        bool running_;
        bool display_enabled_;
        uint64_t frame_counter_;
        
};