//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//
#pragma once

#include "Base.hpp"
#include "../Renderer.hpp"
#include "ft2build.h"
#include FT_FREETYPE_H

struct Character
{
    uint32_t TextureID{};
    glm::ivec2 Size{};
    glm::ivec2 Bearing{};
    uint32_t Advance{};
};

namespace oe::Renderer::GL
{
    class Text
    {
    public:
        void Init()
        {
            mVAO.Init();
            mVAO.Bind();
            mVBO.Create(sizeof(GLfloat) * 6 * 4, nullptr, true);
            VertexBuffer::PushLayout(0, 4, 4, 0);
            mVBO.UnBind();
            mVAO.UnBind();

            const char* vertexShaderSrc = R"(
                #version 330 core
                layout (location = 0) in vec4 aPos; // <vec2 pos, vec2 tex_coord>
                out vec2 UV;

                uniform mat4 projection;

                void main()
                {
                    gl_Position = projection * vec4(aPos.xy, 0.0, 1.0);
                    UV = aPos.zw;
                }
            )";

            const char* fragmentShaderSrc = R"(
                #version 330 core
                in vec2 UV;
                out vec4 color;

                uniform sampler2D text;
                uniform vec3 textColor;

                void main()
                {
                    color = vec4(textColor, 1.0) * vec4(1.0, 1.0, 1.0, texture(text, UV).r);
                }
            )";

            mShader.LoadFromSource(vertexShaderSrc, fragmentShaderSrc);

            FT_Library ftLibrary{};

            if (FT_Init_FreeType(&ftLibrary) != 0)
                throw std::runtime_error("Failed to init freetype!");

            FT_Face face{};

            if (FT_New_Face(ftLibrary, "Assets/Fonts/font.ttf", 0, &face) != 0)
            {
            }
            else
            {
                FT_Set_Pixel_Sizes(face, 0, 48);

                gl::PixelStorei(gl::UNPACK_ALIGNMENT, 1);

                for (uint32_t c = 0; c < 128; c++)
                {
                    if ((c >= 0xC0) && (c <= 0xDF))
                    {
                        if (FT_Load_Char(face, 0x0410 + (c - 0xC0), FT_LOAD_RENDER) != 0)
                        {
                            std::cout << "FreeType: failed to load glyph!" << std::endl;
                            continue;
                        }
                    }
                    else if ((c >= 0xE0) && (c <= 0xFF))
                    {
                        if (FT_Load_Char(face, 0x0430 + (c - 0xE0), FT_LOAD_RENDER) != 0)
                        {
                            std::cout << "FreeType: failed to load glyph!" << std::endl;
                            continue;
                        }
                    }
                    else if (c == 0xA8)
                    {
                        if (FT_Load_Char(face, 0x0401, FT_LOAD_RENDER) != 0)
                        {
                            std::cout << "FreeType: failed to load glyph!" << std::endl;
                            continue;
                        }
                    }
                    else if (c == 0xB8)
                    {
                        if (FT_Load_Char(face, 0x0451, FT_LOAD_RENDER) != 0)
                        {
                            std::cout << "FreeType: failed to load glyph!" << std::endl;
                            continue;
                        }
                    }
                    else if (c == 0x85)
                    {
                        if (FT_Load_Char(face, 0x2026, FT_LOAD_RENDER) != 0)
                        {
                            std::cout << "FreeType: failed to load glyph!" << std::endl;
                            continue;
                        }
                    }
                    else
                    {
                        if (FT_Load_Char(face, c, FT_LOAD_RENDER) != 0)
                        {
                            std::cout << "FreeType: failed to load glyph!" << std::endl;
                            continue;
                        }
                    }

                    uint32_t texture{};
                    GL::Texture::GenerateTexture(&texture, 0, face->glyph->bitmap.width,
                                                 face->glyph->bitmap.rows,
                                                 face->glyph->bitmap.buffer, gl::CLAMP_TO_EDGE);

                    Character character = {texture,
                            glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                            glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                            static_cast<unsigned int>(face->glyph->advance.x)};
                    mCharacters.insert(std::pair<char, Character>(c, character));
                }
                gl::BindTexture(gl::TEXTURE_2D, 0);
            }

            FT_Done_Face(face);
            FT_Done_FreeType(ftLibrary);
        }

        void SetString(const std::string& str) { mString = str; }

        void Draw(const glm::vec2& pos = {25.0F, 25.0F},
                  const glm::vec3& color = {1.0F, 0.0F, 0.0F})
        {
            float x = pos.x;
            float y = pos.y;
            mShader.Use();
            mShader.SetUniform("projection",
                               glm::ortho(0.0F, (float) Core::Root::GetWindow()->GetWidth(), 0.0F,
                                          (float) Core::Root::GetWindow()->GetHeight()));
            mShader.SetUniform("textColor", glm::vec3(1.0, 0.0F, 0.0F));

            gl::ActiveTexture(gl::TEXTURE0);
            mVAO.Bind();

            std::string::const_iterator stringIt;
            for (stringIt = mString.begin(); stringIt != mString.end(); stringIt++)
            {
                auto character = mCharacters[*stringIt];

                const float xpos = x + character.Bearing.x * 1.0F;
                const float ypos = y - (character.Size.y - character.Bearing.y) * 1.0F;

                const float width = character.Size.x * 1.0F;
                const float height = character.Size.y * 1.0F;
                const float vertices[6][4] =
                        {{xpos, ypos + height, 0.0, 0.0}, {xpos, ypos, 0.0, 1.0},
                                {xpos + width, ypos, 1.0, 1.0}, {xpos, ypos + height, 0.0, 0.0},
                                {xpos + width, ypos, 1.0, 1.0},
                                {xpos + width, ypos + height, 1.0, 0.0}};
                gl::BindTexture(gl::TEXTURE_2D, character.TextureID);
                mVBO.Bind();
                VertexBuffer::SubData(sizeof(vertices), vertices);
                DrawArrays(TRIANGLES, 6);
                mVBO.UnBind();

                // Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
                x += (character.Advance >> 6) *
                        1.0F; // Bitshift by 6 to get value in pixels (2^6 = 64)
            }
            gl::BindVertexArray(0);
            gl::BindTexture(gl::TEXTURE_2D, 0);
        }
    private:
        std::map<char, Character> mCharacters{};
        std::string mString{};
        Shader mShader{};
        VertexArray mVAO{};
        VertexBuffer mVBO{};
    };
}