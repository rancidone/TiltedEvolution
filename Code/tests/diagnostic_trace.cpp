#include <TiltedCore/Stl.hpp>
#include <TiltedCore/Allocator.hpp>
#include <TiltedCore/Buffer.hpp>
#include <TiltedCore/Serialization.hpp>
#include <optional>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <catch2/catch.hpp>
#include <Messages/ClientMessageFactory.h>
#include <Messages/ServerMessageFactory.h>
#include <Structs/DiagnosticTrace.h>
#include <limits>

namespace
{
template <class T, class Factory>
void CheckRoundTrip(const DiagnosticTrace& acTrace)
{
    T message{};
    message.Trace = acTrace;
    if constexpr (requires { message.ServerId; })
        message.ServerId = 0x20003A;
    if constexpr (requires { message.OwnershipEpoch; })
        message.OwnershipEpoch = 127;
    if constexpr (requires { message.IsDead; })
    {
        message.Id = 0x600014;
        message.IsDead = true;
    }
    if constexpr (requires { message.ActivatorId; })
    {
        message.Id = {3, 0xFFFF1234};
        message.ActivatorId = 0x200039;
        message.PreActivationOpenState = 3;
    }
    if constexpr (requires { message.CellId; })
        message.CellId = {7, 0x12345};
    if constexpr (requires { message.Item; })
    {
        message.Item.BaseId = {2, 0x64B33};
        message.Item.Count = -3;
        message.Drop = true;
    }
    if constexpr (requires { message.SceneId; })
    {
        message.SceneId = {5, 0x76543};
        message.ScenePhase = 999;
    }
    if constexpr (requires { message.Text; })
    {
        message.Text = "Ughhhh";
        message.TopicFormId = 0x23456;
    }
    if constexpr (requires { message.SoundFilename; })
        message.SoundFilename = "test-voice.fuz";

    TiltedPhoques::Buffer buffer(4096);
    TiltedPhoques::Buffer::Writer writer(&buffer);
    message.Serialize(writer);
    TiltedPhoques::Buffer::Reader reader(&buffer);
    auto decoded = Factory{}.Extract(reader);
    REQUIRE(decoded);
    REQUIRE(decoded->GetOpcode() == message.GetOpcode());
    const auto& received = static_cast<const T&>(*decoded);
    REQUIRE(received.Trace == acTrace);
    REQUIRE(received == message);
}
}

TEST_CASE("Diagnostic correlation survives real message factories", "[encoding.diagnostics]")
{
    // Zero (disabled), high-bit values, and maximum-width varints. Gameplay fields
    // follow the trace on the wire and must also survive without shifts or truncation.
    const DiagnosticTrace traces[] = {{}, {0xFEDCBA9876543210ULL, 0x8123456789ABCDEFULL, 128},
        {std::numeric_limits<uint64_t>::max(), std::numeric_limits<uint64_t>::max(), std::numeric_limits<uint64_t>::max()}};
    for (const auto& trace : traces)
    {
        CheckRoundTrip<RequestInventoryChanges, ClientMessageFactory>(trace);
        CheckRoundTrip<NotifyInventoryChanges, ServerMessageFactory>(trace);
        CheckRoundTrip<RequestDeathStateChange, ClientMessageFactory>(trace);
        CheckRoundTrip<NotifyDeathStateChange, ServerMessageFactory>(trace);
        CheckRoundTrip<ActivateRequest, ClientMessageFactory>(trace);
        CheckRoundTrip<NotifyActivate, ServerMessageFactory>(trace);
        CheckRoundTrip<SubtitleRequest, ClientMessageFactory>(trace);
        CheckRoundTrip<NotifySubtitle, ServerMessageFactory>(trace);
        CheckRoundTrip<DialogueRequest, ClientMessageFactory>(trace);
        CheckRoundTrip<NotifyDialogue, ServerMessageFactory>(trace);
    }
}
