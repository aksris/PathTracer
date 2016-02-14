#ifndef TOTALLIGHTINGINTEGRATOR_H
#define TOTALLIGHTINGINTEGRATOR_H
#include <raytracing/directlightingintegrator.h>

class TotalLightingIntegrator: public DirectLightingIntegrator
{
public:
    TotalLightingIntegrator();
    virtual glm::vec3 TraceRay(Ray r, unsigned int depth);
protected:
    std::mt19937 mersenne_generator;
    std::uniform_real_distribution<float> unif_distribution;
};

#endif // TOTALLIGHTINGINTEGRATOR_H
