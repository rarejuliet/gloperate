
#include "SSAOSceneRenderingStage.h"

#include <tuple>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/random.hpp>

#include <glbinding/gl/gl.h>

#include <globjects/VertexArray.h>
#include <globjects/VertexAttributeBinding.h>
#include <globjects/Framebuffer.h>
#include <globjects/globjects.h>

#include <gloperate/gloperate.h>
#include <gloperate/base/Environment.h>


namespace
{


// Geometry, a triangle fan spanning 3 sides of a cube
// position, surface normal
static const std::array<std::array<glm::vec3, 2>, 8> s_vertices { {
    {{ glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 0.f, 1.f) }},
    {{ glm::vec3(1.f, 0.f, 0.f), glm::vec3(0.f, 0.f, 1.f) }},
    {{ glm::vec3(1.f, 1.f, 0.f), glm::vec3(0.f, 0.f, 1.f) }},
    {{ glm::vec3(0.f, 1.f, 0.f), glm::vec3(0.f, 0.f, 1.f) }},
    {{ glm::vec3(0.f, 1.f, 1.f), glm::vec3(1.f, 0.f, 0.f) }},
    {{ glm::vec3(0.f, 0.f, 1.f), glm::vec3(1.f, 0.f, 0.f) }},
    {{ glm::vec3(1.f, 0.f, 1.f), glm::vec3(0.f, 1.f, 0.f) }},
    {{ glm::vec3(1.f, 0.f, 0.f), glm::vec3(0.f, 1.f, 0.f) }}
} };


} // namespace


CPPEXPOSE_COMPONENT(SSAOSceneRenderingStage, gloperate::Stage)


SSAOSceneRenderingStage::SSAOSceneRenderingStage(gloperate::Environment * environment, const std::string & name)
: Stage(environment, name)
, canvasInterface(this)
, projectionMatrix("projectionMatrix", this)
, normalMatrix("normalMatrix", this)
{
}

SSAOSceneRenderingStage::~SSAOSceneRenderingStage()
{
}

void SSAOSceneRenderingStage::onContextInit(gloperate::AbstractGLContext *)
{
    canvasInterface.onContextInit();
    setupGeometry();
    setupProgram();
}

void SSAOSceneRenderingStage::onContextDeinit(gloperate::AbstractGLContext *)
{
    // deinitialize program
    m_program.reset();
    m_fragmentShader.reset();
    m_vertexShader.reset();

    // deinitialize geometry
    m_vertexBuffer.reset();
    m_vao.reset();

    canvasInterface.onContextDeinit();
}

void SSAOSceneRenderingStage::onProcess()
{
    // Get viewport
    const glm::vec4 & viewport = *canvasInterface.viewport;

    // Update viewport
    gl::glViewport(
        viewport.x,
        viewport.y,
        viewport.z,
        viewport.w
    );

    // Set uniforms
    auto viewMatrix = glm::lookAt(glm::vec3(2.0f, 1.0f, 1.0f), glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    auto projectionMatrix = glm::perspective(20.0f, viewport.z / viewport.w, 1.0f, 10.0f);
    auto viewProjectionMatrix = projectionMatrix * viewMatrix;

    this->projectionMatrix.setValue(projectionMatrix);
    this->normalMatrix.setValue(glm::inverseTranspose(glm::mat3(viewMatrix)));

    m_program->setUniform("viewProjectionMatrix", viewProjectionMatrix);

    // Bind color FBO
    globjects::Framebuffer * fbo = canvasInterface.obtainFBO();
    fbo->bind(gl::GL_FRAMEBUFFER);

    // Clear background
    const auto & color = *canvasInterface.backgroundColor;
    gl::glClearColor(color.redf(), color.greenf(), color.bluef(), 1.0f);
    gl::glScissor(viewport.x, viewport.y, viewport.z, viewport.w);
    gl::glEnable(gl::GL_SCISSOR_TEST);
    gl::glClear(gl::GL_COLOR_BUFFER_BIT | gl::GL_DEPTH_BUFFER_BIT);
    gl::glDisable(gl::GL_SCISSOR_TEST);
    gl::glDisable(gl::GL_BLEND); // ..., as it is still enabled from previous frame

    // Draw to color
    m_program->use();
    m_vao->drawArrays(gl::GL_TRIANGLE_FAN, 0, 8);
    m_program->release();

    // Unbind color FBO
    globjects::Framebuffer::unbind(gl::GL_FRAMEBUFFER);

    // Signal that output is valid
    canvasInterface.updateRenderTargetOutputs();
}

void SSAOSceneRenderingStage::setupGeometry()
{
    m_vao = cppassist::make_unique<globjects::VertexArray>();
    m_vertexBuffer = cppassist::make_unique<globjects::Buffer>();
    m_vertexBuffer->setData(s_vertices, gl::GL_STATIC_DRAW);

    auto positionBinding = m_vao->binding(0);
    positionBinding->setAttribute(0);
    positionBinding->setBuffer(m_vertexBuffer.get(), 0, sizeof(glm::vec3) * 2);
    positionBinding->setFormat(3, gl::GL_FLOAT, gl::GL_FALSE, 0);
    m_vao->enable(0);

    auto normalBinding = m_vao->binding(1);
    normalBinding->setAttribute(1);
    normalBinding->setBuffer(m_vertexBuffer.get(), 0, sizeof(glm::vec3) * 2);
    normalBinding->setFormat(3, gl::GL_FLOAT, gl::GL_FALSE, sizeof(glm::vec3));
    m_vao->enable(1);
}

void SSAOSceneRenderingStage::setupProgram()
{
    m_vertexShader   = std::unique_ptr<globjects::Shader>(m_environment->resourceManager()->load<globjects::Shader>(gloperate::dataPath() + "/gloperate/shaders/demos/DemoSSAORendering.vert"));
    m_fragmentShader = std::unique_ptr<globjects::Shader>(m_environment->resourceManager()->load<globjects::Shader>(gloperate::dataPath() + "/gloperate/shaders/demos/DemoSSAORendering.frag"));

    m_program = cppassist::make_unique<globjects::Program>();
    m_program->attach(m_vertexShader.get(), m_fragmentShader.get());
}
