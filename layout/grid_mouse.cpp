
#include "grid.hpp"

bool Grid::containsLocation(int x, int y) {

    auto& coords = this->getAbsoluteCoords(); // should make this const at some point to satisfy best practices.
    return (
        x >= coords[0]
        && x <= coords[2]
        && y >= coords[1]
        && y <= coords[3]
    );
}

Grid* Grid::getLeafAtLocation(int x, int y) {

     if(!this->containsLocation(x, y) || !this->getInteractable()) {
        return nullptr;
    }

    Grid* currentGrid = this;

    // using recursion would break down for many elements. I know I've used recursion elsewhere in this program though - I should move away from recursion if I plan on reusing the code for more complex projects.
    while(true) {
        std::vector<Grid*> elements = currentGrid->getElements();

        bool foundInteractiveLeaf = false;

        // from the last rendered element to the first rendered element (to ensure on-top elemnts within he same container are selected), iterate.
        for(int i = (int)elements.size() - 1; i >= 0; i--) {
            // A key assumption here is that the person writing the ui code knows that elements should be spacially contained within their container, and that elements do not overlap.
            if(elements[i]->containsLocation(x, y) && elements[i]->getInteractable()) {
                currentGrid = elements[i];  // found a valid element, set to current and continue.
                foundInteractiveLeaf = true;        
                break;
            }
        }

        if(!foundInteractiveLeaf) {
            return currentGrid; // current grid was searched and no interactable contained elements. Then this element is the element being clicked.
        }; // exit condition, looped elements and found nothing, this is the leaf node.
        
    }
}