#pragma once

#include <cui/style.h>
#include <cui/layout_types.h>
#include <string>
#include <vector>
#include <functional>

namespace cui::ui {

struct LayoutNode {
    std::string id;
    LayoutConfig config;
    std::vector<LayoutNode> children;

    // Resolved bounds (after layout)
    Rect bounds;
};

class LayoutSolver {
public:
    LayoutResult solve(const LayoutNode& root, float available_width, float available_height);

private:
    void solve_node(const LayoutNode& node, Rect available, LayoutResult& result);
};

} // namespace cui::ui
