#pragma once


enum class InputType { Keyboard, Mouse, Gamepad };
enum class InputState { Pressed, Released, Held };

struct InputEvent
{
    InputType type;
    int code;
    InputState state;
};

using ActionCallback = std::function<void( const InputEvent& )>;

class InputManager
{
public:
    void Update();

    void MapAction( const std::string& actionName, InputType type, int code );
    void BindAction( const std::string& actionName, InputState state, ActionCallback callback );

    void ProcessInput( InputType type, int code, InputState state );

private:
    std::unordered_map<std::string, std::vector<InputEvent>> actionMap;
    std::unordered_map<std::string, std::unordered_map<InputState, std::vector<ActionCallback>>> actionCallbacks;
};