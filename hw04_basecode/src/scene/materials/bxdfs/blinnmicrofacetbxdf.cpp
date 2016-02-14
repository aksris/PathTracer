#include <scene/materials/bxdfs/blinnmicrofacetbxdf.h>
float FresnelTerm(float cosi, float eta_i, float eta_o) {
    // Do Fresnel equation.
    float air_ior = 1.00029;
    float vacuum_ior = 1;
    float incident_media_ior = eta_i;
    float transmitted_media_ior = eta_o;

    cosi = (cosi > 1.f) ? 1.f : cosi;
    cosi = (cosi < -1.f) ? -1.f : cosi;

    float ei = incident_media_ior;
    float et = transmitted_media_ior;
    if (!(cosi > 0)) {
        std::swap(ei, et);
    }

    float sint = ei/et * sqrt(fmax(0.f, 1.f - cosi*cosi));
    if (sint >= 1.f) {
        // total internal reflection.
        return 1.f;
    }

    float cost = sqrt(fmax(0.f, 1.f - sint*sint));

    cosi = fabsf(cosi);
    float r_parallel = (et * cosi - ei * cost)
                     / (et * cosi + ei * cost);
    float r_perpendicular = (ei * cosi - et * cost)
                     / (ei * cosi + et * cost);
    float result = (0.5f * (r_parallel * r_parallel + r_perpendicular * r_perpendicular));
    return result;
}
glm::vec3 BlinnMicrofacetBxDF::EvaluateScatteredEnergy(const glm::vec3 &wo, const glm::vec3 &wi) const
{
    //TODO
    glm::vec3 normal(0.0f, 0.0f, 1.0f);

    float cos_wo = glm::abs(wo.z);
    float cos_wi = glm::abs(wi.z);
    if (fequal(cos_wi, 0.f) || fequal(cos_wo, 0.f)) {
        return glm::vec3(0);
    }
    glm::vec3 half_angle_vec = glm::normalize(wi + wo);
    float cosi = glm::dot(wo, half_angle_vec);
    float fresnel = FresnelTerm(cosi, 1.0f, 1.34f);

    float nDotHalf = fabs(half_angle_vec.z);
    float nDotWo = fabs(wo.z);
    float nDotWi = fabs(wi.z);
    float masking_term = (2 * nDotHalf * nDotWo) / nDotWo;
    float shadowing_term = (2 * nDotHalf * nDotWi) / nDotWo;
    float geom_term = fmin(1, fmin(masking_term, shadowing_term));

    float D_term = (exponent + 2.f)/(2.f * M_PI) * pow(fabs(half_angle_vec.z), exponent);
//    fresnel_part = 1.0f;

    glm::vec3 brdf = reflection_color * (D_term * geom_term * fresnel / (4.0f * cos_wo * cos_wi));
    return brdf;
}
float BlinnMicrofacetBxDF::PDF(const glm::vec3 &wo, const glm::vec3 &wi) const{
    glm::vec3 wh = glm::normalize(wo + wi);
    float costheta = glm::abs(wh.z);
    if(glm::dot(wo, wh) <= 0.0f){
        return 0.0f;
    }
    return ((exponent + 1.0f) * glm::pow(costheta, exponent)) /
            (2.0f * M_PI * 4.0f * glm::dot(wo, wh));
}
glm::vec3 BlinnMicrofacetBxDF::cosineSampleHemisphere(float u1, float u2) const{
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
        return glm::vec3(dx, dy, glm::sqrt(glm::max(0.0f, 1.0f - dx*dx - dy*dy)));
}
glm::vec3 SphericalDirection(float sin_theta, float cos_theta, float phi) {
    return glm::vec3(sin_theta * cosf(phi), sin_theta * sinf(phi), cos_theta);
}
float SameHemisphere(const glm::vec3 &v1, const glm::vec3 &v2)
{
    return v1.z * v2.z > 0.f;
}
glm::vec3 BlinnMicrofacetBxDF::SampleAndEvaluateScatteredEnergy(const glm::vec3 &wo, glm::vec3 &wi_ret, float rand1, float rand2, float &pdf_ret) const
{
    //TODO
    wi_ret = cosineSampleHemisphere(rand1, rand2);
    pdf_ret = PDF(wo, wi_ret);
    if(wo.z < 0.0f) wi_ret.z *= -1.0f;
    return EvaluateScatteredEnergy(wo, wi_ret);

    float cos_theta = powf(rand1, 1.f / (exponent+1));
    float sin_theta = sqrt(fmaxf(0.f, 1.f - cos_theta * cos_theta));
    float phi = rand2 * 2.f * M_PI;
    glm::vec3 wh = SphericalDirection(sin_theta, cos_theta, phi);
    wh = (!SameHemisphere(wo, wh)) ? -wh : wh;
    wi_ret = -wo + 2.f * glm::dot(wo, wh) * wh;
    pdf_ret = PDF(wo, wi_ret);
    return EvaluateScatteredEnergy(wo, wi_ret);
}
glm::vec3 BlinnMicrofacetBxDF::EvaluateHemisphereScatteredEnergy(const glm::vec3 &wo, int num_samples, const glm::vec2 *samples) const
{
    //TODO
    return glm::vec3(0);
}

