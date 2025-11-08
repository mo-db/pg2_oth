// core.hpp
#pragma once

#include <iostream>
#include <exception>
#include <numbers>
#include <memory>

#include <luajit-2.1/lua.hpp>
#include <SDL3/SDL.h>
#include <fmt/core.h>
#include "../include/exprtk.hpp"

namespace core {
constexpr double pi = std::numbers::pi;
constexpr double epsilon = 1e-6;

bool equal_epsilon(double x, double y);
} // namespace gk
