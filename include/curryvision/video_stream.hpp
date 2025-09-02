#pragma once

#include "types.hpp"
#include <vector>

class VideoStream {
    public:
        static constexpr int QVGA = 0;
        static constexpr int VGA = 1; 
        static constexpr int HD = 2;
        static constexpr int FULLHD = 3;

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
        std::vector<int> widths = {320, 640, 1280, 1920};
        std::vector<int> heights = {240, 480, 720, 1080};
};