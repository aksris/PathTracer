#pragma once
#include <scene/geometry/geometry.h>
#include <raytracing/intersectionengine.h>

//A square is aligned with the XY plane with a normal aligned with the positive Z axis. Its side length is 1, and it is centered at the origin.
//These attributes can be altered by applying a transformation matrix to the square.
class SquarePlane : public Geometry
{
    Intersection GetIntersection(Ray r);
    virtual glm::vec2 GetUVCoordinates(const glm::vec3 &point);
    virtual glm::vec3 ComputeNormal(const glm::vec3 &P);
    virtual Intersection SampleLight(float x, float y, glm::vec3 ray_origin, IntersectionEngine *i_engine);
    void ComputeTangents(const glm::vec3 &normal, glm::vec3 &tangent, glm::vec3 &bitangent);

    void create();

    virtual void ComputeArea();
};
