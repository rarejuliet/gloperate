
#pragma once


#include <vector>
#include <string>


#include <cppexpose/plugin/plugin_api.h>

#include <gloperate/gloperate-version.h>
#include <gloperate/base/Loader.h>


namespace globjects
{
    class Texture;
}


/**
*  @brief
*    .raw file loader
*
*  Supported options:
*    none
*/
class GlRawLoader : public gloperate::Loader<globjects::Texture>
{
public:
    CPPEXPOSE_DECLARE_COMPONENT(
        GlRawLoader, gloperate::AbstractLoader
      , "" // Tags
      , "" // Icon
      , "" // Annotations
      , "Load .raw files"
      , GLOPERATE_AUTHOR_ORGANIZATION
      , "v1.0.0"
    )


public:
    /**
    *  @brief
    *    Constructor
    *
    *  @param[in] environment
    *    Environment to which the loader belongs (must NOT be null!)
    */
    GlRawLoader(gloperate::Environment * environment);

    /**
    *  @brief
    *    Destructor
    */
    virtual ~GlRawLoader();

    // Virtual gloperate::AbstractLoader functions
    virtual bool canLoad(const std::string & ext) const override;
    virtual std::vector<std::string> loadingTypes() const override;
    virtual std::string allLoadingTypes() const override;

    // Virtual gloperate::Loader<globjects::Texture> functions
    virtual globjects::Texture * load(const std::string & filename, const cppexpose::Variant & options, std::function<void(int, int)> progress) const override;

protected:
    globjects::Texture * loadGLRawImage(const std::string & filename) const;
    globjects::Texture * loadRawImage(const std::string & filename) const;


protected:
    std::vector<std::string> m_extensions; /**< List of supported file extensions (e.g., ".bmp") */
    std::vector<std::string> m_types;      /**< List of supported file types (e.g., "bmp image (*.bmp)") */
};
