
class Grid;

enum class BorderShape {
    Rectangle,
    RoundedRectangle,
    Ellipse,
    Circle
};

enum class GridDirection {
    Row,
    Col
};

struct GridStyle {
    GridDirection gridDirection = GridDirection::Row;
    uint32_t background_color = NULL;
    uint32_t border_color = NULL;
    std::string text = "";
    BorderShape borderShape = BorderShape::Rectangle;
    float border_radius_width = NULL;
    float border_radius_height = NULL;
};

struct GridSize {
    std::string width = "100%";
    std::string height = "100%";
};

struct GridConfig {
    std::string id = "";
    std::vector<float> coordinates = std::vector<float> { 0.0f, 0.0f};
    GridSize size{};
    GridStyle style{};
    Grid* container = nullptr;
};