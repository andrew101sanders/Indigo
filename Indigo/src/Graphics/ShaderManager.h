#pragma once

class GraphicsDevice;
class ShaderProgram;

class ShaderManager
{
public:
    explicit ShaderManager( GraphicsDevice* device, const std::string& directory );
    ~ShaderManager();

    // Load a shader. If already loaded, returns the existing one.
    std::shared_ptr<ShaderProgram> LoadShader( const std::string& vsName, const std::string& psName );

    // Reload a specific shader. Can be used for hot-reloading.
    void ReloadShader( const std::string& vsName, const std::string& psName );
    void CheckForShaderUpdates();
    // Optional: Implement these to integrate with a file watcher for hot-reloading.
    // void StartWatching();
    // void StopWatching();

private:
    struct ShaderInfo {
        std::shared_ptr<ShaderProgram> shaderProgram;
        std::filesystem::file_time_type lastWriteTime;
    };
    GraphicsDevice* device;
    std::string shaderDirectory;
    std::unordered_map<std::string, ShaderInfo> shaders;

    // Generate a unique key for the shader combination for internal use.
    std::string GenerateShaderKey( const std::string& vsName, const std::string& psName );

    // Optional: Callback when a file changes. Used for hot-reloading.
    // void OnShaderFileChanged(const std::string& filename);
};

