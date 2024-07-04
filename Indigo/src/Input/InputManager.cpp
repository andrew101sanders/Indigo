#pragma once

#include "pch.h"
#include "Input/InputManager.h"

void InputManager::Update()
{
    // Process held keys, update time-based input, etc.
}

void InputManager::MapAction( const std::string& actionName, InputType type, int code )
{
    actionMap[actionName].push_back( { type, code, InputState::Pressed } );
}

void InputManager::BindAction( const std::string& actionName, InputState state, ActionCallback callback )
{
    actionCallbacks[actionName][state].push_back( callback );
}

void InputManager::ProcessInput( InputType type, int code, InputState state )
{
    for (const auto& [actionName, events] : actionMap)
    {
        for (const auto& event : events)
        {
            if (event.type == type && event.code == code)
            {
                for (const auto& callback : actionCallbacks[actionName][state])
                {
                    callback( { type, code, state } );
                }
            }
        }
    }
}