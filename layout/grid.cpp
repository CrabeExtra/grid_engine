#include "grid.hpp"
#include "log.hpp"

// TODO: this is a bit inefficient, it has to check ALL levels of hierarchy to get the width and height in pixels every time.

float Grid::getWidthPx() const {
    Grid* container = this->getContainer();
    
    return strToWidthPx( 
        this->width, 
        container ? container->getWidthPx() : std::optional<float>{}
    );

}

float Grid::getHeightPx() const {
    Grid* container = this->getContainer();
    
    return strToHeightPx( 
        this->height, 
        container ? container->getHeightPx() : std::optional<float>{}
    );
}

float Grid::strToWidthPx(std::string str, std::optional<float> containerWidth) const {
    size_t len = str.size();

    if(str[len - 1] == '%') {
        float percentage = std::stof(str.substr(0, len-1));
        
        if(!containerWidth) return 0;

        return percentage * *containerWidth / 100;
    } else if(str.substr(len - 2, 2) == "px") {
        str = str.substr(0, len - 2);
    }

    return std::stof(str);
}

float Grid::strToHeightPx(std::string str, std::optional<float> containerHeight) const {
    size_t len = str.size();

    if(str[len - 1] == '%') {
        float percentage = std::stof(str.substr(0, len-1));
        
        if(!containerHeight) return 0;

        return percentage * *containerHeight / 100;
    } else if(str.substr(len - 2, 2) == "px") {
        str = str.substr(0, len - 2);
    }
    
    return std::stof(str);
}

void Grid::addElement(Grid* elem) {
    switch(elem->getGridDirection()) {
        case GridDirection::Row:
            return addRow(elem);
        case GridDirection::Col:
            return addCol(elem);
    }
}

// TODO: think about overflow.
void Grid::addCol(Grid* col) {
    
    const std::vector<float>& coords = col->getCoordinates();
    const std::vector<float>& containerCoords = col->getContainer()->getAbsoluteCoords();

    std::vector<float>& absoluteCoords = col->getAbsoluteCoords();
    std::vector<Grid*>& elementsInThisContainer = col->getContainer()->getElements();
    
    absoluteCoords.resize(4);

    float width = col->getWidthPx();
    float height = col->getHeightPx();

    if(elementsInThisContainer.empty()) {
        // can set starting coords to same as container.
        absoluteCoords[0] = coords[0] + containerCoords[0];
        absoluteCoords[2] = coords[0] + width;

        absoluteCoords[1] = coords[1] + containerCoords[1];
        absoluteCoords[3] = absoluteCoords[1] + height;
    } else {
        Grid* latestElement = elementsInThisContainer[elementsInThisContainer.size() - 1];
        const std::vector<float>& latestElemCoords = latestElement->getAbsoluteCoords();

        absoluteCoords[0] = latestElemCoords[2] + coords[0];
        absoluteCoords[2] = absoluteCoords[0] + width;

        absoluteCoords[1] = containerCoords[1] + coords[1];
        absoluteCoords[3] = absoluteCoords[1] + height;
    }

    elementsInThisContainer.push_back(col);
}

void Grid::addRow(Grid* row) {
    const std::vector<float>& coords = row->getCoordinates();
    const std::vector<float>& containerCoords = row->getContainer()->getAbsoluteCoords();
    
    std::vector<float>& absoluteCoords = row->getAbsoluteCoords();
    std::vector<Grid*>& elementsInThisContainer = row->getContainer()->getElements();

    float width = row->getWidthPx();
    float height = row->getHeightPx();
    
    absoluteCoords.resize(4);

    if(elementsInThisContainer.empty()) {
        absoluteCoords[0] = coords[0] + containerCoords[0];
        absoluteCoords[2] = absoluteCoords[0] + width;

        absoluteCoords[1] = coords[1] + containerCoords[1];
        absoluteCoords[3] = absoluteCoords[1] + height;
    } else {
        Grid* latestElement = elementsInThisContainer[elementsInThisContainer.size() - 1];
        const std::vector<float>& latestRowCoords = latestElement->getAbsoluteCoords();

        float latestRowHeight = latestElement->getHeightPx();

        absoluteCoords[0] = coords[0] + containerCoords[0];
        absoluteCoords[2] = absoluteCoords[0] + width;

        absoluteCoords[1] = coords[1] + latestRowCoords[3];
        absoluteCoords[3] = absoluteCoords[1] + height;
    }   
    
    elementsInThisContainer.push_back(row);
}
