
#pragma once


#include <cppexpose/plugin/plugin_api.h>

#include <gloperate/gloperate-version.h>
#include <gloperate/pipeline/Pipeline.h>
#include <gloperate/pipeline/Input.h>
#include <gloperate/stages/interfaces/CanvasInterface.h>


namespace gloperate_glkernel {

class MultiFrameAggregationPipeline;

}


class SSAORenderingPipeline;


/**
*  @brief
*    Demo pipeline showing multiframe aggregation
*/
class DemoSSAOAggregationPipeline : public gloperate::Pipeline
{
public:
    CPPEXPOSE_DECLARE_COMPONENT(
        DemoSSAOAggregationPipeline, gloperate::Stage
      , "RenderStage Demos" // Tags
      , ""                  // Icon
      , ""                  // Annotations
      , "Demo pipeline showing multiframe aggregation"
      , GLOPERATE_AUTHOR_ORGANIZATION
      , "v0.1.0"
    )


public:
    // Interfaces
    gloperate::CanvasInterface canvasInterface; ///< Interface for rendering into a viewer

    // Inputs
    Input<int>                 multiFrameCount; ///< Number of frames to aggregate


public:
    /**
    *  @brief
    *    Constructor
    *
    *  @param[in] environment
    *    Environment to which the pipeline belongs (must NOT be null!)
    *  @param[in] name
    *    Pipeline name
    */
    DemoSSAOAggregationPipeline(gloperate::Environment * environment, const std::string & name = "DemoSSAOAggregationPipeline");

    /**
    *  @brief
    *    Destructor
    */
    virtual ~DemoSSAOAggregationPipeline();


protected:
    // Stages
    std::unique_ptr<gloperate_glkernel::MultiFrameAggregationPipeline> m_multiFramePipeline; ///< Aggregation Pipeline
    std::unique_ptr<SSAORenderingPipeline>                             m_ssaoPipeline;       ///< Pipeline generating frames to aggregate
};
