#pragma once

#include <vector>
#include <string>

#include <gloperate/input/AbstractDevice.h>

#include <gloperate-hidapi/gloperate-hidapi_api.h>

#include <hidapi/hidapi.h>


namespace gloperate_hidapi
{

class GLOPERATE_HIDAPI_API SpaceNavigator : public gloperate::AbstractDevice
{
public:
    /**
     * @brief
     *   Constructor
     */
    SpaceNavigator(gloperate::InputManager * inputManager, const std::string & deviceDescriptor, const std::string& path);

    /**
     * @brief
     *   Destructor
     */
    virtual ~SpaceNavigator();

    /**
     * @brief
     *   Updates the state of the device and the corresponding controls.
     *   This will fire signals from the controls.
     */
    void virtual update();


private:
    hid_device * m_pHandle;
    static const int m_timeout = 10;
};

}
