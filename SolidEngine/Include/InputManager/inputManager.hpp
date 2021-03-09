#pragma once

    #include <unordered_map>

    #include "Core/Debug/log.hpp"
    #include <GLFW/glfw3.h>

namespace Solid
{

    /**
     * @enum ImEnumDetectionType
     * @brief Different type of detection.
     */ 

    enum class ImEnumDetectionType
    {
        SWITCH,     // This type of input set On / Off each time pressed.
        PRESSED,    // This type of input set On if pressed and Off if released
        NO_REPEAT   // This type of input set On if pressed and Off at the next frame
    };

    /**
     * @brief Input manager for easyer use of input
     * 
     * @tparam T is the type of id key
     */
    template <typename T = int>
    class InputManager
    {
    private:
        // RC: nested class in template class is BAD! Multiple definition.
        enum class InputType
        {
            KEY,
            MOUSE,
            CONTROLLER
        };

        struct Input
        {
            ImEnumDetectionType detectionType;
            InputType inputType;
            int key;
            int joystickId;
            bool isPressed;     // RC: Bad name: an input is either pressed or released.
            bool isReleased;    // RC: Bad name: an input is either pressed or released.
        };
        GLFWwindow* window;
        std::unordered_map<T,Input> inputList;

        // RC: NORM ? 
        static void joystickCallBack(int jid, int event)
        {
            if(event == GLFW_CONNECTED)
            {
                /*if(glfwJoystickIsGamepad(jid))
                    _isJoystickXbox.insert({jid,true});
                else
                    _isJoystickXbox.insert({jid,false});*/
                Log::Send("Controller " + std::to_string(jid) + " connected",Log::ELogSeverity::INFO);
            }
            else if(event == GLFW_DISCONNECTED)
            {
                //_isJoystickXbox.erase(jid);
                Log::Send("Controller " + std::to_string(jid) + " disconnected",Log::ELogSeverity::INFO);
            }
        }

        bool getInputPressed(const Input& input)
        {
            if(input.inputType == InputType::KEY)
            {
                if(glfwGetKey(window,input.key) == GLFW_PRESS)
                    return true;
            }
            else if(input.inputType == InputType::MOUSE)
            {
                if(glfwGetMouseButton(window,input.key) == GLFW_PRESS)
                    return true;
            }
            else
            {
                int count;
                const unsigned char* buttons = glfwGetJoystickButtons(input.joystickId,&count);

                if(buttons == nullptr)
                    return false;
                
                if(buttons[input.key] == GLFW_PRESS)
                    return true;
                
            }

            return false;   
        }
    public:
        /**
         * @brief Construct input manager without window set
         */
        InputManager()
        {
            window = nullptr;
        }

        /**
         * @brief Construct a new Input Manager object
         * 
         * @param _window The pointer of the GLFW _window
         */
        InputManager(GLFWwindow* _window)
        {
            window = _window;

            glfwSetJoystickCallback(joystickCallBack);
        }

        ~InputManager() = default;

        /**
         * @brief Add keyboard input to list
         * 
         * @param id id of the input
         * @param key GLFW_KEY input
         * @param detectionType Type of input
         */
        void AddKeyInput(T id, int key, ImEnumDetectionType detectionType)
        {
            inputList.insert({id,{detectionType,InputType::KEY,key,-1,false,true}});
        }

        /**
         * @brief Add mouse input to list
         * 
         * @param id id of the input
         * @param key GLFW_MOUSE input
         * @param detectionType Type of input
         */
        void AddMouseInput(T id, int key, ImEnumDetectionType detectionType)
        {
            inputList.insert({id,{detectionType,InputType::MOUSE,key,-1,false,true}});
        }

        /**
         * @brief Add controller input to list (only button input)
         * 
         * @param id id of the input
         * @param glfwJoystick Number of the joystick
         * @param key GLFW_MOUSE input
         * @param detectionType Type of input
         */
        void AddControllerInput(T id, int glfwJoystick, int key, ImEnumDetectionType detectionType)
        {
            inputList.insert({id,{detectionType,InputType::CONTROLLER,key,glfwJoystick,false,true}});
        }
        
