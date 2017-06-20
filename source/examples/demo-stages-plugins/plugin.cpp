
#include <cppexpose/plugin/plugin_api.h>

#include "AntialiasableTriangleStage.h"
#include "AntialiasingRenderingPipeline.h"
#include "DOFCubeStage.h"
#include "DOFRenderingPipeline.h"
#include "DemoAntialiasingAggregationPipeline.h"
#include "DemoDOFAggregationPipeline.h"
#include "DemoSSAOAggregationPipeline.h"
#include "DemoTransparencyAggregationPipeline.h"
#include "ShapeDemo.h"
#include "SSAORenderingPipeline.h"
#include "SSAOApplicationStage.h"
#include "SSAOSceneRenderingStage.h"
#include "TransparencyRenderingPipeline.h"
#include "TransparentCirclesStage.h"
#include "DemoTextRenderingPipeline.h"
#include "GlyphSequenceDemoStage.h"
#include "LightTestPipeline.h"
#include "LightTestStage.h"
#include "ShaderDemoPipeline.h"
#include "AssimpMeshLoader.h"


CPPEXPOSE_PLUGIN_LIBRARY

    CPPEXPOSE_PLUGIN_COMPONENT(AntialiasableTriangleStage)
    CPPEXPOSE_PLUGIN_COMPONENT(AntialiasingRenderingPipeline)
    CPPEXPOSE_PLUGIN_COMPONENT(DOFCubeStage)
    CPPEXPOSE_PLUGIN_COMPONENT(DOFRenderingPipeline)
    CPPEXPOSE_PLUGIN_COMPONENT(DemoAntialiasingAggregationPipeline)
    CPPEXPOSE_PLUGIN_COMPONENT(DemoDOFAggregationPipeline)
    CPPEXPOSE_PLUGIN_COMPONENT(DemoSSAOAggregationPipeline)
    CPPEXPOSE_PLUGIN_COMPONENT(DemoTransparencyAggregationPipeline)
    CPPEXPOSE_PLUGIN_COMPONENT(ShapeDemo)
    CPPEXPOSE_PLUGIN_COMPONENT(SSAORenderingPipeline)
    CPPEXPOSE_PLUGIN_COMPONENT(SSAOApplicationStage)
    CPPEXPOSE_PLUGIN_COMPONENT(SSAOSceneRenderingStage)
    CPPEXPOSE_PLUGIN_COMPONENT(TransparencyRenderingPipeline)
    CPPEXPOSE_PLUGIN_COMPONENT(TransparentCirclesStage)
    CPPEXPOSE_PLUGIN_COMPONENT(DemoTextRenderingPipeline)
    CPPEXPOSE_PLUGIN_COMPONENT(GlyphSequenceDemoStage)
    CPPEXPOSE_PLUGIN_COMPONENT(LightTestPipeline)
    CPPEXPOSE_PLUGIN_COMPONENT(LightTestStage)
    CPPEXPOSE_PLUGIN_COMPONENT(ShaderDemoPipeline)
    CPPEXPOSE_PLUGIN_COMPONENT(AssimpMeshLoader)

CPPEXPOSE_PLUGIN_LIBRARY_END
