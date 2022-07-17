#pragma once
#include "Widget.hpp"

/**
 *  Engine subsystem for drawing widgets.
 */
class WidgetManager {
    std::vector<Widget*> widgets;
public:
    WidgetManager() = default;

    void drawWidgets() const;

    void addWidget(Widget* widget);
    void removeWidget(Widget* widget);

    ~WidgetManager();
};
