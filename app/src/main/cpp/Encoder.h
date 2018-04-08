#ifndef PEOPLEWATCHER_ENCODER_H
#define PEOPLEWATCHER_ENCODER_H

#include "readerwriterqueue.h"
#include "FFmpegUtils.h"

extern "C" {
#include "libavutil/frame.h"
}

using namespace moodycamel;

class Encoder {
public:
    static Encoder& getInstance() {
        static Encoder instance;

        return instance;
    }

    Encoder(Encoder const&) = delete;
    void operator=(Encoder const&)  = delete;
private:
    Encoder(void);

    enum FrameOperationType {
        EncodeFrame,
        CloseRecord,
        FinalizeEncoder
    };

    struct EncoderOperation {
        FrameOperationType  operationType;
        AVFrame *frame;
    };

    int initialized;

    std::string sdCardPath;

    BlockingReaderWriterQueue<EncoderOperation> pendingOperations;

    FFmpegEncoder encoder;
    FILE *io_file;
    void* io_buffer;

    pthread_t thread;

    static void* thread_entrypoint(void* opaque);
    void threadLoop(void);

    void startEncoding(void);
    void stopEncoding(void);

    void createIO(const char *filePath, AVIOContext **pb);
    void closeIO(void);

    static void io_callback_create(const char *filePath, AVIOContext **pb);
    static int io_callback_write(void *opaque, uint8_t *buf, int buf_size);
public:
    static const int WIDTH  = 640;
    static const int HEIGHT = 480;

    void initialize(const char *sdCardPath);

    void startRecord(void);
    void stopRecord(void);
    bool canAcceptFrame(void);
    void sendFrame(AVFrame* yuvFrame);
    void terminate(void);
};

#endif //PEOPLEWATCHER_ENCODER_H
