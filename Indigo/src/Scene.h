#pragma once

#include "Indigo.h"

class Scene {
public:
    void LoadFromFile( const std::string& filepath );
private:
    std::vector<Model> models;
};