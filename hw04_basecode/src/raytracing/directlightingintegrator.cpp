#include "directlightingintegrator.h"

DirectLightingIntegrator::DirectLightingIntegrator() :
     mersenne_generator(), unif_distribution(0,1)
{

}

glm::vec3 localToWorld(glm::vec3 w_s, Intersection isx){
    glm::vec3 bitangentL = isx.bitangent;
    glm::vec3 normalL = isx.normal;
    glm::vec3 tangentL = isx.tangent;

    glm::mat4 transMatrix (
                tangentL.x, tangentL.y, tangentL.z, 0.0f,
                bitangentL.x, bitangentL.y, bitangentL.z, 0.0f,
                normalL.x, normalL.y, normalL.z, 0.0f,
                0.0f, 0.0f, 0.0f, 1.0f);
    w_s = glm::vec3(transMatrix * glm::vec4(w_s, 0.0f));
    return w_s;
}

glm::vec3 DirectLightingIntegrator::DirectLights(Ray r, glm::vec3& direction, float& bxdf_pdf, Intersection isx, glm::vec3& energy){
    glm::vec3 colour(0.0f);

        Intersection intersection;
        intersection = isx;//intersection_engine->GetIntersection(r);
        glm::vec3 offset_point = intersection.point + 0.1f * intersection.normal;
        if(intersection.object_hit == NULL){
            return colour;
        }
        if(intersection.object_hit->material->is_light_source){
            return intersection.object_hit->material->EvaluateScatteredEnergy(intersection, glm::vec3(0.0f),
                     -r.direction);
        }
//        isx_point = intersection.point;
        glm::vec3 light_colour;
        float light_pdf;
        float w_f, w_g;
        int light_nSamples = 1;
        int brdf_nSamples = 1;
        float brdf_pdf;
        Ray omega_j;
        glm::vec3 light_final_colour(0.0f);
        Geometry* light = scene->lights.at(rand() % scene->lights.size());
        for(int i = 0; i < light_nSamples; i++){
            float x = unif_distribution(mersenne_generator);
            float y = unif_distribution(mersenne_generator);

            Intersection rand_intersection = light->SampleLight(x ,y, offset_point, intersection_engine);
            if(rand_intersection.object_hit != light){
                continue;
            }
            omega_j = Ray(offset_point, glm::normalize(rand_intersection.point - offset_point));
            glm::vec3 wi;
            intersection.object_hit->material->SampleAndEvaluateScatteredEnergy(intersection, -r.direction, wi, brdf_pdf);
            light_pdf = rand_intersection.object_hit->RayPDF(rand_intersection, omega_j);

            if(fequal(light_pdf, 0.0f)){
                continue;
            }

            light_colour =
                    light->material->EvaluateScatteredEnergy(
                        rand_intersection,
                        -r.direction,
                        -omega_j.direction
                        );

            glm::vec3 brdf_colour =
                    intersection.object_hit->material->EvaluateScatteredEnergy(
                        intersection,
                        -r.direction,
                        omega_j.direction
                        );

            w_g = glm::pow(light_nSamples * light_pdf, 2.0f) /
                    (glm::pow(brdf_nSamples * brdf_pdf, 2.0f) + glm::pow(light_nSamples * light_pdf, 2.0f));
            light_final_colour +=
                    w_g *
                    light_colour *
                    brdf_colour *
                    glm::abs(glm::dot((omega_j.direction), (intersection.normal))) / light_pdf;

        }
        light_final_colour = light_final_colour * float(scene->lights.size()) / float(light_nSamples);
        //BIS
        glm::vec3 brdf_final_colour(0.0f);

        glm::vec3 brdf_colourb = intersection.object_hit->material->SampleAndEvaluateScatteredEnergy
                (intersection, -r.direction, omega_j.direction, brdf_pdf);

        direction = omega_j.direction;
        bxdf_pdf = brdf_pdf;
        energy = brdf_colourb;
        omega_j.origin = intersection.point + 0.0001f * omega_j.direction;
        Ray lightRay = Ray(omega_j.origin, omega_j.direction);
        Intersection light_inter = intersection_engine->GetIntersection(lightRay);

        light_nSamples = 1;

        if(!fequal(brdf_pdf, 0.0f)){

            light_pdf = intersection.object_hit->RayPDF(light_inter, omega_j);

            if(light_inter.object_hit && light_inter.object_hit == light){
                if(!fequal(light_pdf, 0.0f)){
                    w_f = glm::pow(brdf_nSamples * brdf_pdf, 2.0f) /
                        (glm::pow(brdf_nSamples * brdf_pdf, 2.0f) + glm::pow(light_nSamples * light_pdf, 2.0f));
                }
                else{
                    w_f = 1.0f;
                }
                light_colour = light_inter.object_hit->material->EvaluateScatteredEnergy(
                    light_inter,
                    -r.direction,
                    -omega_j.direction
                    );
            }
            else{
                w_f = 0.0f;
            }
            float abs_cos = glm::abs(glm::dot((omega_j.direction), (intersection.normal)));
            brdf_final_colour =
                    w_f *
                    light_colour *
                    brdf_colourb *
                    abs_cos / (brdf_pdf);
        }
        return colour = light_final_colour + brdf_final_colour;
}

glm::vec3 DirectLightingIntegrator::TraceRay(Ray r, unsigned int depth){

    glm::vec3 direction, energy;
    Intersection isx;
    float brdf_pdf;

    return DirectLights(r, direction, brdf_pdf, isx, energy);
}
