#pragma once

#include "Vector.h"
#include "Matrix.h"
#include "Just/Graphic/Transform.h"

namespace Just {

    //绕x轴旋转矩阵
    inline Matrix4f XRotationMatrix(float angle) {
        return {
                {1, 0,          0,           0},
                {0, Cos(angle), -Sin(angle), 0},
                {0, Sin(angle), Cos(angle),  0},
                {0, 0,          0,           1}
        };
    }

    //绕y轴旋转矩阵
    inline Matrix4f YRotationMatrix(float angle) {
        return {
                {Cos(angle),  0, Sin(angle), 0},
                {0,           1, 0,          0},
                {-Sin(angle), 0, Cos(angle), 0},
                {0,           0, 0,          1}
        };
    }

    //绕z轴旋转矩阵
    inline Matrix4f ZRotationMatrix(float angle) {
        return {
                {Cos(angle), -Sin(angle), 0, 0},
                {Sin(angle), Cos(angle),  0, 0},
                {0,          0,           1, 0},
                {0,          0,           0, 1}
        };
    }

    inline Matrix4f RotationMatrix(float x, float y, float z) {
        return ZRotationMatrix(z) * YRotationMatrix(y) * XRotationMatrix(x);
    }

    inline Matrix4f RotationMatrix(const Vector3f &rotation) {
        return ZRotationMatrix(rotation.z) * YRotationMatrix(rotation.y) * XRotationMatrix(rotation.x);
    }

    inline Matrix4f RotationMatrix(const Vector3f &i, const Vector3f &j, const Vector3f &k) {
        return {
                {i.x, j.x, k.x, 0},
                {i.y, j.y, k.y, 0},
                {i.z, j.z, k.z, 0},
                {0,   0,   0,   1}
        };
    }

    //缩放矩阵
    inline Matrix4f ScaleMatrix(float x, float y, float z) {
        return {
                {x, 0, 0, 0},
                {0, y, 0, 0},
                {0, 0, z, 0},
                {0, 0, 0, 1}
        };
    }

    //缩放矩阵
    inline Matrix4f ScaleMatrix(const Vector3f &scale) {

        return {
                {scale.x, 0,       0,       0},
                {0,       scale.y, 0,       0},
                {0,       0,       scale.z, 0},
                {0,       0,       0,       1}
        };
    }

    //平移矩阵
    inline Matrix4f TranslationMatrix(float x, float y, float z) {
        return {
                {1, 0, 0, x},
                {0, 1, 0, y},
                {0, 0, 1, z},
                {0, 0, 0, 1}
        };
    }

    //平移矩阵
    inline Matrix4f TranslationMatrix(const Vector3f &position) {
        return {
                {1, 0, 0, position.x},
                {0, 1, 0, position.y},
                {0, 0, 1, position.z},
                {0, 0, 0, 1}
        };
    }

    inline Matrix4f ObjectToWorld(const Transform &transform) {
        return TranslationMatrix(transform.position) * ScaleMatrix(transform.scale) * RotationMatrix(transform.rotation);
    }

    inline Matrix4f ObjectToWorld(const Vector3f &position, const Vector3f &scale, const Vector3f &rotation) {
        return TranslationMatrix(position) * ScaleMatrix(scale) * RotationMatrix(rotation);
    }

    //视图矩阵
    inline Matrix4f LookAt(const Vector3f &origin, const Vector3f &target, const Vector3f &up) {
        Vector3f g = Normalize(target - origin);
        Vector3f gxt = Normalize(Cross(g, up));
        Vector3f t = Cross(gxt, g);
        return Transpose(RotationMatrix(gxt, t, -g)) * TranslationMatrix(-origin);
    }


    //正交投影变换矩阵
    inline Matrix4f OrthogonalProject(float aspectRatio, float fov, float n, float f) {
        //参数far和near为远近平面的长度，即正值
        float t = Tan(ConvertDegreesToRadians(fov / 2)) * n;
        float r = t * aspectRatio;

        //直接构建
        return {
                {1 / r, 0,     0,           0},
                {0,     1 / t, 0,           0},
                {0,     0,     2 / (f - n), -(f + n) / (f - n)},
                {0,     0,     0,           1}
        };

        //先位移后缩放
        Vector3f translation(0, 0, -(n + f) / 2);
        Vector3f scale(1 / r, 1 / t, 2 / (f - n));
        return ScaleMatrix(scale) * TranslationMatrix(translation);
    }

    //透视转正交矩阵
    inline Matrix4f PerspectiveToOrthogonal(float n, float f) {
        return {
                {n, 0, 0,        0},
                {0, n, 0,        0},
                {0, 0, -(n + f), -n * f},
                {0, 0, 1,        0}
        };
    }

    //透视投影变换矩阵
    inline Matrix4f PerspectiveProject(float aspectRatio, float fov, float n, float f) {
        //参数far和near为远近平面的长度，即正值
        float t = Tan(ConvertDegreesToRadians(fov / 2)) * n;
        float r = t * aspectRatio;

        //直接构建
        return {
                {n / r, 0,     0,                  0},
                {0,     n / t, 0,                  0},
                {0,     0,     -(n + f) / (f - n), -2 * f * n / (f - n)},
                {0,     0,     -1,                 0}
        };

        //先透视转正交再正交投影
        return OrthogonalProject(aspectRatio, fov, n, f) * PerspectiveToOrthogonal(n, f);
    }

    //视口变换矩阵
    inline Matrix4f ScreenMapping(const Vector2f &size) {
        return {
                {size.x / 2, 0,          0, size.x / 2},
                {0,          size.y / 2, 0, size.y / 2},
                {0,          0,          1, 0},
                {0,          0,          0, 1}
        };
    }

}