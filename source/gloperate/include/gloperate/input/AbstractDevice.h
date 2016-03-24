
#pragma once


#include <string>

#include <gloperate/gloperate_api.h>


namespace gloperate
{


class InputManager;


/**
*  @brief
*    An Interface for devices
*
*    This is a virtual interface for devices (to be held by an InputManager)
*/
class GLOPERATE_API AbstractDevice
{
public:
    /**
    *  @brief
    *    Constructor
    *
    *  @param[in] inputManager
    *    A pointer to the input manager (must NOT be null)
    *  @param[in] deviceDescriptor
    *    A String generated by the DevicePovider by which to identify the device
    */
    AbstractDevice(InputManager * inputManager, const std::string & deviceDescriptor);

    /**
    *  @brief
    *    Destructor
    */
    virtual ~AbstractDevice();

    /**
    *  @brief
    *    Poll the current state of the device and dispatche appropriate events
    */
    virtual void update() = 0;

    /**
    *  @brief
    *    Get device descriptor as a string
    *
    *  @return
    *    A String generated by the DevicePovider by which to identify the device
    */
    virtual const std::string & deviceDescriptor() const;


protected:
    InputManager * m_inputManager;
    std::string    m_deviceDescriptor;
};


} // namespace gloperate
