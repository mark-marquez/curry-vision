#include <iostream>
#include <curryvision/video_stream.hpp>
#include <curryvision/ball_detector.hpp>
#include <chrono>


int main() {
    constexpr int CAMERA_INDEX = 1;
    constexpr int NUM_FRAMES = 100;

    VideoStream stream { CAMERA_INDEX };       
    BallDetector detector {};      

    if (!stream.start()) {
        std::cerr << "Failed to start VideoStream\n";
        return 1;
    }

    stream.show(true);             
    using clock = std::chrono::steady_clock;  
    using microseconds = std::chrono::microseconds;
    long long running_sum = 0;

    for (int i = 0; i < NUM_FRAMES; ++i) {
        Frame frame = stream.get_frame();
        if (frame.width == 0 || frame.height == 0) continue;

        auto start = clock::now();
        Ball ball = detector.find_ball(frame);
        auto end = clock::now();
        running_sum += std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

        detector.draw_ball(frame, ball);
        stream.display(frame);
        
        std::cout << "id=" << frame.id
                  << " size=" << frame.width << "x" << frame.height
                  << " bytes=" << frame.data.size() << "\n";
        
    }

    stream.show(false);             
    stream.stop();

    double average = (running_sum / NUM_FRAMES);
    std::cout << "\n=== Baseline ===\n"
                << "Frames measured       : " << NUM_FRAMES << "\n"
                << "Avg find_ball runtime : " << average << " ms per frame\n";
    return 0;
}