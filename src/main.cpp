#include <iostream>
#include <curryvision/video_stream.hpp>

int main() {
    VideoStream stream { 1 };              
    if (!stream.start()) {
        std::cerr << "Failed to start VideoStream\n";
        return 1;
    }

    stream.show(true);               

    for (int i = 0; i < 100; ++i) {
        auto f = stream.get_frame();
        if (f.width == 0 || f.height == 0) continue;
        std::cout << "id=" << f.id
                  << " size=" << f.width << "x" << f.height
                  << " bytes=" << f.data.size() << "\n";
    }

    stream.show(false);             
    stream.stop();
    return 0;
}