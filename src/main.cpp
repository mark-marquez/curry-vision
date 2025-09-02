#include <iostream>
#include <curryvision/video_stream.hpp>
#include <curryvision/ball_detector.hpp>

int main() {
    VideoStream stream { 1 };       
    BallDetector detector {};      

    if (!stream.start()) {
        std::cerr << "Failed to start VideoStream\n";
        return 1;
    }

    stream.show(true);               

    for (int i = 0; i < 1000; ++i) {
        Frame frame = stream.get_frame();
        if (frame.width == 0 || frame.height == 0) continue;
        Ball ball = detector.find_ball(frame);
        detector.draw_ball(frame, ball);
        stream.display(frame);
        
        std::cout << "id=" << frame.id
                  << " size=" << frame.width << "x" << frame.height
                  << " bytes=" << frame.data.size() << "\n";
        
    }

    stream.show(false);             
    stream.stop();
    return 0;
}