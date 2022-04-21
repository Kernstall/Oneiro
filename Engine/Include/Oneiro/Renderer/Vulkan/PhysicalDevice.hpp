#pragma once

#include "Oneiro/Core/Root.hpp"
#include "WindowSurface.hpp"
#include <optional>

namespace oe::Renderer::Vulkan
{
    class PhysicalDevice
    {
    public:
        static VkSurfaceCapabilitiesKHR GetSurfaceCapabilities(VkPhysicalDevice device);

        static std::vector<VkSurfaceFormatKHR> GetSurfaceFormats(VkPhysicalDevice device);

        static std::vector<VkPresentModeKHR> GetPresentModes(VkPhysicalDevice device);

        bool IsDeviceSuitable(VkPhysicalDevice device);

        bool CheckDeviceExtensionsSupport(VkPhysicalDevice device) const;

        size_t RateDeviceSuitability(VkPhysicalDevice device);

        void Create();

        struct QueueFamilyIndices {
            std::optional<uint32_t> GraphicsFamily;
            std::optional<uint32_t> PresentFamily;

            [[nodiscard]] bool IsComplete() const;
        };

        static QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device);

        [[nodiscard]] VkPhysicalDevice Get() const;
        [[nodiscard]] const VkPhysicalDevice* GetPtr() const;
        [[nodiscard]] VkPhysicalDeviceFeatures GetFeatures() const;
        [[nodiscard]] const VkPhysicalDeviceFeatures* GetFeaturesPtr() const;
        [[nodiscard]] const QueueFamilyIndices& GetQueueFamilyIndices() const;
        [[nodiscard]] const std::vector<VkDeviceQueueCreateInfo>& GetQueueInfos() const;
        [[nodiscard]] const std::vector<const char*>& GetExtensions() const;
    private:
        VkPhysicalDevice mPhysicalDevice{};
        VkPhysicalDeviceProperties mDeviceProperties{};
        VkPhysicalDeviceFeatures mDeviceFeatures{};
        QueueFamilyIndices mQueueFamilyIndices{};
        VkQueue mPresentQueue{};
        std::vector<VkDeviceQueueCreateInfo> mQueueCreateInfos;
        std::vector<const char*> mDeviceExtensions = {
            VK_KHR_SWAPCHAIN_EXTENSION_NAME
        };
    };
}