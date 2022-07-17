#include "Widget.hpp"

Widget::Widget(std::string name, int* width, int* height) : name(std::move(name)), width(width), height(height) {}

Widget::Widget(std::string name, int width, int height) : Widget(name, new int(width), new int(height)) {}
