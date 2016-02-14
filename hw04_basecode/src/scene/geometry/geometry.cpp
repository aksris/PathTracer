#include <scene/geometry/geometry.h>
#include <raytracing/intersectionengine.h>

float Geometry::RayPDF(const Intersection &isx, const Ray &ray)
{
    //TODO
    //The isx passed in was tested ONLY against us (no other scene objects), so we test if NULL
    //rather than if != this.
    if(isx.object_hit == NULL)
    {
        return 0;
    }
    //Add more here
    //distance between light origin and object surface
    float theta = glm::abs(glm::dot(isx.normal, -ray.direction));
    ComputeArea();
    return pow(glm::length(isx.point-ray.origin), 2.0f) / (theta * area);
}
