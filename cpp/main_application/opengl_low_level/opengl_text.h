#ifndef OPENGL_TEXT_H_
#define OPENGL_TEXT_H_

#include <string>

#include "dvs/math/math.h"

using namespace dvs;

/*void drawTextFromBottomRight(const std::string s, float x, float y);
void drawTextFromBottomLeft(const std::string s, float x, float y);
void drawTextFromTopRight(const std::string s, float x, float y);
void drawTextFromTopLeft(const std::string s, float x, float y);

double calculateStringWidth(const std::string& s);
double calculateStringHeight();*/

void putTextAt(
    const std::string& text, const double x, const double y, const float size_x, const float size_y, const float scale);
void putTextAtNew(const std::string& text);
void putTextAt_Old(const std::string& s, const Vec2Dd& v);
void putTextAt3D(const std::string& s, const Vec3Dd& v);
void putTextAt_Old(const std::string& s, const double x, const double y);
void putTextAt3D(const std::string& s, const double x, const double y, const double z);

#endif
