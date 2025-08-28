#pragma once

#include <cstdint>
#include <vector>

class VideoStream {
    public:
        explicit VideoStream(int camera_index = 0);        
        ~VideoStream();

        bool start();
        void stop();
        bool is_running() const; 

        struct Frame {
            uint64_t id;
            std::uint64_t timestamp_ms;
            int width;
            int height;
            std::vector<unsigned char> data; // raw pixel buffer
        };
        Frame get_frame();

        void show(bool enable);
        

    private:
        bool running_;
        bool display_enabled_;
        uint64_t frame_counter_;
        
};