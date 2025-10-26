#include "ftrace.h"
#include <libcamera/control_ids.h>
#include <libcamera/libcamera.h>
#include <print>
#include <thread>

void requestComplete(libcamera::Request* request)
{
    if (request->status() != libcamera::Request::RequestComplete) {
        std::println("Request failed!");
        return;
    }

    const libcamera::ControlList& metadata = request->metadata();

    std::string aeStatus {};
    if (metadata.contains(libcamera::controls::AeState.id())) {
        auto aeState = metadata.get(libcamera::controls::AeState.id()).get<int32_t>();
        switch (aeState) {
        case libcamera::controls::AeStateIdle:
            aeStatus = "Idle";
            break;
        case libcamera::controls::AeStateSearching:
            aeStatus = "Searching";
            break;
        case libcamera::controls::AeStateConverged:
            aeStatus = "Converged";
            break;
        default:
            aeStatus = "Null";
        }
    }

    std::string awbStatus {};
    std::string afStatus {};
    std::string timestamp {};
    std::string sequence {};

    std::println("Frame #{} AeStatus: {} AwbStatus: {} AfbStatus: {} Timestamp: {}", sequence, aeStatus, awbStatus, afStatus, timestamp);
}

auto main(int argc, char* argv[]) -> int
{
    int ret = -1;

    // 1. 初始化 CameraManager
    auto cameraManager = std::make_unique<libcamera::CameraManager>();
    Ftrace::ftrace_duration_begin("cameraManager start");
    ret = cameraManager->start();
    if (ret) {
        std::println("Failed to start CameraManager");
        Ftrace::ftrace_duration_end();
        return ret;
    } else {
        std::println("Successfully started CameraManager");
        Ftrace::ftrace_duration_end();
    }

    // 2. 获取第一个相机
    auto cameras = cameraManager->cameras();
    if (cameras.empty()) {
        std::println("No cameras found!");
        return -1;
    }
    auto& camera = cameras[0];
    Ftrace::ftrace_duration_begin("camera acquire");
    ret = camera->acquire();
    if (ret) {
        std::println("Failed to acquire camera");
        Ftrace::ftrace_duration_end();
        return ret;
    } else {
        std::println("Successfully acquired camera");
        Ftrace::ftrace_duration_end();
    }

    // 3. 配置相机（Still 模式）
    auto roles = { libcamera::StreamRole::StillCapture };
    Ftrace::ftrace_duration_begin("camera generateConfiguration");
    auto config = camera->generateConfiguration(roles);
    if (!config) {
        std::println("Failed to generate configuration");
        Ftrace::ftrace_duration_end();
        camera->release();
        return -1;
    } else {
        std::println("Successfully generated configuration");
        Ftrace::ftrace_duration_end();
    }
    Ftrace::ftrace_duration_begin("camera configure");
    ret = camera->configure(config.get());
    if (ret) {
        std::println("Failed to configure camera");
        Ftrace::ftrace_duration_end();
        camera->release();
        return ret;
    } else {
        std::println("Successfully configured camera");
        Ftrace::ftrace_duration_end();
    }

    // 4. 创建 allocator 并分配 buffer
    auto allocator = std::make_unique<libcamera::FrameBufferAllocator>(camera);
    libcamera::Stream* stream = config->at(0).stream();
    Ftrace::ftrace_duration_begin("allocate buffers");
    ret = allocator->allocate(stream);
    if (ret < 0) {
        std::println("Failed to allocate buffers");
        Ftrace::ftrace_duration_end();
        return ret;
    } else {
        std::println("Successfully allocated buffers");
        Ftrace::ftrace_duration_end();
    }

    // 5. 注册回调
    Ftrace::ftrace_duration_begin("camera connect");
    camera->requestCompleted.connect(requestComplete);
    Ftrace::ftrace_duration_end();

    // 6. 启动相机
    Ftrace::ftrace_duration_begin("camera start");
    ret = camera->start();
    if (ret) {
        std::println("Failed to start camera");
        Ftrace::ftrace_duration_end();
        camera->release();
        return ret;
    } else {
        std::println("Successfully started camera");
        Ftrace::ftrace_duration_end();
    }

    // 8. 等待完成（实际项目中应使用事件循环）
    Ftrace::ftrace_duration_begin("sleep");
    std::this_thread::sleep_for(std::chrono::seconds(5));
    Ftrace::ftrace_duration_end();

    Ftrace::ftrace_duration_begin("camera stop");
    camera->stop();
    std::println("Successfully stoped camera");
    Ftrace::ftrace_duration_end();

    Ftrace::ftrace_duration_begin("camera release");
    camera->release();
    std::println("Successfully released camera");
    Ftrace::ftrace_duration_end();

#if 0
        // 7. 创建并提交 Request
        request_ = camera_->createRequest();
        if (!request_) {
            std::cerr << "Failed to create request" << std::endl;
            return -1;
        }

        const std::vector<std::unique_ptr<libcamera::FrameBuffer>> &buffers = allocator_->buffers(stream);
        request_->addBuffer(stream, buffers[0].get());

        ret = camera_->queueRequest(request_.get());
        if (ret) {
            std::cerr << "Failed to queue request" << std::endl;
            return ret;
        }
#endif

    return 0;
}