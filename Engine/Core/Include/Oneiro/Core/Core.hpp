//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#pragma once

#include "Logger.hpp"
#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"

namespace oe::Core
{
    void Init() {
        if (!glfwInit())
            throw std::runtime_error("Failed to init glfw!");
        Logger::Create("log", "log.txt");
    }
    void Shutdown() { glfwTerminate(); }
}