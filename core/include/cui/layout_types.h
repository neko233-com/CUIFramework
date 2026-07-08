#pragma once

#include <cui/style.h>
#include <string>
#include <functional>
#include <vector>

namespace cui {

enum class FlexDirection {
    Row,
    Column
};

enum class Alignment {
    Start,
    Center,
    End,
    Stretch
};

enum class Sizing {
    Fixed,
    Grow,
    Shrink,
    FitContent
};

struct LayoutConfig {
    FlexDirection direction = FlexDirection::Column;
    Alignment crossAlignment = Alignment::Stretch;
    Padding padding;
    float gap = 0.0f;
    float width = 0.0f;
    float height = 0.0f;
    Sizing widthSizing = Sizing::Grow;
    Sizing heightSizing = Sizing::Grow;
};

struct LayoutResult {
    Rect bounds;
    std::vector<LayoutResult> children;
};

struct RenderCommand {
    enum class Type {
        Rect,
        Text,
        Border,
        ScissorStart,
        ScissorEnd
    };

    Type type;
    Rect bounds;
    Color color;
    CornerRadius cornerRadius;
    std::string text;
    float fontSize = 16.0f;
    Color textColor{255, 255, 255, 255};
    float borderWidth = 0.0f;
    Color borderColor{0, 0, 0, 0};
};

} // namespace cui
