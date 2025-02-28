#include "Vector2.h"

float Dot(Vector2 v1, Vector2 v2) {
	float result{};
	result = v1.x * v2.x + v1.y * v2.y;
	return result;
}

float Length(Vector2 v) {
	float result{};
	result = sqrt(Dot(v, v));
	return result;
}