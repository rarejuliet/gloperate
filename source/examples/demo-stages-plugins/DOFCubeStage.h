
#pragma once


#include <cppexpose/plugin/plugin_api.h>

#include <globjects/VertexArray.h>
#include <globjects/Buffer.h>
#include <globjects/Program.h>
#include <globjects/Shader.h>
#include <globjects/Texture.h>
#include <globjects/base/StringTemplate.h>

#include <gloperate/gloperate-version.h>
#include <gloperate/pipeline/Stage.h>
#include <gloperate/stages/interfaces/CanvasInterface.h>
#include <gloperate/rendering/Camera.h>


/**
*  @brief
*    Demo stage that renders a static cube onto the screen
*/
class DOFCubeStage : public gloperate::Stage
{
public:
    CPPEXPOSE_DECLARE_COMPONENT(
        DOFCubeStage, gloperate::Stage
      , "" // Tags
      , "" // Icon
      , "" // Annotations
      , "Demo stage that renders a static cube onto the screen"
      , GLOPERATE_AUTHOR_ORGANIZATION
      , "v1.0.0"
    )


public:
    // Interfaces
    gloperate::CanvasInterface canvasInterface; ///< Interface for rendering into a viewer

    // Inputs
    Input<std::vector<glm::vec2> *> dofShifts;  ///< DOF shift for multiframe rendering


public:
    /**
    *  @brief
    *    Constructor
    *
    *  @param[in] environment
    *    Environment to which the stage belongs (must NOT be null!)
    *  @param[in] name
    *    Stage name
    */
    DOFCubeStage(gloperate::Environment * environment, const std::string & name = "DOFCubeStage");

    /**
    *  @brief
    *    Destructor
    */
    virtual ~DOFCubeStage();


protected:
    // Virtual Stage functions
    virtual void onContextInit(gloperate::AbstractGLContext * context) override;
    virtual void onContextDeinit(gloperate::AbstractGLContext * context) override;
    virtual void onProcess() override;

    // Helper functions
    void setupGeometry();
    void setupProgram();


protected:
    // Rendering objects
    std::unique_ptr<globjects::VertexArray> m_vao;
    std::unique_ptr<globjects::Buffer>      m_vertexBuffer;
    std::unique_ptr<globjects::Shader>      m_vertexShader;
    std::unique_ptr<globjects::Shader>      m_fragmentShader;
    std::unique_ptr<globjects::Program>     m_program;
};
