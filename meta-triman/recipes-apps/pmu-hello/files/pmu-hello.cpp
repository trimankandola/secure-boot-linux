// pmu-hello: counts instructions (hardware PMU) for a loop,
// falling back to task-clock (software) if no hardware counter exists
#include <linux/perf_event.h>
#include <sys/ioctl.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <cerrno>
#include <cstdint>
#include <cstring>
#include <iostream>

static int open_counter(uint32_t type, uint64_t config) {
    perf_event_attr attr{};
    attr.size = sizeof(attr);
    attr.type = type;
    attr.config = config;
    attr.disabled = 1;
    attr.exclude_kernel = 1;
    attr.exclude_hv = 1;
    return static_cast<int>(syscall(SYS_perf_event_open, &attr, 0, -1, -1, 0));
}

int main() {
    const char* name = "instructions (hardware PMU)";
    int fd = open_counter(PERF_TYPE_HARDWARE, PERF_COUNT_HW_INSTRUCTIONS);
    if (fd < 0) {
        std::cerr << "hardware counter unavailable: " << std::strerror(errno) << "\n";
        name = "task-clock ns (software)";
        fd = open_counter(PERF_TYPE_SOFTWARE, PERF_COUNT_SW_TASK_CLOCK);
        if (fd < 0) {
            std::cerr << "perf_event_open failed: " << std::strerror(errno)
                      << " (is CONFIG_PERF_EVENTS set?)\n";
            return 1;
        }
    }
    ioctl(fd, PERF_EVENT_IOC_RESET, 0);
    ioctl(fd, PERF_EVENT_IOC_ENABLE, 0);
    volatile uint64_t sum = 0;
    for (uint64_t i = 0; i < 10000000; ++i) sum = sum + i;
    ioctl(fd, PERF_EVENT_IOC_DISABLE, 0);
    uint64_t count = 0;
    if (read(fd, &count, sizeof(count)) != sizeof(count)) {
        std::cerr << "read failed\n";
        return 1;
    }
    close(fd);
    std::cout << "pmu-hello: " << name << " = " << count << "\n";
    return 0;
}
