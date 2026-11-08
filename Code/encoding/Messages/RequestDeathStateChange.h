#pragma once

#include "Message.h"
#include <Structs/DiagnosticTrace.h>

struct RequestDeathStateChange final : ClientMessage
{
    static constexpr ClientOpcode Opcode = kRequestDeathStateChange;

    RequestDeathStateChange()
        : ClientMessage(Opcode)
    {
    }

    DiagnosticTrace Trace{};

    void SerializeRaw(TiltedPhoques::Buffer::Writer& aWriter) const noexcept override;
    void DeserializeRaw(TiltedPhoques::Buffer::Reader& aReader) noexcept override;

    bool operator==(const RequestDeathStateChange& acRhs) const noexcept { return Trace == acRhs.Trace && Id == acRhs.Id && OwnershipEpoch == acRhs.OwnershipEpoch && IsDead == acRhs.IsDead && GetOpcode() == acRhs.GetOpcode(); }

    uint32_t Id;
    uint32_t OwnershipEpoch{};
    bool IsDead;
};
