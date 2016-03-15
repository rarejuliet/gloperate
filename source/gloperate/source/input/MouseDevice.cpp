
#include <gloperate/input/MouseDevice.h>

#include <gloperate/input/MouseEvent.h>
#include <gloperate/input/InputManager.h>


namespace gloperate
{


MouseDevice::MouseDevice(InputManager * inputManager, const std::string & deviceDescriptor)
: AbstractDevice(inputManager, deviceDescriptor)
{
}

MouseDevice::~MouseDevice()
{
}

void MouseDevice::update()
{
}

void MouseDevice::move(const glm::ivec2 & pos)
{
    auto inputEvent = new MouseEvent{
        InputEvent::Type::MouseButtonPress,
        this,
        pos
    };

    m_inputManager->onEvent(inputEvent);
}

void MouseDevice::buttonPress(int button, const glm::ivec2 & pos)
{
    auto inputEvent = new MouseEvent{
        InputEvent::Type::MouseButtonPress,
        this,
        pos,
        button
    };

    m_inputManager->onEvent(inputEvent);
}

void MouseDevice::buttonRelease(int button, const glm::ivec2 & pos)
{
    auto inputEvent = new MouseEvent{
        InputEvent::Type::MouseButtonRelease,
        this,
        pos,
        button
    };

    m_inputManager->onEvent(inputEvent);
}

void MouseDevice::wheelScroll(const glm::vec2 & delta, const glm::ivec2 & pos)
{
    auto inputEvent = new MouseEvent{
        InputEvent::Type::MouseWheelScroll,
        this,
        pos,
        delta
    };

    m_inputManager->onEvent(inputEvent);
}


} // namespace gloperate
