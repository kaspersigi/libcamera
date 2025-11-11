#include "ftrace.h"
#include <chrono>
#include <libcamera/control_ids.h>
#include <libcamera/libcamera.h>
#include <spdlog/spdlog.h>
#include <thread>

// sudo chmod 666 /sys/kernel/debug/tracing/trace_marker

std::atomic<int> completedFrames { 0 };
static const int totalFrames = 30;

auto requestComplete(libcamera::Request* request) -> void
{
    if (request->status() != libcamera::Request::RequestComplete) {
        spdlog::error("Request failed!");
        return;
    }

    completedFrames.fetch_add(1);

    const libcamera::ControlList& metadata = request->metadata();

    // --- AE Status ---
    std::string aeStatus = "Null";
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
            aeStatus = "Unknown";
        }
    }

    // --- AWB Status (from draft namespace) ---
    std::string awbStatus = "Null";
    if (metadata.contains(libcamera::controls::draft::AwbState.id())) {
        auto awbState = metadata.get(libcamera::controls::draft::AwbState.id()).get<int32_t>();
        switch (awbState) {
        case libcamera::controls::draft::AwbStateInactive:
            awbStatus = "Inactive";
            break;
        case libcamera::controls::draft::AwbStateSearching:
            awbStatus = "Searching";
            break;
        case libcamera::controls::draft::AwbConverged:
            awbStatus = "Converged";
            break;
        case libcamera::controls::draft::AwbLocked:
            awbStatus = "Locked";
            break;
        default:
            awbStatus = "Unknown";
        }
    }

    // --- AF Status ---
    std::string afStatus = "Null";
    if (metadata.contains(libcamera::controls::AfState.id())) {
        auto afState = metadata.get(libcamera::controls::AfState.id()).get<int32_t>();
        switch (afState) {
        case libcamera::controls::AfStateIdle:
            afStatus = "Idle";
            break;
        case libcamera::controls::AfStateScanning:
            afStatus = "Scanning";
            break;
        case libcamera::controls::AfStateFocused:
            afStatus = "Focused";
            break;
        case libcamera::controls::AfStateFailed:
            afStatus = "Failed";
            break;
        default:
            afStatus = "Unknown";
        }
    }

    // --- Timestamp (SENSOR_TIMESTAMP) ---
    std::string timestamp = "0";
    if (metadata.contains(libcamera::controls::SensorTimestamp.id())) {
        auto ts = metadata.get(libcamera::controls::SensorTimestamp.id()).get<int64_t>();
        timestamp = std::to_string(ts);
    }

    // --- Frame sequence number ---
    // Assume single stream; take the first buffer
    uint32_t sequence = 0;
    if (!request->buffers().empty()) {
        auto buffer = request->buffers().begin()->second;
        sequence = buffer->metadata().sequence;
    }

    spdlog::info("Frame #{} AeStatus: {} AwbStatus: {} AfStatus: {} Timestamp: {}", sequence, aeStatus, awbStatus, afStatus, timestamp);
}

