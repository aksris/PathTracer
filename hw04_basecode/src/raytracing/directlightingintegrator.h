#ifndef DIRECTLIGHTINGINTEGRATOR_H
#define DIRECTLIGHTINGINTEGRATOR_H
#include <random>

#include <raytracing/integrator.h>

class DirectLightingIntegrator : public Integrator
{
public:
    DirectLightingIntegrator();
    virtual glm::vec3 TraceRay(Ray r, unsigned int depth);
    glm::vec3 DirectLights(Ray r, glm::vec3 &direction, float &bxdf_pdf, Intersection isx, glm::vec3 &energy);

protected:
    std::mt19937 mersenne_generator;
    std::uniform_real_distribution<float> unif_distribution;
};

#endif // DIRECTLIGHTINGINTEGRATOR_H
