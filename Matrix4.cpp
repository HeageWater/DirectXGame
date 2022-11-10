#include "Matrix4.h"

Matrix4 Matrix4::Queue() {
	m[0][0] = 1;
	m[1][1] = 1;
	m[2][2] = 1;
	m[3][3] = 1;

	return *this;
}

// 0‚ð‘ã“ü
Matrix4 Matrix4::Reset() {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			m[i][j] = 0;
		}
	}

	m[0][0] = 1;
	m[1][1] = 1;
	m[2][2] = 1;
	m[3][3] = 1;

	return *this;
}