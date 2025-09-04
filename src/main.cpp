#include <iostream>
#include <chrono>
#include <fstream>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <thread>
#include<curryvision/types.hpp>
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

void save_benchmark(int num_frames, int total_time_secs) {
    std::string file_name = "benchmark_" + get_timestamp() + ".txt";
    std::ofstream out(file_name);
    if (out) {
        out << "\n=== Benchmark ===\n"
            << "Frames measured      : " << num_frames << "\n"
            << "Roughly processed FPS: " << num_frames / total_time_secs << " frames per second\n";
    }
}


int main() {
    constexpr int CAMERA_INDEX = 1;
    constexpr int NUM_FRAMES = 200;

    VideoStream stream { CAMERA_INDEX, VGA };       
    BallDetector detector {};      

    if (!stream.start()) {
        std::cerr << "Failed to start VideoStream\n";
        return 1;
    }
    stream.show(true);
    LatestFrame latest;

    std::thread find([&]{
        while (auto f = latest.get_frame()) { 
            auto ball = detector.find_ball(*f); 
            latest.set_ball(std::move(ball)); 
        }
    });
     
    using clock = std::chrono::steady_clock;  
    using milliseconds = std::chrono::milliseconds;

    auto start = clock::now();
    for (int i = 0; i < NUM_FRAMES; ++i) {
        Frame frame = stream.get_frame();
        if (frame.width == 0 || frame.height == 0 || frame.data.empty()) {
            continue;
        }

        latest.set_frame(frame);
        if (auto b = latest.try_get_ball()) {
            detector.draw_ball(frame, *b);
        }
        stream.display(frame);
    }
    long long total_time_secs = (std::chrono::duration_cast<std::chrono::milliseconds>(clock::now() - start).count()) / 1000;
    latest.stop();
    if (find.joinable()) find.join();
    stream.show(false);             
    stream.stop();
    save_benchmark(NUM_FRAMES, total_time_secs);
    return 0;
}