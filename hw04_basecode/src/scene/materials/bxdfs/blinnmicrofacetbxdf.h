#pragma once
#define _USE_MATH_DEFINES
#include <scene/materials/bxdfs/bxdf.h>
#include <math.h>

class BlinnMicrofacetBxDF : public BxDF
{
public:
//Constructors/Destructors
    BlinnMicrofacetBxDF() : BlinnMicrofacetBxDF(glm::vec3(0.5f), 1.0f)
    {}
    BlinnMicrofacetBxDF(const glm::vec3 &color) : BlinnMicrofacetBxDF(color, 1.0f)
    {}
    BlinnMicrofacetBxDF(const glm::vec3 &color, float exp) : BxDF(BxDFType(BSDF_REFLECTION | BSDF_SPECULAR)), reflection_color(color), exponent(exp)
    {}
//Functions
    virtual glm::vec3 EvaluateScatteredEnergy(const glm::vec3 &wo, const glm::vec3 &wi) const;
    glm::vec3 SampleAndEvaluateScatteredEnergy(const glm::vec3 &wo, glm::vec3 &wi_ret, float rand1, float rand2, float &pdf_ret) const;
    virtual glm::vec3 EvaluateHemisphereScatteredEnergy(const glm::vec3 &wo, int num_samples, const glm::vec2 *samples) const;
    glm::vec3 cosineSampleHemisphere(float u1, float u2) const;
    virtual float PDF(const glm::vec3 &wo, const glm::vec3 &wi) const;
//Member variables
    glm::vec3 reflection_color;
    float exponent;
};