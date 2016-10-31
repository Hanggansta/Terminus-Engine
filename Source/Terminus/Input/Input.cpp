#include "Input.h"
#include "../Core/Event/Event.h"
#include "../Core/Event/EventHandler.h"
#include "PlayerContext.h"
#include <vector>
#include <GLFW/glfw3.h>

#define MAX_PLAYERS 8

namespace Input
{
    int m_PlayerCount;
    std::vector<PlayerContexts> m_PlayerContextList;
    KeyboardDevice m_keyboard;
    MouseDevice    m_mouse;

    void FindPlayerDevices()
    {
         for (int i = m_PlayerContextList.size(); i < m_PlayerCount; i++)
        {
            bool present = (bool)glfwJoystickPresent(GLFW_JOYSTICK_1 + i);

            if(present)
            {
                if(i > m_PlayerContextList.size() - 1)
                {
                    PlayerContexts newPlayer = PlayerContexts();
                    m_PlayerContextList.push_back(newPlayer);
                }

                m_PlayerContextList[i].m_GamepadIndex = GLFW_JOYSTICK_1 + i;
            }
        }
    }

    void Initialize()
    {
        PlayerContexts initialPlayer = PlayerContexts();
        m_PlayerContextList.push_back(initialPlayer);
        m_PlayerCount = 1;

        for (int i = 0; i < MAX_MOUSE_BUTTONS; i++)
        {
            m_mouse.button_states[i] = false;
        }
        
        for (int i = 0; i < MAX_KEYBOARD_BUTTONS; i++)
        {
            m_keyboard.button_states[i] = false;
            m_keyboard.button_axis_states[i] = 0.0;
        }
        
        FindPlayerDevices();
    }
    
    MouseDevice* GetMouseDevice()
    {
        return &m_mouse;
    }
    
    KeyboardDevice* GetKeyboardDevice()
    {
        return &m_keyboard;
    }

    void SetPlayerCount(int _Count)
    {
        m_PlayerCount = _Count;
        FindPlayerDevices();
    }

    void LoadContext(std::string _Name, int _PlayerIndex)
    {
        if(_PlayerIndex > m_PlayerContextList.size() - 1)
            return;
        else
        {
            PlayerContexts* cxt = &m_PlayerContextList[_PlayerIndex];
            // Load context
            
            // Add to Player Context
        }
    }
    
    InputContext* CreateContext(int _PlayerIndex)
    {
        PlayerContexts* player_context = &m_PlayerContextList[_PlayerIndex];
        
        InputContext input_context;
        int size = player_context->m_Contexts.size();
        player_context->m_Contexts.push_back(input_context);
        
        return &player_context->m_Contexts[size];
    }

    void SetActiveContext(std::string _Name, int _PlayerIndex)
    {
        if(_PlayerIndex > m_PlayerContextList.size() - 1)
            return;
        else
        {
            PlayerContexts* cxt = &m_PlayerContextList[_PlayerIndex];

            for (int i = 0; i < cxt->m_Contexts.size(); i++)
            {
                if(cxt->m_Contexts[i].m_ContextName == _Name)
                {
                    cxt->m_ActiveContext = &cxt->m_Contexts[i];
                    break;
                }
            }
        }
    }

