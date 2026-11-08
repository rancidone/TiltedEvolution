#pragma once

#include <Structs/DiagnosticTrace.h>
#include <spdlog/spdlog.h>
#include <atomic>
#include <chrono>
#include <cstdlib>
#include <cstring>
#include <utility>

namespace CoopDiagnostics
{
struct Configuration
{
    bool Enabled{};
    DiagnosticTrace Process{};
    char Session[33]{};
    std::chrono::steady_clock::time_point Started = std::chrono::steady_clock::now();
};

inline bool ParseId(const char* apValue, uint64_t& aHigh, uint64_t& aLow) noexcept
{
    if (!apValue || std::strlen(apValue) != 32)
        return false;
    aHigh = 0;
    aLow = 0;
    for (size_t i = 0; i < 32; ++i)
    {
        const char c = apValue[i];
        const int digit = c >= '0' && c <= '9' ? c - '0' : c >= 'a' && c <= 'f' ? c - 'a' + 10 : -1;
        if (digit < 0)
            return false;
        uint64_t& half = i < 16 ? aHigh : aLow;
        half = (half << 4) | static_cast<uint64_t>(digit);
    }
    return aHigh != 0 || aLow != 0;
}

inline const Configuration& GetConfiguration() noexcept
{
    static const Configuration config = []
    {
        Configuration result;
        const char* flag = std::getenv("STR_DIAGNOSTICS");
        const char* session = std::getenv("STR_DIAGNOSTICS_SESSION");
        uint64_t sessionHigh{}, sessionLow{};
        if (flag && std::strcmp(flag, "1") == 0
            && ParseId(std::getenv("STR_DIAGNOSTICS_PROCESS"), result.Process.ProcessHigh, result.Process.ProcessLow)
            && ParseId(session, sessionHigh, sessionLow))
        {
            std::memcpy(result.Session, session, 32);
            result.Enabled = true;
        }
        return result;
    }();
    return config;
}

inline DiagnosticTrace NextTrace() noexcept
{
    static std::atomic<uint64_t> sequence{0};
    if (!GetConfiguration().Enabled)
        return {};
    DiagnosticTrace trace = GetConfiguration().Process;
    trace.Sequence = sequence.fetch_add(1, std::memory_order_relaxed) + 1;
    return trace;
}

template <class... Args>
void Log(const DiagnosticTrace& acTrace, const char* apStage, fmt::format_string<Args...> aFormat, Args&&... aArgs) noexcept
{
    const auto& config = GetConfiguration();
    if (!config.Enabled)
        return;
    // Record order is local. UTC is context only; cross-host causality comes from the propagated trace.
    const auto record = NextTrace();
    const auto utc = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    const auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - config.Started).count();
    spdlog::info("CoopDiag v=1 session={} process={:016x}{:016x} record={} utc_us={} elapsed_us={} trace={:016x}{:016x}:{} stage={} {}",
        config.Session, config.Process.ProcessHigh, config.Process.ProcessLow, record.Sequence, utc, elapsed,
        acTrace.ProcessHigh, acTrace.ProcessLow, acTrace.Sequence, apStage, fmt::format(aFormat, std::forward<Args>(aArgs)...));
}
}
