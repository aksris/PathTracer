#include <scene/materials/bxdfs/lambertBxDF.h>


glm::vec3 LambertBxDF::cosineSampleHemisphere(float u1, float u2) const{
    float sx = 2 * u1 - 1.0f;
    float sy = 2 * u2 - 1.0f;
    float r, theta;

    if (sx == 0.0 && sy == 0.0)
    {
        return glm::vec3(0,0,0);
    }
    if (sx >= -sy)
    {
        if (sx > sy)
        {
            // Handle first region of disk
            r = sx;
            if (sy > 0.0) theta = sy/r;
            else          theta = 8.0f + sy/r;
        }
        else
        {
            // Handle second region of disk
            r = sy;
            theta = 2.0f - sx/r;
        }
    }
    else
    {
        if (sx <= sy)
        {
            // Handle third region of disk
            r = -sx;
            theta = 4.0f - sy/r;
        }
        else
        {
            // Handle fourth region of disk
            r = -sy;
            theta = 6.0f + sx/r;
        }
    }
    theta *= PI / 4.f;
    float dx = r * cosf(theta);
    float dy = r * sinf(theta);
//    return glm::vec3(dx, dy, 0.0f);
    return glm::vec3(dx, dy, glm::sqrt(glm::max(0.0f, 1.0f - dx*dx - dy*dy)));
}

glm::vec3 LambertBxDF::EvaluateScatteredEnergy(const glm::vec3 &wo, const glm::vec3 &wi) const
{
    //TODO
    return diffuse_color / float(M_PI);
}
glm::vec3 LambertBxDF::SampleAndEvaluateScatteredEnergy(const glm::vec3 &wo, glm::vec3 &wi_ret, float rand1, float rand2, float &pdf_ret) const
{
    //TODO
    wi_ret = cosineSampleHemisphere(rand1, rand2);
    pdf_ret = PDF(wo, wi_ret);
    if(wo.z < 0.0f) wi_ret.z *= -1.0f;
    return EvaluateScatteredEnergy(wo, wi_ret);
}
float LambertBxDF::PDF(const glm::vec3 &wo, const glm::vec3 &wi) const{
    if(wo.z * wi.z > 0)
        return glm::abs(wi.z) / M_PI;
    return 0.0f;
}
glm::vec3 LambertBxDF::EvaluateHemisphereScatteredEnergy(const glm::vec3 &wo, int num_samples, const glm::vec2 *samples) const
{
    //TODO
    return glm::vec3(0);
}