auto main(int argc, char* argv[]) -> int
{
    int ret = -1;
    spdlog::info("Start!");

    // 1. 初始化 CameraManager
    auto cameraManager = std::make_unique<libcamera::CameraManager>();
    Ftrace::ftrace_duration_begin("cameraManager start");
    ret = cameraManager->start();
    if (ret) {
        spdlog::error("Failed to start CameraManager");
        Ftrace::ftrace_duration_end();
        return ret;
    } else {
        spdlog::info("Successfully started CameraManager");
        Ftrace::ftrace_duration_end();
    }

    // 2. 获取第一个相机
    auto cameras = cameraManager->cameras();
    if (cameras.empty()) {
        spdlog::error("No cameras found!");
        return -1;
    }
    auto& camera = cameras[0];
    Ftrace::ftrace_duration_begin("camera acquire");
    ret = camera->acquire();
    if (ret) {
        spdlog::error("Failed to acquire camera");
        Ftrace::ftrace_duration_end();
        return ret;
    } else {
        spdlog::info("Successfully acquired camera");
        Ftrace::ftrace_duration_end();
    }

    // 3. 配置相机
    auto roles = { libcamera::StreamRole::Viewfinder };
    // auto roles = { libcamera::StreamRole::StillCapture };

    Ftrace::ftrace_duration_begin("camera generateConfiguration");
    auto config = camera->generateConfiguration(roles);
    if (!config) {
        spdlog::error("Failed to generate configuration");
        Ftrace::ftrace_duration_end();
        camera->release();
        return -1;
    } else {
        spdlog::info("Successfully generated configuration");
        Ftrace::ftrace_duration_end();
    }
    Ftrace::ftrace_duration_begin("camera configure");
    ret = camera->configure(config.get());
    if (ret) {
        spdlog::error("Failed to configure camera");
        Ftrace::ftrace_duration_end();
        camera->release();
        return ret;
    } else {
        spdlog::info("Successfully configured camera");
        Ftrace::ftrace_duration_end();
    }

    // 4. 创建 allocator 并分配 buffer
    auto allocator = std::make_unique<libcamera::FrameBufferAllocator>(camera);
    libcamera::Stream* stream = config->at(0).stream();
    Ftrace::ftrace_duration_begin("allocate buffers");
    ret = allocator->allocate(stream);
    if (ret < 0) {
        spdlog::error("Failed to allocate buffers");
        Ftrace::ftrace_duration_end();
        return ret;
    } else {
        spdlog::info("Successfully allocated buffers");
        Ftrace::ftrace_duration_end();
    }
    Ftrace::ftrace_duration_begin("allocat FrameBuffer");
    const std::vector<std::unique_ptr<libcamera::FrameBuffer>>& buffers = allocator->buffers(stream);
    if (buffers.empty()) {
        spdlog::error("No buffers allocated for stream!");
        camera->release();
        return -1;
    }
    Ftrace::ftrace_duration_end();

    // 5. 注册回调
    Ftrace::ftrace_duration_begin("camera connect");
    camera->requestCompleted.connect(requestComplete);
    Ftrace::ftrace_duration_end();

    // 6. 启动相机
    Ftrace::ftrace_duration_begin("camera start");
    ret = camera->start();
    if (ret) {
        spdlog::error("Failed to start camera");
        Ftrace::ftrace_duration_end();
        camera->release();
        return ret;
    } else {
        spdlog::info("Successfully started camera");
        Ftrace::ftrace_duration_end();
    }

    // 7. 创建并提交 Request
    std::vector<std::unique_ptr<libcamera::Request>> requests;
    for (int i = 0; i < totalFrames; ++i) {
        auto request = camera->createRequest();
        if (!request) {
            spdlog::error("Failed to create request {}", i);
            break;
        }
        request->addBuffer(stream, buffers[i % buffers.size()].get());
        requests.push_back(std::move(request));
    }

    for (auto& request : requests) {
        camera->queueRequest(request.get());
    }

    Ftrace::ftrace_duration_begin("sleep");
    auto deadline = std::chrono::steady_clock::now() + std::chrono::seconds(5);
    while (completedFrames < totalFrames) {
        if (std::chrono::steady_clock::now() > deadline) {
            spdlog::warn("Timeout waiting for frames, got {}/{}", completedFrames.load(), totalFrames);
            break;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    Ftrace::ftrace_duration_end();

    // 8. 释放资源
    Ftrace::ftrace_duration_begin("camera stop");
    camera->stop();
    spdlog::info("Successfully stopped camera");
    Ftrace::ftrace_duration_end();

    Ftrace::ftrace_duration_begin("camera release");
    camera->release();
    spdlog::info("Successfully released camera");
    Ftrace::ftrace_duration_end();

    spdlog::info("End!");

    return 0;
}