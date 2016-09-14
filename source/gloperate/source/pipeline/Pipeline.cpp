
#include <gloperate/pipeline/Pipeline.h>

#include <vector>
#include <set>

#include <cppexpose/variant/Variant.h>

#include <cppassist/logging/logging.h>

#include <gloperate/base/Environment.h>
#include <gloperate/base/ComponentManager.h>


using namespace cppassist;
using namespace cppexpose;


namespace gloperate
{


// [TODO] invalidate sorting when stages or connections change


Pipeline::Pipeline(Environment * environment, const std::string & name)
: Stage(environment, name)
, m_sorted(false)
{
    // Register functions
    addFunction("createStage",      this, &Pipeline::scr_createStage);
    addFunction("removeStage",      this, &Pipeline::scr_removeStage);
    addFunction("addConnection",    this, &Pipeline::scr_addConnection);
    addFunction("removeConnection", this, &Pipeline::scr_removeConnection);
}

Pipeline::~Pipeline()
{
}

const std::vector<Stage *> Pipeline::stages() const
{
    return m_stages;
}

Stage * Pipeline::stage(const std::string & name) const
{
    return m_stagesMap.at(name);
}

void Pipeline::addStage(Stage * stage, cppexpose::PropertyOwnership ownership)
{
    // Check parameters
    if (!stage)
    {
        return;
    }

    // Find free name
    std::string name = getFreeName(stage->name());
    stage->setName(name);

    // Add stage as property
    addProperty(stage, ownership);

    // Add stage
    m_stages.push_back(stage);
    if (stage->name() != "") {
        m_stagesMap.insert(std::make_pair(stage->name(), stage));        
    }

    // Emit signal
    stageAdded(stage);
}

bool Pipeline::removeStage(Stage * stage)
{
    if (!stage)
    {
        return false;
    }

    auto it = std::find(m_stages.begin(), m_stages.end(), stage);
    if (it == m_stages.end())
    {
        return false;
    }

    m_stages.erase(it);
    m_stagesMap.erase(stage->name());

    stageRemoved(stage);

    removeProperty(stage);

    return true;
}

bool Pipeline::destroyStage(Stage * stage)
{
    if (!removeStage(stage))
    {
        return false;
    }

    delete stage;

    return true;
}

bool Pipeline::isPipeline() const
{
    return true;
}

void Pipeline::sortStages()
{
    auto couldBeSorted = true;
    std::vector<Stage *> sorted;
    std::set<Stage *> touched;

    std::function<void(Stage *)> visit = [&] (Stage * stage)
    {
        if (!couldBeSorted)
        {
            sorted.push_back(stage);
            return;
        }

        if (touched.count(stage) > 0)
        {
            critical() << "Pipeline is not a directed acyclic graph" << std::endl;
            couldBeSorted = false;
            sorted.push_back(stage);
            return;
        }

        touched.insert(stage);

        for (auto stageIt = m_stages.begin(); stageIt != m_stages.end(); /* nop */)
        {
            if (!stage->requires(*stageIt, false))
            {
                ++stageIt;
                continue;
            }

            auto nextStage = *stageIt;
            m_stages.erase(stageIt);
            visit(nextStage);

            stageIt = m_stages.begin();
        }

        sorted.push_back(stage);
    };

    while (!m_stages.empty())
    {
        auto stageIt = m_stages.begin();
        auto stage = *stageIt;
        m_stages.erase(stageIt);
        visit(stage);
    }

    m_stages = sorted;
    m_sorted = couldBeSorted;
}

void Pipeline::onContextInit(AbstractGLContext * context)
{
    for (auto stage : m_stages)
    {
        stage->initContext(context);
    }
}

void Pipeline::onContextDeinit(AbstractGLContext * context)
{
    for (auto stage : m_stages)
    {
        stage->deinitContext(context);
    }
}

void Pipeline::onProcess(AbstractGLContext * context)
{
    if (!m_sorted) {
        sortStages();
    }

    for (auto stage : m_stages)
    {
        if (stage->needsProcessing()) {
            stage->process(context);
        }
    }
}

void Pipeline::onInputValueChanged(AbstractSlot *)
{
    // Not necessary for pipelines (handled by inner connections)
}

void Pipeline::onOutputRequiredChanged(AbstractSlot *)
{
    // Not necessary for pipelines (handled by inner connections)
}

cppexpose::Variant Pipeline::scr_getDescription()
{
    // Get stage description
    cppexpose::Variant obj = Stage::scr_getDescription();

    // List stages
    Variant stages = Variant::array();

    for (auto stage : m_stages)
    {
        stages.asArray()->push_back(stage->name());
    }

    (*obj.asMap())["stages"] = stages;

    // Return pipeline description
    return obj;
}

std::string Pipeline::scr_createStage(const std::string & className, const std::string & name)
{
    // Get component manager
    auto componentManager = m_environment->componentManager();

    // List available stages
    /*
    auto stageComponents = componentManager->components<gloperate::Stage>();

    for (auto component : stageComponents)
    {
        info() << "- " << component->name();
    }
    */

    // Get component for the requested stage
    auto component = componentManager->component<gloperate::Stage>(className);

    if (component)
    {
        // Create stage
        Stage * stage = component->createInstance(m_environment, name);
        addStage(stage);

        return stage->name();
    }

    return "";
}

void Pipeline::scr_removeStage(const std::string & name)
{
    Stage * stage = this->stage(name);

    removeStage(stage);
}

void Pipeline::scr_addConnection(const std::string & from, const std::string & to)
{
}

void Pipeline::scr_removeConnection(const std::string & to)
{
}


} // namespace gloperate
