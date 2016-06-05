
#include <gloperate-qtquick/viewer/QmlScriptContext.h>

#include <QQmlContext>
#include <QQmlExpression>
#include <QString>
#include <QVariant>
#include <QJSValue>

#include <cppexpose/reflection/Object.h>

#include <gloperate-qtquick/viewer/QmlEngine.h>
#include <gloperate-qtquick/viewer/QmlObjectWrapper.h>


using namespace cppexpose;


namespace gloperate_qtquick
{


QmlScriptContext::QmlScriptContext(QmlEngine * engine)
: m_engine(engine)
{
}

QmlScriptContext::~QmlScriptContext()
{
    // Is already done by someone else
    // Hints about identity welcome!
    //for (auto * wrappedObject : m_wrappedObjects)
    //{
        //delete wrappedObject;
    //}
}

void QmlScriptContext::initialize(cppexpose::ScriptContext * scriptContext)
{
    m_scriptContext = scriptContext;
}

void QmlScriptContext::setGlobalNamespace(const std::string &)
{
    // This backend automatically uses the namespace 'gloperate'
    // and does not support any other global namespace.
}

void QmlScriptContext::registerObject(PropertyGroup * group)
{
    // Check arguments
    if (!group) return;

    // Create object wrapper
    QmlObjectWrapper * wrapper = new QmlObjectWrapper(m_engine, group);
    m_wrappedObjects.push_back(wrapper);

    // Add object to global object 'gloperate'
    QJSValue gloperate = m_engine->gloperate();
    gloperate.setProperty(group->name().c_str(), wrapper->wrapObject());
    m_engine->setGloperate(gloperate);
}

void QmlScriptContext::unregisterObject(PropertyGroup *)
{
    // [TODO]
}

Variant QmlScriptContext::evaluate(const std::string & code)
{
    QQmlExpression expr(m_engine->rootContext(), nullptr, QString::fromStdString(code));

    return m_engine->fromQVariant(
        expr.evaluate()
    );
}


} // namespace gloperate_qtquick
