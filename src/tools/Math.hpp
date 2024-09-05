#pragma once

#include "Common.hpp"

namespace Math {
double lerp(double a, double b, double t);
double move_to(double a, double b, double t);

int clamp(int min, int value, int max);
float clamp(float min, float value, float max);
int round(double value);
int floor(double value);
double sign(double value);
double abs(double value);
float fabs(float value);
double pow(double base, double exponent);
double sqrt(double value);

int percentage(int x, int y);

int dist(int a, int b);
int dist_vec(vec2 a, vec2 b);

bool approx(int a, int b, int tolerance = 10);

int mid(int a, int b);
vec2 mid_vec(vec2 a, vec2 b);
void normalize(vec2 &v);
}; // namespace Math
