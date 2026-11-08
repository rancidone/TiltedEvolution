#pragma once

#include <TiltedCore/Buffer.hpp>
#include <TiltedCore/Serialization.hpp>

// Observability only: this identifies an event, not a replicated object or a transaction.
// The launcher supplies a fresh 128-bit process ID for each process invocation.
struct DiagnosticTrace
{
    uint64_t ProcessHigh{};
    uint64_t ProcessLow{};
    uint64_t Sequence{};

    bool operator==(const DiagnosticTrace&) const noexcept = default;

    void Serialize(TiltedPhoques::Buffer::Writer& aWriter) const noexcept
    {
        TiltedPhoques::Serialization::WriteVarInt(aWriter, ProcessHigh);
        TiltedPhoques::Serialization::WriteVarInt(aWriter, ProcessLow);
        TiltedPhoques::Serialization::WriteVarInt(aWriter, Sequence);
    }

    void Deserialize(TiltedPhoques::Buffer::Reader& aReader) noexcept
    {
        ProcessHigh = TiltedPhoques::Serialization::ReadVarInt(aReader);
        ProcessLow = TiltedPhoques::Serialization::ReadVarInt(aReader);
        Sequence = TiltedPhoques::Serialization::ReadVarInt(aReader);
    }
};
