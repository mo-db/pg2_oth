// foo.hpp
#pragma once

#include "core.hpp"
#include "app.hpp"
#include "graphics.hpp"
#include "render.hpp"

void foo_func(App& app);

class DataClass1 {
	public:
	std::vector<uint32_t> pixels;
	int width = 0;
	int height = 0;

	DataClass1(int width_, int height_) :
		pixels((uint32_t)width_ * (uint32_t)height_), 
		width(width_), height(height_) {
			std::cout << "constructor1 called" << std::endl;
		}

	size_t get_data_size() {
		return pixels.size();
	}
};

struct DataClass2 {
	uint32_t *pixels = nullptr;
	int width = 0;
	int height = 0;

	DataClass2(int width_, int height_) : width{width_}, height{height_} {
		pixels = new uint32_t[width_ * height_];
		std::cout << "constructor2 called" << std::endl;
	}
	~DataClass2() {
		std::cout << "destructor2 called" << std::endl;
		delete pixels;
	}
	size_t get_data_size() {
		return width * height;
	}
};

struct DataClass3 {
	std::unique_ptr<uint32_t[]> pixels{};
	int width = 0;
	int height = 0;

	DataClass3(int width_, int height_) : width{width_}, height{height_} {
		pixels = std::make_unique<uint32_t[]>(width_ * height_);
		std::cout << "constructor2 called" << std::endl;
	}
	size_t get_data_size() {
		return width * height;
	}
};

class Manager {
	public:
	// std::unique_ptr<DataClass> dc1_ptr{new DataClass{640, 480}};
	// DataClass dc1_obj{640, 480};

	// std::unique_ptr<DataClass2> dc2_ptr{new DataClass2{640, 480}};
	// std::unique_ptr<DataClass2> dc2_ptr_small{new DataClass2{100, 200}};

	DataClass1 dc1_obj{640, 480};
	DataClass1 dc1_obj_small{100, 200};

	DataClass2 dc2_obj{640, 480};
	DataClass2 dc2_obj_small{100, 200};

	DataClass3 dc3_obj{640, 480};
	DataClass3 dc3_obj_small{100, 200};
};

void bar_func();
