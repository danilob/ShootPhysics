#ifndef DRAW_H
#define DRAW_H

#include "math/quaternion.h"

class Scene;

namespace Draw{


    void GroundInfinity(int size = 100, int steps = 5);
    void SphereColor(Vec4 position, QuaternionQ orientation, Vec4 color, float radius);

    void Sphere(Vec4 position, QuaternionQ orientation, Vec4 color, float radius);
    void Sphere2Shadow(Vec4 position, QuaternionQ orientation, float radius);

    void Box(Vec4 position, QuaternionQ orientation, Vec4 scale, Vec4 color);
    void Box2Shadow(Vec4 position, QuaternionQ orientation, Vec4 scale);

    void Cylinder(Vec4 position, QuaternionQ orientation, Vec4 scale, Vec4 color);
    void Cylinder2Shadow(Vec4 position, QuaternionQ orientation, Vec4 scale);

    void TrajetoryBall(std::vector<Vec4> positions, Vec4 color);
    void Text(QString text, int x1_o2d, int x2_o2d, int x3_o2d, int x4_o2d, float posWin, float posHei, Vec4 color, int size);

    void Shadow(Scene *scene);


}
#endif // DRAW_H
