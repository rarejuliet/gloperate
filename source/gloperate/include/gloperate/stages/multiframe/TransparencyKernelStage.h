#pragma once

#include <cppexpose/plugin/plugin_api.h>

#include <gloperate/gloperate-version.h>
#include <gloperate/pipeline/Stage.h>
#include <gloperate/pipeline/Output.h>
#include <gloperate/pipeline/Input.h>


namespace globjects
{

class Texture;

}


namespace gloperate
{


class GLOPERATE_API TransparencyKernelStage : public gloperate::Stage
{
public:
    CPPEXPOSE_DECLARE_COMPONENT(
        TransparencyKernelStage, gloperate::Stage
      , ""
      , ""
      , ""
      , "Stage that provides noise for random discard based transparency"
      , GLOPERATE_AUTHOR_ORGANIZATION
      , "v1.0.0"
    )

public:
    TransparencyKernelStage(gloperate::Environment * environment, const std::string & name = "TransparencyKernelStage");

public:
    Input<bool> reprocess;

    Output<globjects::Texture *> transparencyMaskTexture;

protected:
    virtual void onProcess(gloperate::AbstractGLContext * context) override;
    virtual void onContextInit(gloperate::AbstractGLContext * context) override;

};


} // namespace gloperate
