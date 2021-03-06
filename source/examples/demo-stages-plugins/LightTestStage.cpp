
#include "LightTestStage.h"

#include <glm/gtx/transform.hpp>

#include <glbinding/gl/enum.h>
#include <glbinding/gl/bitfield.h>

#include <globjects/base/File.h>
#include <globjects/VertexArray.h>
#include <globjects/VertexAttributeBinding.h>
#include <globjects/Framebuffer.h>
#include <globjects/NamedString.h>
#include <globjects/globjects.h>

#include <gloperate/gloperate.h>
#include <gloperate/base/Environment.h>


namespace
{


// Geometry describing the cube
// position, surface normal
static const std::array<std::array<glm::vec3, 2>, 14> s_cube { {
    {{ glm::vec3(-1.f, -1.f, -1.f), glm::vec3( 0.0f, -1.0f,  0.0) }},
    {{ glm::vec3(-1.f, -1.f, +1.f), glm::vec3( 0.0f, -1.0f,  0.0) }},
    {{ glm::vec3(+1.f, -1.f, -1.f), glm::vec3( 0.0f, -1.0f,  0.0) }},
    {{ glm::vec3(+1.f, -1.f, +1.f), glm::vec3( 0.0f, -1.0f,  0.0) }},
    {{ glm::vec3(+1.f, +1.f, +1.f), glm::vec3( 1.0f,  0.0f,  0.0) }},
    {{ glm::vec3(-1.f, -1.f, +1.f), glm::vec3( 0.0f,  0.0f,  1.0) }},
    {{ glm::vec3(-1.f, +1.f, +1.f), glm::vec3( 0.0f,  0.0f,  1.0) }},
    {{ glm::vec3(-1.f, -1.f, -1.f), glm::vec3(-1.0f,  0.0f,  0.0) }},
    {{ glm::vec3(-1.f, +1.f, -1.f), glm::vec3(-1.0f,  0.0f,  0.0) }},
    {{ glm::vec3(+1.f, -1.f, -1.f), glm::vec3( 0.0f,  0.0f, -1.0) }},
    {{ glm::vec3(+1.f, +1.f, -1.f), glm::vec3( 0.0f,  0.0f, -1.0) }},
    {{ glm::vec3(+1.f, +1.f, +1.f), glm::vec3( 1.0f,  0.0f,  0.0) }},
    {{ glm::vec3(-1.f, +1.f, -1.f), glm::vec3( 0.0f,  1.0f,  0.0) }},
    {{ glm::vec3(-1.f, +1.f, +1.f), glm::vec3( 0.0f,  1.0f,  0.0) }}
} };

static const glm::vec3 s_cameraEye(0.0f, -1.5f, -3.0f);


} // namespace


CPPEXPOSE_COMPONENT(LightTestStage, gloperate::Stage)


LightTestStage::LightTestStage(gloperate::Environment * environment, const std::string & name)
: Stage(environment, "LightTestStage", name)
, canvasInterface(this)
, glossiness("glossiness", this, 0.0f)
, totalTime("totalTime", this, 0.0f)
, lightColorTypeData("lightColorTypeData", this, nullptr)
, lightPositionData("lightPositionData", this, nullptr)
, lightAttenuationData("lightAttenuationData", this, nullptr)
{
}

LightTestStage::~LightTestStage()
{
}

