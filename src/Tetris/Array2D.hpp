#pragma once
#include <vector>
#include <iostream>
#include <iomanip>

template<class T>
class Array2D {
public:
	unsigned long long width;
	unsigned long long height;

	explicit Array2D(int width, int height) : Array2D(width, height, T()) {}
	explicit Array2D(int width, int height, T defaultValue) : width(width), height(height), array(std::vector<T>(width * height, defaultValue)) {}

	// ReSharper disable once CppNonExplicitConvertingConstructor
	Array2D(const std::vector<std::vector<T>>& v) {
		height = v.size();

		width = 0;
		for(const auto& sub: v) {
			if (width < sub.size()) width = sub.size();
		}
		
		array = std::vector<T>();
		array.reserve(width * height);

		for (const auto& sub: v) {
			array.insert(array.end(), sub.begin(), sub.end());

			if (sub.size() < width) {
				array.insert(array.end(), width - sub.size(), T());
			}
		}
	}

	//Array2D(Array2D& that) {
	//	width = that.width;
	//	height = that.height;
	//	array = std::vector<T>(width * height);
	//	std::copy(that.array.begin(), that.array.end(), array.begin());
	//}

	//Array2D(Array2D&& that) noexcept {
	//	width = that.width;
	//	height = that.height;
	//	array = std::vector<T>(width * height);
	//	std::copy(that.array.begin(), that.array.end(), array.begin());
	//}

	//Array2D& operator=(Array2D that) noexcept {
	//	width = that.width;
	//	height = that.height;
	//	array = std::vector<T>(width * height);
	//	std::copy(that.array.begin(), that.array.end(), array.begin());
	//	return *this;
	//}

	T get(unsigned long long col, unsigned long long row) const {
		validate(col, row);
		return array[col + row * width];
	}

	void set(unsigned long long col, unsigned long long row, T value) {
		validate(col, row);
		array[col + row * width] = value;
	}

	void print() const {
		for (int row = 0; row < height; ++row) {
			for (int col = 0; col < width; ++col) {
				std::cout << std::setw(4) << array[col + row * width];
			}
			std::cout << std::endl << std::endl;
		}
	}

private:
	std::vector<T> array;

	void validate(unsigned long long col, unsigned long long row) const {
		if (col >= width || row >= height) {
			throw std::out_of_range("index out of array's bounds");
		}
	}
};
