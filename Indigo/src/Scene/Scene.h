#pragma once

class Model;

class Scene {
public:
    void LoadFromFile( const std::string& filepath );
private:
    std::vector<Model> models;
};