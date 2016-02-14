#include <scene/materials/weightedmaterial.h>

WeightedMaterial::WeightedMaterial() : Material(){}
WeightedMaterial::WeightedMaterial(const glm::vec3 &color) : Material(color){}

glm::vec3 WeightedMaterial::EvaluateScatteredEnergy(const Intersection &isx, const glm::vec3 &woW, const glm::vec3 &wiW, BxDFType flags) const
{
    //TODO
    glm::vec3 bitangentL = isx.bitangent;
    glm::vec3 normalL = isx.normal;
    glm::vec3 tangentL = isx.tangent;

    glm::mat4 transMatrix (
               tangentL.x, tangentL.y, tangentL.z, 0.0f,
               bitangentL.x, bitangentL.y, bitangentL.z, 0.0f,
               normalL.x, normalL.y, normalL.z, 0.0f,
               0.0f, 0.0f, 0.0f, 1.0f);
    transMatrix = glm::inverse(transMatrix);
//    woW = glm::vec3(transMatrix * glm::vec4(woW, 0.0f));
//    wiW_ret = glm::vec3(transMatrix * glm::vec4(wiW_ret, 0.0f));
    glm::vec3 woL = glm::vec3(transMatrix * glm::vec4(woW, 0.0f));
    glm::vec3 wiL = glm::vec3(transMatrix * glm::vec4(wiW, 0.0f));
    glm::vec3 bxdf;
    for (int i = 0; i < bxdf_weights.size(); i++){
        bxdf += bxdfs.at(i)->EvaluateScatteredEnergy(woL, wiL)* isx.texture_color * this->base_color * bxdf_weights.at(i);
    }
    transMatrix = glm::inverse(transMatrix);
    //wiW = glm::vec3(transMatrix * glm::vec4(wiL, 0.0f));
    return bxdf;
}

glm::vec3 WeightedMaterial::SampleAndEvaluateScatteredEnergy(const Intersection &isx, const glm::vec3 &woW, glm::vec3 &wiW_ret, float &pdf_ret, BxDFType flags) const
{
    //TODO
    float rand1 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
    float rand2 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
    glm::vec3 bitangentL = isx.bitangent;
    glm::vec3 normalL = isx.normal;
    glm::vec3 tangentL = isx.tangent;

    glm::mat4 transMatrix (
               tangentL.x, tangentL.y, tangentL.z, 0.0f,
               bitangentL.x, bitangentL.y, bitangentL.z, 0.0f,
               normalL.x, normalL.y, normalL.z, 0.0f,
               0.0f, 0.0f, 0.0f, 1.0f);
    transMatrix = glm::inverse(transMatrix);
//    woW = glm::vec3(transMatrix * glm::vec4(woW, 0.0f));
//    wiW_ret = glm::vec3(transMatrix * glm::vec4(wiW_ret, 0.0f));
    //wiW_ret = pdf_ret * wiW_ret;
    glm::vec3 woL = glm::vec3(transMatrix * glm::vec4(woW, 0.0f));
    glm::vec3 wiL = glm::vec3(transMatrix * glm::vec4(wiW_ret, 0.0f));
    glm::vec3 bxdf;
    for(int i = 0; i < bxdf_weights.size(); i++){
        bxdf += bxdfs.at(i)->SampleAndEvaluateScatteredEnergy(woL, wiL, rand1, rand2, pdf_ret)* isx.texture_color * this->base_color * bxdf_weights.at(i);
    }
    transMatrix = glm::inverse(transMatrix);
    wiW_ret = glm::vec3(transMatrix * glm::vec4(wiL, 0.0f));
    return bxdf;
}
