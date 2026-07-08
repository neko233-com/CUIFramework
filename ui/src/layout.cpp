#include <cui/ui/layout.h>
#include <algorithm>

namespace cui::ui {

LayoutResult LayoutSolver::solve(const LayoutNode& root, float available_width, float available_height) {
    LayoutResult result;
    Rect available{0, 0, available_width, available_height};
    solve_node(root, available, result);
    return result;
}

void LayoutSolver::solve_node(const LayoutNode& node, Rect available, LayoutResult& result) {
    auto& cfg = node.config;

    // Calculate content area (subtract padding)
    Rect content_area = available;
    content_area.x += cfg.padding.left;
    content_area.y += cfg.padding.top;
    content_area.width -= cfg.padding.left + cfg.padding.right;
    content_area.height -= cfg.padding.top + cfg.padding.bottom;

    result.bounds = available;

    if (node.children.empty()) {
        // Leaf node — use explicit sizing or fill parent
        if (cfg.widthSizing == Sizing::Fixed) {
            result.bounds.width = cfg.width;
        } else {
            result.bounds.width = content_area.width;
        }
        if (cfg.heightSizing == Sizing::Fixed) {
            result.bounds.height = cfg.height;
        } else {
            result.bounds.height = content_area.height;
        }
        return;
    }

    // Layout children
    float offset = 0;
    bool horizontal = (cfg.direction == FlexDirection::Row);

    // First pass: calculate total fixed size and count of grow children
    float total_fixed = 0;
    int grow_count = 0;
    float total_gap = cfg.gap * (node.children.size() - 1);

    for (auto& child : node.children) {
        if (horizontal) {
            if (child.config.widthSizing == Sizing::Fixed) {
                total_fixed += child.config.width;
            } else {
                grow_count++;
            }
        } else {
            if (child.config.heightSizing == Sizing::Fixed) {
                total_fixed += child.config.height;
            } else {
                grow_count++;
            }
        }
    }

    // Available space for grow children
    float available_space = (horizontal ? content_area.width : content_area.height)
                          - total_fixed - total_gap;
    float grow_size = (grow_count > 0) ? available_space / grow_count : 0;

    // Second pass: layout children
    for (size_t i = 0; i < node.children.size(); i++) {
        auto& child = node.children[i];
        LayoutResult child_result;

        Rect child_rect;
        child_rect.x = content_area.x + (horizontal ? offset : 0);
        child_rect.y = content_area.y + (!horizontal ? offset : 0);

        if (horizontal) {
            child_rect.width = (child.config.widthSizing == Sizing::Fixed)
                             ? child.config.width : grow_size;
            child_rect.height = content_area.height;
        } else {
            child_rect.width = content_area.width;
            child_rect.height = (child.config.heightSizing == Sizing::Fixed)
                              ? child.config.height : grow_size;
        }

        solve_node(child, child_rect, child_result);
        result.children.push_back(std::move(child_result));

        offset += (horizontal ? child_rect.width : child_rect.height) + cfg.gap;
    }
}

} // namespace cui::ui
