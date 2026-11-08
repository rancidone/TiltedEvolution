#include <Messages/DialogueRequest.h>

void DialogueRequest::SerializeRaw(TiltedPhoques::Buffer::Writer& aWriter) const noexcept
{
    Trace.Serialize(aWriter);
    Serialization::WriteVarInt(aWriter, ServerId);
    Serialization::WriteString(aWriter, SoundFilename);
    SceneId.Serialize(aWriter);
    Serialization::WriteVarInt(aWriter, ScenePhase);
}

void DialogueRequest::DeserializeRaw(TiltedPhoques::Buffer::Reader& aReader) noexcept
{
    ClientMessage::DeserializeRaw(aReader);
    Trace.Deserialize(aReader);

    ServerId = Serialization::ReadVarInt(aReader) & 0xFFFFFFFF;
    SoundFilename = Serialization::ReadString(aReader);
    SceneId.Deserialize(aReader);
    ScenePhase = Serialization::ReadVarInt(aReader) & 0xFFFFFFFF;
}
