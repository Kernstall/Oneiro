//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//


#pragma once

#include <iostream>
#include <Oneiro/Core/Window.hpp>
#include "Oneiro/Runtime/Application.hpp"
#include "Oneiro/Renderer/Renderer.hpp"
#include "Oneiro/Core/Logger.hpp"
#include "Oneiro/Core/Core.hpp"

namespace oe::Runtime
{
    class OE_API Engine
    {
    public:
        static void Init()
        {
            Core::Init();
            Renderer::Init();
        }

        static void Shutdown()
        {
            Renderer::Shutdown();
            Core::Shutdown();
        }

        static void Run(const std::shared_ptr<Application>& app)
        {
            mApplication = app.get();

            mWindow->Create();

            glfwSetKeyCallback(Core::Window::GetGLFWWindow(), Engine::KeyCallback);
            glfwSetMouseButtonCallback(Core::Window::GetGLFWWindow(), Engine::MouseButtonCallback);

            glfwSetFramebufferSizeCallback(Core::Window::GetGLFWWindow(), [](GLFWwindow* window, int width, int height){
                gl::Viewport(0,0, width, height);
                Core::Window::UpdateSize(width, height);
            });

            app->Init();

            while (!mWindow->IsClosed())
            {
                if (app->IsStopped())
                    break;

                Core::Window::PollEvents();

                if (!app->Update())
                    break;

                mWindow->SwapBuffers();
            }

            app->Close();
        }

        static Application* GetApplication() { return mApplication; }

    private:
        static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
        {
            mApplication->HandleKey(static_cast<Input::Key>(key), static_cast<Input::Action>(action));
        }

        static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
        {
            mApplication->HandleButton(static_cast<Input::Button>(button), static_cast<Input::Action>(action));
        }

        static Application* mApplication;
        static Core::Window* mWindow;
    };
}

oe::Core::Window* oe::Runtime::Engine::mWindow{new oe::Core::Window};
oe::Runtime::Application* oe::Runtime::Engine::mApplication{nullptr};