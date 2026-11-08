#pragma once

#include "Message.h"
#include <Structs/DiagnosticTrace.h>

#include <Structs/GameId.h>

struct NotifyActivate final : ServerMessage
{
    static constexpr ServerOpcode Opcode = kNotifyActivate;
    NotifyActivate()
        : ServerMessage(Opcode)
    {
    }

    DiagnosticTrace Trace{};

    void SerializeRaw(TiltedPhoques::Buffer::Writer& aWriter) const noexcept override;
    void DeserializeRaw(TiltedPhoques::Buffer::Reader& aReader) noexcept override;

    bool operator==(const NotifyActivate& acRhs) const noexcept { return Trace == acRhs.Trace && GetOpcode() == acRhs.GetOpcode() && Id == acRhs.Id && ActivatorId == acRhs.ActivatorId && PreActivationOpenState == acRhs.PreActivationOpenState; }

    GameId Id;
    uint32_t ActivatorId;
    uint8_t PreActivationOpenState;
};
