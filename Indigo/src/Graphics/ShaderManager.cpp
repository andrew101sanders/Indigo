#include "pch.h"

#include "Graphics/ShaderManager.h"
#include "Graphics/ShaderProgram.h"
#include "Graphics/GraphicsDevice.h"


namespace fs = std::filesystem;

#ifdef _DEBUG
const std::string pathPrefix = "../";
#else
const std::string pathPrefix = "";
#endif

ShaderManager::ShaderManager( GraphicsDevice* device, const std::string& directory )
    : device( device ), shaderDirectory( directory )
{
}

ShaderManager::~ShaderManager()
{
    // Cleanup if necessary
    // If using smart pointers like shared_ptr, explicit cleanup might not be necessary here
    shaders.clear();
}

std::shared_ptr<ShaderProgram> ShaderManager::LoadShader( const std::string& vsName, const std::string& psName )
{
    std::string key = GenerateShaderKey( vsName, psName );

    // Check if the shader is already loaded
    auto it = shaders.find( key );
    if (it != shaders.end())
    {
        return it->second.shaderProgram;
    }

    // Load and compile the shader
    auto shader = std::make_shared<ShaderProgram>( device );
    shader->LoadShader( vsName.c_str(), psName.c_str() );

    // Get the last write time for the shaders
    std::filesystem::path vsPath = pathPrefix + shaderDirectory + "/" + vsName + ".hlsl";
    std::filesystem::path psPath = pathPrefix + shaderDirectory + "/" + psName + ".hlsl";
    auto lastWriteTime = max( std::filesystem::last_write_time( vsPath ), std::filesystem::last_write_time( psPath ) );

    // Store the shader and its last write time in the map
    ShaderInfo info;
    info.shaderProgram = shader;
    info.lastWriteTime = lastWriteTime;
    shaders[key] = info;

    return shader;
}


void ShaderManager::ReloadShader( const std::string& vsName, const std::string& psName )
{
    std::string key = GenerateShaderKey( vsName, psName );

    // Check if the shader exists in the map
    auto it = shaders.find( key );
    if (it != shaders.end())
    {
        // Clear the resources of the old shader
        it->second.shaderProgram->ClearResources();

        // Load and recompile the shader
        it->second.shaderProgram->LoadShader( vsName.c_str(), psName.c_str() );
    }
    else
    {
        // If not found, simply load the new shader
        LoadShader( vsName, psName );
    }
}


void ShaderManager::CheckForShaderUpdates()
{
    for (auto& [key, shaderInfo] : shaders)
    {
        // Derive filenames from the key
        size_t separatorPos = key.find( '_' );
        if (separatorPos == std::string::npos)
        {
            // This is unexpected and probably an error in key formation.
            continue;
        }

        std::string vsName = key.substr( 0, separatorPos ) + ".hlsl";
        std::string psName = key.substr( separatorPos + 1 ) + ".hlsl";

        // Construct the full paths
        std::filesystem::path vsPath = pathPrefix + shaderDirectory + "/" + vsName;
        std::filesystem::path psPath = pathPrefix + shaderDirectory + "/" + psName;

        // Get the current write times for the shaders
        auto currentVsWriteTime = std::filesystem::last_write_time( vsPath );
        auto currentPsWriteTime = std::filesystem::last_write_time( psPath );

        // If either of the shaders has been modified since it was last loaded, reload it
        if (currentVsWriteTime > shaderInfo.lastWriteTime || currentPsWriteTime > shaderInfo.lastWriteTime)
        {
            std::string vsNameWithoutExtension = key.substr( 0, separatorPos );
            std::string psNameWithoutExtension = key.substr( separatorPos + 1 );

            // 1. Delete the .cso file from shadercache
            std::filesystem::path vsCsoPath = "assets/shaders/shadercache/" + vsNameWithoutExtension + ".cso";
            std::filesystem::path psCsoPath = "assets/shaders/shadercache/" + psNameWithoutExtension + ".cso";
            std::filesystem::remove( vsCsoPath );
            std::filesystem::remove( psCsoPath );

            // 2. Delete the .hlsl file from /assets/shaders/
            std::filesystem::path binVsPath = "assets/shaders/" + vsNameWithoutExtension + ".hlsl";
            std::filesystem::path binPsPath = "assets/shaders/" + psNameWithoutExtension + ".hlsl";
            std::filesystem::remove( binVsPath );
            std::filesystem::remove( binPsPath );

            // 3. Copy the .hlsl file from /assets/shaders/ to /assets/shaders/
            std::filesystem::path srcVsPath = "../assets/shaders/" + vsNameWithoutExtension + ".hlsl";
            std::filesystem::path srcPsPath = "../assets/shaders/" + psNameWithoutExtension + ".hlsl";
            std::filesystem::copy( srcVsPath, binVsPath );
            std::filesystem::copy( srcPsPath, binPsPath );

            // Reload the shader
            ReloadShader( vsNameWithoutExtension, psNameWithoutExtension );

            // Update the last write time
            shaderInfo.lastWriteTime = max( currentVsWriteTime, currentPsWriteTime );
        }
    }
}


std::string ShaderManager::GenerateShaderKey( const std::string& vsName, const std::string& psName )
{
    return vsName + "_" + psName;
}