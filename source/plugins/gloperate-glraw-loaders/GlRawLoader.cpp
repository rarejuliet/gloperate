
#include "GlRawLoader.h"

#include <QString>
#include <QFileInfo>

#include <RawFile.h>
#include <FileNameSuffix.h>

#include <cppexpose/variant/Variant.h>

#include <glbinding/gl/gl.h>

#include <globjects/Texture.h>


CPPEXPOSE_COMPONENT(GlRawLoader, gloperate::AbstractLoader)


GlRawLoader::GlRawLoader(gloperate::Environment * environment)
    : gloperate::Loader<globjects::Texture>(environment)
{
    m_extensions.push_back(".raw");
    m_extensions.push_back(".glraw");

    m_types.push_back("raw images (*.raw)");
    m_types.push_back("glraw images (*.glraw)");
}

GlRawLoader::~GlRawLoader()
{
}

bool GlRawLoader::canLoad(const std::string & ext) const
{
    // Check if file type is supported
    return (std::count(m_extensions.begin(), m_extensions.end(), "." + ext) > 0);
}

std::vector<std::string> GlRawLoader::loadingTypes() const
{
    // Return list of supported file types
    return m_types;
}

std::string GlRawLoader::allLoadingTypes() const
{
    // Compose list of all supported file extensions
    std::string allTypes;
    for (unsigned int i = 0; i < m_extensions.size(); ++i) {
        if (i > 0) allTypes += " ";
        allTypes += "*." + m_extensions[i].substr(1);
    }

    // Return supported types
    return allTypes;
}

globjects::Texture * GlRawLoader::load(const std::string & filename, const cppexpose::Variant & /*options*/, std::function<void(int, int)> /*progress*/) const
{

    globjects::Texture * texture = nullptr;

    QFileInfo fi(QString::fromStdString(filename));
    if (fi.suffix() == "glraw")
        texture = loadGLRawImage(filename);
    else if (fi.suffix() == "raw")
        texture = loadRawImage(filename);

    return texture;
}

globjects::Texture * GlRawLoader::loadGLRawImage(const std::string & filename) const
{
    glraw::RawFile rawFile(filename);

    if (!rawFile.isValid())
        return nullptr;

    const int w = rawFile.intProperty("width");
    const int h = rawFile.intProperty("height");

    globjects::Texture * texture = globjects::Texture::createDefault(gl::GL_TEXTURE_2D);

    if (rawFile.hasIntProperty("format"))
    {
        auto format = static_cast<gl::GLenum>(rawFile.intProperty("format"));
        auto type = static_cast<gl::GLenum>(rawFile.intProperty("type"));

        texture->image2D(
            0,
            gl::GL_RGBA8,
            w,
            h,
            0,
            format,
            type,
            rawFile.data()
        );
    }
    else
    {
        auto compressedFormat = static_cast<gl::GLenum>(rawFile.intProperty("compressedFormat"));
        auto size = rawFile.intProperty("size");

        texture->compressedImage2D(
            0,
            compressedFormat,
            w,
            h,
            0,
            size,
            rawFile.data()
        );
    }

    return texture;
}

globjects::Texture * GlRawLoader::loadRawImage(const std::string & filename) const
{
    glraw::FileNameSuffix suffix(QString::fromStdString(filename));

    if (!suffix.isValid())
        return nullptr;

    QFile file(QString::fromStdString(filename));
    if (!file.open(QIODevice::ReadOnly))
        return nullptr;
    QByteArray data = file.readAll();


    globjects::Texture * texture = globjects::Texture::createDefault(gl::GL_TEXTURE_2D);

    if (!suffix.compressed())
    {
        texture->image2D(
            0,
            gl::GL_RGBA8,
            suffix.width(),
            suffix.height(),
            0,
            suffix.format(),
            suffix.type(),
            data.data()
        );
    }
    else
    {
        texture->compressedImage2D(
            0,
            gl::GL_RGBA8,
            suffix.width(),
            suffix.height(),
            0,
            data.size(),
            data.data()
        );
    }

    return texture;
}
