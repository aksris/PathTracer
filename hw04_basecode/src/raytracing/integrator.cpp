#include <raytracing/integrator.h>


Integrator::Integrator():
    max_depth(5)
{
    scene = NULL;
    intersection_engine = NULL;
}

glm::vec3 ComponentMult(const glm::vec3 &a, const glm::vec3 &b)
{
    return glm::vec3(a.x * b.x, a.y * b.y, a.z * b.z);
}

//Basic ray trace
glm::vec3 Integrator::TraceRay(Ray r, unsigned int depth){
    return glm::vec3(0.0f);
}
//{
//    //TODO
//    glm::vec3 colour(0.0f);
//    if(depth == 5){
//        return colour;
//    }
//    Intersection intersection;
//    intersection = intersection_engine->GetIntersection(r);

//    if(intersection.object_hit == NULL){
//        return colour;
//    }

//    for (Geometry* light : scene->lights){
//        Ray ray_light(intersection.point, glm::vec3(light->transform.invT() * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)));
               
//    }
//    return glm::vec3(0,0,0);
//}

void Integrator::SetDepth(unsigned int depth)
{
    max_depth = depth;
}
