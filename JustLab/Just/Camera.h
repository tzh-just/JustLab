//================================================
// 相机
//================================================

#pragma once

#include "Global.h"
#include "Math/Vector.h"
#include "Math/Matrix.h"
#include "Graphics/Transform.h"
#include "Geometry/Ray.h"

namespace Just {

    struct Camera {
        Transform transform;
        Vector3f target;
        Vector3f up;

        float fov;
        float aspectRatio;
        float near, far;

        Camera(const Vector3f &position_, const Vector3f &target_, float near_, float far_, float fov_, float ratio_)
                : transform(Transform(position_)), target(target_), near(near_), far(far_), fov(fov_), aspectRatio(ratio_) {}

        Ray3f CastRay(){

        }
    };

    struct PerspectiveCamera : public Camera {

    };
    struct OrthogonalCamera : public Camera {

    };
}