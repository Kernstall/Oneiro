//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#pragma once

#include <string>

namespace oe::Renderer
{
    class Texture
    {
        struct Data
        {
            int width{};
            int height{};
            int nrChannels{};
            float ar{};
            uint8_t* data{};
        };
    public:
        ~Texture();
        bool PreLoad(const std::string& path);
        bool Load(const std::string& path);
        void Load();
        void UnLoad();
        void Bind() const;
        void Bind(uint8_t id);
        void UnBind() const;
        [[nodiscard]] const Data& GetData() const;
        bool IsLoaded() const;
    private:
        void GenerateTexture();
        Data mData;
        uint32_t mID{};
        bool mIsLoaded{};
    };
}