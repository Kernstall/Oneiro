//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#include "Oneiro/Renderer/OpenGL/IndexBuffer.hpp"
#include "OpenGL/gl_core_4_5.hpp"

oe::Renderer::IndexBuffer::~IndexBuffer()
{
    gl::DeleteBuffers(1, &mID);
}

void oe::Renderer::IndexBuffer::Init(const uint32_t* data, uint32_t count)
{
    gl::GenBuffers(1, &mID);
    Bind();
    gl::BufferData(gl::ELEMENT_ARRAY_BUFFER, count, data, gl::STATIC_DRAW);
}

void oe::Renderer::IndexBuffer::Bind() const
{
    gl::BindBuffer(gl::ELEMENT_ARRAY_BUFFER, mID);
}

void oe::Renderer::IndexBuffer::UnBind() const
{
    gl::BindBuffer(gl::ELEMENT_ARRAY_BUFFER, 0);
}