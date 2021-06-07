#pragma once

namespace Metamod::Engine 
{
    class IHudTextParams 
    {
        public:
            virtual ~IHudTextParams() = default;
            enum class Prop { fadeInTime = 0, fadeOutTime, holdTime, fxTime };

            virtual void setPosX(float x) = 0;
            virtual void setPosY(float y) = 0;
            virtual void setEffect(int effect) = 0;
            virtual void setColor(std::byte r, std::byte g, std::byte b, std::byte a) = 0;
            virtual void setColorEffect(std::byte r, std::byte g, std::byte b, std::byte a) = 0;
            virtual void setProp(Prop property, float value) = 0;
            virtual void setChannel(std::uint32_t channel) = 0;
    };
}