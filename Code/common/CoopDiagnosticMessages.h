#pragma once

#include <CoopDiagnostics.h>
#include <Messages/RequestInventoryChanges.h>
#include <Messages/NotifyInventoryChanges.h>
#include <Messages/RequestDeathStateChange.h>
#include <Messages/NotifyDeathStateChange.h>
#include <Messages/ActivateRequest.h>
#include <Messages/NotifyActivate.h>
#include <Messages/SubtitleRequest.h>
#include <Messages/NotifySubtitle.h>
#include <Messages/DialogueRequest.h>
#include <Messages/NotifyDialogue.h>

namespace CoopDiagnostics
{
inline const DiagnosticTrace* GetTrace(const ClientMessage& acMessage) noexcept
{
    switch (acMessage.GetOpcode())
    {
    case RequestInventoryChanges::Opcode: return &static_cast<const RequestInventoryChanges&>(acMessage).Trace;
    case RequestDeathStateChange::Opcode: return &static_cast<const RequestDeathStateChange&>(acMessage).Trace;
    case ActivateRequest::Opcode: return &static_cast<const ActivateRequest&>(acMessage).Trace;
    case SubtitleRequest::Opcode: return &static_cast<const SubtitleRequest&>(acMessage).Trace;
    case DialogueRequest::Opcode: return &static_cast<const DialogueRequest&>(acMessage).Trace;
    default: return nullptr;
    }
}
inline const DiagnosticTrace* GetTrace(const ServerMessage& acMessage) noexcept
{
    switch (acMessage.GetOpcode())
    {
    case NotifyInventoryChanges::Opcode: return &static_cast<const NotifyInventoryChanges&>(acMessage).Trace;
    case NotifyDeathStateChange::Opcode: return &static_cast<const NotifyDeathStateChange&>(acMessage).Trace;
    case NotifyActivate::Opcode: return &static_cast<const NotifyActivate&>(acMessage).Trace;
    case NotifySubtitle::Opcode: return &static_cast<const NotifySubtitle&>(acMessage).Trace;
    case NotifyDialogue::Opcode: return &static_cast<const NotifyDialogue&>(acMessage).Trace;
    default: return nullptr;
    }
}
}
