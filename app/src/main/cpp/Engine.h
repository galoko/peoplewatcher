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

    long long lastMotionRealtimeTimestamp;

    void restartRecordIfFramesTooFarApart(long long realtimeTimestamp);

    static void motionDetectorCallback(AVFrame *yuvFrame, long long realtimeTimestamp);
    void motionDetected(AVFrame* yuvFrame, long long realtimeTimestamp);
public:
    // all these methods should be called from single thread

    void initialize(const char* rootDir);
    void finalize(void);

    void startRecord(void);
    void stopRecord(void);
    void sendFrame(uint8_t* dataY, uint8_t* dataU, uint8_t* dataV,
                   int strideY, int strideU, int strideV, long long timestamp);
};

#endif //PEOPLEWATCHER_ENGINE_H