void LightTestStage::onContextInit(gloperate::AbstractGLContext *)
{
    canvasInterface.onContextInit();

    // Setup Geometry
    m_vao = cppassist::make_unique<globjects::VertexArray>();
    m_vertexBuffer = cppassist::make_unique<globjects::Buffer>();
    m_vertexBuffer->setData(s_cube, gl::GL_STATIC_DRAW);

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

    // setup Program
    m_vertexShader   = std::unique_ptr<globjects::Shader>(m_environment->resourceManager()->load<globjects::Shader>(gloperate::dataPath() + "/gloperate/shaders/demos/DemoLights.vert"));
    m_fragmentShader = std::unique_ptr<globjects::Shader>(m_environment->resourceManager()->load<globjects::Shader>(gloperate::dataPath() + "/gloperate/shaders/demos/DemoLights.frag"));

    m_program = cppassist::make_unique<globjects::Program>();
    m_program->attach(m_vertexShader.get(), m_fragmentShader.get());

    m_program->setUniform("colorTypeData", 0);
    m_program->setUniform("positionData", 1);
    m_program->setUniform("attenuationData", 2);

    m_program->setUniform("eye", s_cameraEye);

    // register NamedStrings for shader includes
    auto dataFolderPath = gloperate::dataPath();
    m_lightProcessingString        = globjects::NamedString::create("/gloperate/shaders/lighting/lightprocessing.glsl", new globjects::File(dataFolderPath + "/gloperate/shaders/lighting/lightprocessing.glsl"));
    m_lightProcessingDiffuseString = globjects::NamedString::create("/gloperate/shaders/lighting/lightprocessing_diffuse.glsl", new globjects::File(dataFolderPath + "/gloperate/shaders/lighting/lightprocessing_diffuse.glsl"));
    m_lightProcessingPhongString   = globjects::NamedString::create("/gloperate/shaders/lighting/lightprocessing_phong.glsl", new globjects::File(dataFolderPath + "/gloperate/shaders/lighting/lightprocessing_phong.glsl"));
}

void LightTestStage::onContextDeinit(gloperate::AbstractGLContext *)
{
    // deregister NamedStrings
    m_lightProcessingDiffuseString.reset();
    m_lightProcessingPhongString.reset();
    m_lightProcessingString.reset();

    // deinitialize program
    m_program.reset();
    m_fragmentShader.reset();
    m_vertexShader.reset();

    // deinitialize geometry
    m_vertexBuffer.reset();
    m_vao.reset();

    canvasInterface.onContextDeinit();
}

void LightTestStage::onProcess()
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

    // Bind FBO
    globjects::Framebuffer * fbo = canvasInterface.obtainFBO();
    fbo->bind(gl::GL_FRAMEBUFFER);

    // Clear background
    auto & color = *canvasInterface.backgroundColor;
    gl::glClearColor(color.redf(), color.greenf(), color.bluef(), 1.0f);
    gl::glScissor(viewport.x, viewport.y, viewport.z, viewport.w);
    gl::glEnable(gl::GL_SCISSOR_TEST);
    gl::glClear(gl::GL_COLOR_BUFFER_BIT | gl::GL_DEPTH_BUFFER_BIT);
    gl::glDisable(gl::GL_SCISSOR_TEST);

    // Update transformation
    auto model = glm::rotate((*totalTime) / 3.0f, glm::vec3(0, 1, 0));
    auto view = glm::lookAt(s_cameraEye, glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    auto projection = glm::perspective(30.0f, viewport.z / viewport.w, 0.1f, 10.0f);
    m_program->setUniform("modelMatrix", model);
    m_program->setUniform("modelViewProjection", projection * view * model);

    // Update other uniforms
    m_program->setUniform("glossiness", (*glossiness));

    // Bind textures
    if (*lightColorTypeData)
        (*lightColorTypeData)->bindActive(0);
    if (*lightPositionData)
        (*lightPositionData)->bindActive(1);
    if (*lightAttenuationData)
        (*lightAttenuationData)->bindActive(2);

    // Draw geometry
    m_program->use();
    m_vao->drawArrays(gl::GL_TRIANGLE_STRIP, 0, 14);
    m_program->release();

    // Unbind textures
    if (*lightColorTypeData)
        (*lightColorTypeData)->unbindActive(0);
    if (*lightPositionData)
        (*lightPositionData)->unbindActive(1);
    if (*lightAttenuationData)
        (*lightAttenuationData)->unbindActive(2);

    // Unbind FBO
    globjects::Framebuffer::unbind(gl::GL_FRAMEBUFFER);

    // Signal that output is valid
    canvasInterface.updateRenderTargetOutputs();
}
