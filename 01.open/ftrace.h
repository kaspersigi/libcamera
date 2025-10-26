#pragma once

#include <mutex>
#include <string>

class Ftrace {
public:
    // Duration Events (B/E or X)
    static size_t ftrace_duration_begin(const std::string& message);
    static size_t ftrace_duration_end();
    static size_t ftrace_duration_complete(const std::string& message, uint64_t duration_us);

    // Instant Events (i or I)
    static size_t ftrace_instant(const std::string& message);

    // Counter Events (C)
    static size_t ftrace_counter_set(const std::string& message, size_t count);
    static size_t ftrace_counter_zero(const std::string& message);

    // Async Events (b/n/e or S/T/F)
    static size_t ftrace_async_start(const std::string& message, const std::string& id);
    static size_t ftrace_async_step(const std::string& message, const std::string& id);
    static size_t ftrace_async_end(const std::string& message, const std::string& id);

    // Flow Events (s/t/f)
    static size_t ftrace_flow_start(const std::string& message, const std::string& id);
    static size_t ftrace_flow_step(const std::string& message, const std::string& id);
    static size_t ftrace_flow_end(const std::string& message, const std::string& id);

    // Object Events (N/O/D)
    static size_t ftrace_obj_create(const std::string& obj, const std::string& message);
    static size_t ftrace_obj_snapshot(const std::string& obj, const std::string& message);
    static size_t ftrace_obj_destroy(const std::string& obj);

    // Metadata (M) and Memory Dump (V/v)
    static size_t ftrace_metadata(const std::string& key, const std::string& value);
    static size_t ftrace_memory_dump(bool is_global, const std::string& data);

    // System
    static bool ftrace_enable();
    static void ftrace_disable();

private:
    static int _fd;
    static std::mutex _mutex;
    static size_t _write_to_ftrace(const std::string& str);
};