#ifndef UTILS_H
#define UTILS_H

#include "stdafx.h"
#include <chrono>
#include <thread>
#include <sstream>

namespace utils {
    namespace rainbow {
        extern inline int r = 0, g = 0, b = 0;
        extern inline uint32_t hue = 0;
        extern inline auto last_tick = std::chrono::high_resolution_clock::now();

        extern inline void tick() {
            auto now = std::chrono::high_resolution_clock::now();
            auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - last_tick);

            if (elapsed.count() < 20) { 
                return;
            }
            last_tick = now;

            hue = (hue + 1) % 360;

            float s = 1.0;
            float v = 1.0;
            float c = s * v;
            float x = c * (1 - fabsf(fmodf(hue / 60.0, 2) - 1));
            float m = v - c;
            float rs, gs, bs;

            if (hue < 60) { rs = c; gs = x; bs = 0; }
            else if (hue < 120) { rs = x; gs = c; bs = 0; }
            else if (hue < 180) { rs = 0; gs = c; bs = x; }
            else if (hue < 240) { rs = 0; gs = x; bs = c; }
            else if (hue < 300) { rs = x; gs = 0; bs = c; }
            else { rs = c; gs = 0; bs = x; }

            r = (rs + m) * 255;
            g = (gs + m) * 255;
            b = (bs + m) * 255;
        }
    }

    inline char* addy2str(void* addr) {
        const void* address = static_cast<const void*>(addr);
        std::stringstream ss;
        ss << _xor_("0x") << std::hex << reinterpret_cast<std::uintptr_t>(address);
        std::string temp = ss.str();

        char* result = new char[temp.length() + 1]; 
        std::strcpy(result, temp.c_str()); 

        return result;
    }
}

#endif