#include "input.hpp"

void Input::mouseDown(MouseButton mouseButton, int x, int y) {
    auto it = mouseInputs.find(mouseButton);
    
    if(it != mouseInputs.end()) {
        it->second(x, y);
        heldMouseButtons.insert(mouseButton);
    } else {
        Log::warning("Unbound mouse button pressed.");
    }

    
}

void Input::mouseUp(MouseButton mouseButton, int x, int y) {
    auto it = mouseInputs.find(mouseButton);

    auto held = heldMouseButtons.find(mouseButton);

    if(it != mouseInputs.end()) {
        it->second(x, y);
    } else {
        Log::warning("Unbound mouse button released.");
    }

    if(held != heldMouseButtons.end()) {
        heldMouseButtons.erase(mouseButton);
    }

}

bool Input::mouseIsHeld(MouseButton mouseButton) {
    return heldMouseButtons.contains(mouseButton);
}

bool Input::keyIsHeld(Key key) {
    return heldKeys.contains(key);
}

void Input::keyDown(Key key) {
    auto it = inputs.find(key);

    if(it != inputs.end()) {
        heldKeys.insert(key);
        return it->second();
    }

    Log::warning("Unbound key pressed.");
}

void Input::keyUp(Key key) {
    auto it = inputs.find(key);

    if(it != inputs.end()) {
        heldKeys.erase(key);
        // Might add 'action on key up' here
    }
}

