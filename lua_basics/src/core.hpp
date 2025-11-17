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

using Clock = std::chrono::steady_clock;
using TimePoint = std::chrono::time_point<Clock>;
using ms = std::chrono::duration<double, std::milli>;

bool equal_epsilon(double x, double y);
} // namespace core
