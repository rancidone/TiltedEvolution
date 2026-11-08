#pragma once

#include <CoopDiagnostics.h>

/**
 * @brief Dispatched whenever an actor talks
 */
struct DialogueEvent
{
    DiagnosticTrace Trace = CoopDiagnostics::NextTrace();
    DialogueEvent(uint32_t aActorID, String aVoiceFile, uint32_t aSceneID, uint32_t aScenePhase)
        : ActorID(aActorID)
        , VoiceFile(aVoiceFile)
        , SceneID(aSceneID)
        , ScenePhase(aScenePhase)
    {
    }

    uint32_t ActorID;
    String VoiceFile;
    uint32_t SceneID; // Scene the line is spoken in, 0 for player dialogue menu lines and lines outside scenes
    uint32_t ScenePhase; // The scene's current phase when the line started
};
