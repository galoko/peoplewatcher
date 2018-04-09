#ifndef PEOPLEWATCHER_ENGINE_H
#define PEOPLEWATCHER_ENGINE_H

#include <string>
#include <inttypes.h>

extern "C" {
#include "libavutil/frame.h"
}

class Engine {
public:
    static Engine& getInstance() {
        static Engine instance;

        return instance;
    }

    Engine(Engine const&)          = delete;
    void operator=(Engine const&)  = delete;
private:
    Engine(void);

    int initialized;

    static void motionDetectorCallback(AVFrame *yuvFrame);
    void motionDetected(AVFrame* yuvFrame);
public:
    // all these methods should be called from single thread

    void initialize(const char* sdCardPathStr);
    void finalize(void);

    void startRecord(void);
    void stopRecord(void);
    void sendFrame(uint8_t* dataY, uint8_t* dataU, uint8_t* dataV,
                   int strideY, int strideU, int strideV, long long timestamp);
};

#endif //PEOPLEWATCHER_ENGINE_H
