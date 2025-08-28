#include <iostream>
#include <curryvision/video_stream.hpp>

int main() {
    VideoStream vs { 1 };              
    if (!vs.start()) {
        std::cerr << "Failed to start VideoStream\n";
        return 1;
    }

    vs.show(true);               

    for (int i = 0; i < 100; ++i) {
        auto f = vs.get_frame();
        if (f.width == 0 || f.height == 0) continue;
        std::cout << "id=" << f.id
                  << " size=" << f.width << "x" << f.height
                  << " bytes=" << f.data.size() << "\n";
    }

    vs.show(false);             
    vs.stop();
    return 0;
}