    void ProcessKeyboardInput(int _Key, int _Action)
    {
        if(m_PlayerContextList.size() != 0)
        {
            // For each Player
            InputContext* context = m_PlayerContextList[0].m_ActiveContext;
            
            if(!context)
                return;
            
            // Check if State
            if(context->m_KeyboardStateMap.find(_Key) != context->m_KeyboardStateMap.end() && _Action == GLFW_PRESS)
            {
                std::string state = context->m_KeyboardStateMap[_Key];
                
                // Fire Event
                InputStateEvent* event = new InputStateEvent(state);
                Terminus::EventHandler::QueueEvent(event);
                
                m_keyboard.button_states[_Key] = _Action;
                
                return;
            }
            
            // Check if Action
            if(context->m_KeyboardActionMap.find(_Key) != context->m_KeyboardActionMap.end())
            {
                std::string action = context->m_KeyboardActionMap[_Key];
                
                if(_Action == GLFW_PRESS)
                {
                    // Fire Pressed Event
                    InputActionEvent* event = new InputActionEvent(action, 1);
                    Terminus::EventHandler::QueueEvent(event);
                    
                    return;
                }
                if(_Action == GLFW_RELEASE)
                {
                    // Fire Released Event
                    InputActionEvent* event = new InputActionEvent(action, 0);
                    Terminus::EventHandler::QueueEvent(event);
                    
                    return;
                }
                
                m_keyboard.button_states[_Key] = _Action;
            }
            
            // Check if Axis Press
            if(_Action == GLFW_PRESS)
            {
                // Check if Positive Axis
                if(context->m_KeyboardAxisPositiveMap.find(_Key) != context->m_KeyboardAxisPositiveMap.end())
                {
                    std::string axis = context->m_KeyboardAxisPositiveMap[_Key];
                    
                    double last_value = m_keyboard.button_axis_states[_Key];
                    
                    // Fire Axis Positive Event
                    InputAxisEvent* event = new InputAxisEvent(axis, 1.0, 1.0 - last_value);
                    Terminus::EventHandler::QueueEvent(event);
                    
                    m_keyboard.button_axis_states[_Key] = 1.0;
                    
                    return;
                }
                
                // Check if Negative Axis
                if(context->m_KeyboardAxisNegativeMap.find(_Key) != context->m_KeyboardAxisNegativeMap.end())
                {
                    std::string axis = context->m_KeyboardAxisNegativeMap[_Key];
                    
                    double last_value = (double)m_keyboard.button_axis_states[_Key];
                    
                    // Fire Axis Negative Event
                    InputAxisEvent* event = new InputAxisEvent(axis, -1.0, -1.0 - last_value);
                    Terminus::EventHandler::QueueEvent(event);
                    
                    m_keyboard.button_axis_states[_Key] = -1.0;
                    
                    return;
                }
            }
            
            // Check if Axis Release
            if(_Action == GLFW_RELEASE)
            {
                if(context->m_KeyboardAxisPositiveMap.find(_Key) != context->m_KeyboardAxisPositiveMap.end())
                {
                    std::string axis = context->m_KeyboardAxisPositiveMap[_Key];
                    
                    double last_value = (double)m_keyboard.button_axis_states[_Key];
                    
                    // Fire Axis Positive Event
                    InputAxisEvent* event = new InputAxisEvent(axis, 0.0, 0.0 - last_value);
                    Terminus::EventHandler::QueueEvent(event);
                    
                    m_keyboard.button_axis_states[_Key] = 0.0;
                    
                    return;
                }
                if(context->m_KeyboardAxisNegativeMap.find(_Key) != context->m_KeyboardAxisNegativeMap.end())
                {
                    std::string axis = context->m_KeyboardAxisNegativeMap[_Key];
                    
                    double last_value = (double)m_keyboard.button_axis_states[_Key];
                    
                    // Fire Axis Negative Event
                    InputAxisEvent* event = new InputAxisEvent(axis, 0.0, 0.0 - last_value);
                    Terminus::EventHandler::QueueEvent(event);
                    
                    m_keyboard.button_axis_states[_Key] = 0.0;
                    
                    return;
                }
            }
        }
    }
    
    void ProcessMouseButtonInput(int _Key, int _Action)
    {
        if(m_PlayerContextList.size() != 0)
        {
            // For each Player
            InputContext* context = m_PlayerContextList[0].m_ActiveContext;
            
            if(!context)
                return;
            
            // Check if State
            if(context->m_MouseStateMap.find((uint8)_Key) != context->m_MouseStateMap.end() && _Action == GLFW_PRESS)
            {
                std::string state = context->m_MouseStateMap[_Key];
                // Fire Event
                InputStateEvent* event = new InputStateEvent(state);
                Terminus::EventHandler::QueueEvent(event);
                
                return;
            }
            
            // Check if Action
            if(context->m_MouseActionMap.find((uint8)_Key) != context->m_MouseActionMap.end())
            {
                std::string action = context->m_MouseActionMap[_Key];
                
                if(_Action == GLFW_PRESS)
                {
                    // Fire Pressed Event
                    InputActionEvent* event = new InputActionEvent(action, 1);
                    Terminus::EventHandler::QueueEvent(event);
                    
                    return;
                }
                if(_Action == GLFW_RELEASE)
                {
                    // Fire Released Event
                    InputActionEvent* event = new InputActionEvent(action, 0);
                    Terminus::EventHandler::QueueEvent(event);
                    
                    return;
                }
            }
        }
        
        m_mouse.button_states[_Key] = _Action;
    }
    
    void ProcessCursorInput(double _Xpos, int _Ypos)
    {
        if(m_PlayerContextList.size() != 0)
        {
            InputContext* context = m_PlayerContextList[0].m_ActiveContext;
            
            if(!context)
                return;
            
            for (auto it : context->m_MouseAxisMap)
            {
                if(it.first == MOUSE_AXIS_X)
                {
                    double last_position = m_mouse.x_position;
                    // Fire Mouse Axis Event
                    InputAxisEvent* event = new InputAxisEvent("xaxis", _Xpos, _Xpos - last_position);
                    Terminus::EventHandler::QueueEvent(event);
                }
                if(it.first == MOUSE_AXIS_Y)
                {
                    double last_position = m_mouse.y_position;
                    // Fire Mouse Axis Event
                    InputAxisEvent* event = new InputAxisEvent("taxis", _Ypos, _Ypos - last_position);
                    Terminus::EventHandler::QueueEvent(event);
                }
            }
        }
        
        m_mouse.x_position = _Xpos;
        m_mouse.y_position = _Ypos;
    }

    void GamepadCallback(int _Joy, int _Event)
    {
        if(_Event == GLFW_CONNECTED)
        {

        }
        else if(_Event == GLFW_DISCONNECTED)
        {

        }
    }
}
