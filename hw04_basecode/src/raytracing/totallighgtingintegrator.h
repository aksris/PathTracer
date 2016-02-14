#ifndef TOTALLIGHGTINGINTEGRATOR
#define TOTALLIGHGTINGINTEGRATOR
#include <random>

#include <raytracing/integrator.h>

class TotalLightingIntegrator : public Integrator
{
public:
    TotalLightingIntegrator();
    virtual glm::vec3 TraceRay(Ray r, unsigned int depth);
    glm::vec3 DirectLights(Ray r, glm::vec3 &direction, float &bxdf_pdf, glm::vec3 &isx_point, int depth, float throughput);

protected:
    std::mt19937 mersenne_generator;
    std::uniform_real_distribution<float> unif_distribution;
};

#endif // TOTALLIGHGTINGINTEGRATOR
