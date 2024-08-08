#include "utils.h"
#include <fstream>
#include <iostream>
#include <sstream>

int get_display_resolution(int &width, int &height) {
    std::ifstream fb_info("/sys/class/graphics/fb0/virtual_size");
    if (!fb_info.is_open()) {
        std::cerr << "Failed to open framebuffer resolution file" << std::endl;
        return -1;
    }

    std::string resolution;
    std::getline(fb_info, resolution);
    fb_info.close();

    std::istringstream iss(resolution);
    char delimiter;
    if (!(iss >> width >> delimiter >> height) || delimiter != ',') {
        std::cerr << "Failed to parse resolution" << std::endl;
        return -1;
    }

    return 0;
}
