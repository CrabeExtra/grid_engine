#pragma once

#include <unordered_map>
#include <unordered_set>
#include <functional>

#include "key.hpp"
#include "log.hpp"
#include "grid.hpp"

/// @brief Input handler. This sits between the platform and the app. Allows for the app to automatically or manually set key actions.
///         TODO: handle for key double presses and key holds.
class Input {
    public:

        // mouse handlers
        void mouseDown(MouseButton mouseButton, int x, int y);
        void mouseUp(MouseButton mouseButton, int x, int y);
        bool mouseIsHeld(MouseButton mouseButton);

        // mouse setters
        void setMouseButton(MouseButton mouseButton, std::function<void(int, int)> funcInput) { mouseInputs.insert_or_assign(mouseButton, std::move(funcInput)); }

        // keyboard handlers
        void keyDown(Key key);
        void keyUp(Key key);
        bool keyIsHeld(Key key); // Checks if a key is still down.
        
        // keyboard setters
        /// @brief This sets the provided key to the provided function in inputs.
        void setInput(Key key, std::function<void()> funcInput) { inputs.insert_or_assign(key, std::move(funcInput)); }

    private:

        std::unordered_map<Key, std::function<void()>> inputs;
        std::unordered_map<MouseButton, std::function<void(int, int)>> mouseInputs;
        std::unordered_set<Key> heldKeys;
        std::unordered_set<MouseButton> heldMouseButtons;
};