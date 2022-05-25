//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#pragma once

#include <vector>

#include "Oneiro/Renderer/OpenGL/Sprite2D.hpp"
#include "Oneiro/Scene/SceneManager.hpp"
#include "Oneiro/Scene/Entity.hpp"
#include "HazelAudio/HazelAudio.h"

namespace oe::Lua
{
    class File;
    class Sprite2D;
    class AudioSource;
}

namespace oe::VisualNovel
{
    enum InstructionType : uint8_t
    {
        SHOW_SPRITE,
        HIDE_SPRITE,
        PLAY_MUSIC,
        STOP_MUSIC,
        JUMP_TO_LABEL,
        MOVE_SPRITE,
        SAY_TEXT,
        SET_TEXT_SPEED
    };

    struct Instruction
    {
        InstructionType Type{};
        oe::Renderer::GL::Sprite2D* Sprite2D;
        struct TextData
        {
            std::string Who{};
            std::string What{};
        };
        struct LabelData
        {
            Lua::File* File;
            std::string Name{};
        };
        TextData Text;
        Hazel::Audio::Source* AudioSource{};
        LabelData Label;
        glm::vec3 Vector3{};
    };

    void PushInstruction(Instruction& instruction);
    void PushInstruction(Instruction&& instruction);
    void PushLabel(const std::string& labelName);

    void JumpToLabel(const Lua::File* file, const std::string& labelName);

    void ShowSprite2D(Renderer::GL::Sprite2D* sprite2D);
    void MoveSprite2D(Renderer::GL::Sprite2D* sprite2D, const glm::vec3& pos);
    void HideSprite2D(Renderer::GL::Sprite2D* sprite2D);
    void PlayAudioSource(Hazel::Audio::Source* audioSource);
    void StopAudioSource(Hazel::Audio::Source* audioSource);

    void SetTextSpeed(float speed);
    void LoadTextBox(const std::string& path);

    void Init(const Lua::File* file);
    void NextStep();
    void Update();
}