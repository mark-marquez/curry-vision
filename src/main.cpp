#include <iostream>
#include <chrono>
#include <fstream>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <thread>
#include <curryvision/types.hpp>
#include <curryvision/video_stream.hpp>
#include <curryvision/ball_detector.hpp>


using SteadyClock = std::chrono::steady_clock;  
using milliseconds = std::chrono::milliseconds;

constexpr int CAMERA_INDEX = 1;
constexpr int NUM_FRAMES = 500;

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
    VideoStream stream { CAMERA_INDEX, VGA };       
    BallDetector detector {};      
    stream.start();
    stream.show(true);

    auto start = SteadyClock::now();
    for (int i = 0; i < NUM_FRAMES; ++i) {
        Frame frame = stream.get_frame();
        if (frame.width == 0 || frame.height == 0 || frame.data.empty()) continue;
        Ball b = detector.find_ball(frame);
        detector.draw_ball(frame, b);
        stream.display(frame);
    }
    long long total_time_secs = (std::chrono::duration_cast<std::chrono::milliseconds>(SteadyClock::now() - start).count()) / 1000;
    stream.show(false);             
    stream.stop();
    save_benchmark(NUM_FRAMES, total_time_secs);
    return 0;
}


// DEPRACATED TWO THREAD LOGIC
// int main() {
//     VideoStream stream { CAMERA_INDEX, VGA };       
//     BallDetector detector {};      
//     LatestFrame latest;
//     stream.start();
//     stream.show(true);

//     std::thread find([&]{
//         while (auto f = latest.get_frame()) { 
//             auto ball = detector.find_ball(*f); 
//             latest.set_ball(std::move(ball)); 
//         }
//     });

//     auto start = SteadyClock::now();
//     for (int i = 0; i < NUM_FRAMES; ++i) {
//         Frame frame = stream.get_frame();
//         if (frame.width == 0 || frame.height == 0 || frame.data.empty()) continue;

//         latest.set_frame(frame);
//         if (auto b = latest.try_get_ball()) detector.draw_ball(frame, *b);

//         stream.display(frame);
//     }
//     long long total_time_secs = (std::chrono::duration_cast<std::chrono::milliseconds>(SteadyClock::now() - start).count()) / 1000;
//     latest.stop();
//     if (find.joinable()) find.join();
//     stream.show(false);             
//     stream.stop();
//     save_benchmark(NUM_FRAMES, total_time_secs);
//     return 0;
// }