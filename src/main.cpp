#include <iostream>
#include <chrono>
#include <fstream>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <curryvision/video_stream.hpp>
#include <curryvision/ball_detector.hpp>

std::string get_timestamp() {
    // Current time
    std::time_t now = std::time(nullptr);
    std::tm local_tm = *std::localtime(&now);

    // Format is YYYY-MM-DD_HH-MM-SS
    std::ostringstream oss;
    oss << std::put_time(&local_tm, "%Y-%m-%d_%H-%M-%S");
    std::string timestamp = oss.str();

    return timestamp;
}


int main() {
    constexpr int CAMERA_INDEX = 1;
    constexpr int NUM_FRAMES = 1000;

    VideoStream stream { CAMERA_INDEX, VGA };       
    BallDetector detector {};      

    if (!stream.start()) {
        std::cerr << "Failed to start VideoStream\n";
        return 1;
    }

    stream.show(true);             
    using clock = std::chrono::steady_clock;  
    using milliseconds = std::chrono::milliseconds;
    long long running_sum = 0;

    for (int i = 0; i < NUM_FRAMES; ++i) {
        Frame frame = stream.get_frame();
        if (frame.width == 0 || frame.height == 0) continue;

        auto start = clock::now();
        Ball ball = detector.find_ball(frame);
        auto end = clock::now();
        running_sum += std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

        detector.draw_ball(frame, ball);
        stream.display(frame);
        
        std::cout << "id=" << frame.id
                  << " size=" << frame.width << "x" << frame.height
                  << " bytes=" << frame.data.size() << "\n";
        
    }

    stream.show(false);             
    stream.stop();

    double average = (running_sum / NUM_FRAMES);
    std::string file_name = "benchmark_" + get_timestamp() + ".txt";
    std::ofstream out(file_name);
    if (out) {
        out << "\n=== Benchmark ===\n"
            << "Frames measured      : " << NUM_FRAMES << "\n"
            << "Avg runtime find_ball: " << average << " ms per frame\n";

    }
    return 0;
}