        /**
         * @brief Update every input added. Update each frame
         * 
         */
        void Update()
        {
            for (auto i = inputList.begin(); i != inputList.end(); i++)
            {
                Input& input = i->second;

                if(input.detectionType == ImEnumDetectionType::NO_REPEAT)
                    input.isPressed = false;

                bool inputPressed = getInputPressed(input);

                if(!input.isReleased && inputPressed)
                    continue;
                
                if(input.detectionType == ImEnumDetectionType::SWITCH)
                {
                    if(inputPressed)
                    {
                        input.isPressed = input.isPressed ? false : true;
                        input.isReleased = false;
                    }
                    else
                    {
                        input.isReleased = true;
                    }
                }
                else
                {
                    /**
                    * RC:
                    * input.isPressed = inputPressed;
                    * input.isReleased = !inputPressed;
                    */
                    if(inputPressed)
                    {
                        input.isPressed = true;
                        input.isReleased = false;
                    }
                    else
                    {
                        input.isPressed = false;
                        input.isReleased = true;
                    }
                }
            }
            
        }

        /**
         * @brief Return if the input is pressed
         * 
         * @param id the id of input
         * @return true if the input id is pressed, false instead
         */
        bool IsPressed(T id)
        {
            Input& input = inputList.at(id);
            bool isPressed = input.isPressed;

            return isPressed;
        }

        /**
         * @brief Return if the input is pressed
         * 
         * @param id the id of input
         * @return true if the input id is released, flase instead
         */
        bool IsReleased(T id)
        {
            return inputList.at(id).isReleased;
        }

        /**
         * @brief Set the cursor pos x in _posx and pos y in _posy
         * 
         * @param _posx set the x pos value of cursor
         * @param _posy set the y pos value of cursor
         */ 
        void GetCursorPos(double& _posx, double& _posy)
        {
            glfwGetCursorPos(window,&_posx,&_posy);
        }
        
        #if 0 // 0 = Vector2 not included, 1 = Vector2 included
        /**
         * @brief Get the cursor pos and return it into a Vector2
         * 
         * @return Vector2 x value is the pos x and y value is the pos y of the cursor
         */
        Vector2 getCursorPos()
        {
            Vector2 pos[2];

            glfwGetCursorPos(window,&pos.x,&pos.y);

            return pos;
        }
        #endif
        
        /**
         * @brief Set the specified axis value of the specified joystick id in _pos
         * 
         * @param _jid Joystick id 
         * @param _axeId The joystick axis id
         * @param _pos store the result
         */
        void GetAxis(int _jid, int _axeId, float& _pos)
        {
            int count;
            const float* axes = glfwGetJoystickAxes(_jid,&count);

            if(axes == nullptr || _axeId >= count)
                return;
            
            _pos = axes[_axeId];
            if(_pos < 0.005 && _pos > -0.005)
                _pos = 0;
        }

        /**
         * @brief Get the specified axis value of the specified joystick id
         * 
         * @param _jid Controller id
         * @param _axeId The joystick axis id
         * @return Value of the joystick axis
         */
        float GetAxis(int _jid, int _axeId)
        {
            int count;
            const float* axes = glfwGetJoystickAxes(_jid,&count);

            if(axes == nullptr || _axeId >= count)
                return 0;
            
            float pos = axes[_axeId];
            if(pos < 0.005 && pos > -0.005)
                pos = 0;
            
            return pos;
        }
        
        /**
         * @brief Set the value of the hat id and joystick id in _value
         * 
         * @param _jid Id of the joystick
         * @param _hatId Id of the hat
         * @param _value Store the value of the hat
         */
        void GetHat(int _jid, int _hatId, int& _value)
        {
            int count;
            const unsigned char* hats = glfwGetJoystickHats(_jid,&count);

            if(hats == nullptr || _hatId >= count)
                return;
            
            _value = hats[_hatId];
        }

        /**
         * @brief Get the value of the hat id and joystick id
         * 
         * @param _jid Id of the joystick
         * @param _hatId Id of the hat
         * @return the value of the hat
         */
        int GetHat(int _jid, int _hatId)
        {
            int count;
            const unsigned char* hats = glfwGetJoystickHats(_jid,&count);

            if(hats == nullptr || _hatId >= count)
                return 0;
            
            return hats[_hatId];
        }
    };

} //!namespace