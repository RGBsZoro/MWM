#include "CityBlock.h"
#include <cstdlib>

CityBlock::CityBlock(Point center, int rows, int cols) {
	float spacing = 700.0f;
	float scale = 1.5f;

	for (int r = 0; r < rows; r++) {
		for (int c = 0; c < cols; c++) {
			float x = center.x + (c - cols / 2) * spacing;
			float z = center.z + (r - rows / 2) * spacing;

			buildings.emplace_back(
				Point(x, 0, z),
				180 * scale, 180 * scale,
				(250 + rand() % 200)* scale
			);
		}
	}
}

void CityBlock::draw() {
	for (auto& b : buildings)
		b.draw();
}
