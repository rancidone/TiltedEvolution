#pragma once

#include <TiltedCore/Buffer.hpp>
#include <TiltedCore/Serialization.hpp>

// A server invocation's logging namespace, distributed by the existing login handshake.
struct DiagnosticSessionId
{
    uint64_t High{};
    uint64_t Low{};

    bool operator==(const DiagnosticSessionId&) const noexcept = default;

    void Serialize(TiltedPhoques::Buffer::Writer& aWriter) const noexcept
    {
        TiltedPhoques::Serialization::WriteVarInt(aWriter, High);
        TiltedPhoques::Serialization::WriteVarInt(aWriter, Low);
    }

    void Deserialize(TiltedPhoques::Buffer::Reader& aReader) noexcept
    {
        High = TiltedPhoques::Serialization::ReadVarInt(aReader);
        Low = TiltedPhoques::Serialization::ReadVarInt(aReader);
    }
};
