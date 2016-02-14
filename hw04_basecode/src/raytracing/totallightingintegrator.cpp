#include "totallightingintegrator.h"

TotalLightingIntegrator::TotalLightingIntegrator():
    mersenne_generator(), unif_distribution(0,1)
{
    scene = NULL;
    intersection_engine = NULL;
}
glm::vec3 worldToObjectSpace(glm::vec3 world_ray_direction, Intersection intersection) {
    glm::vec3 normal = intersection.normal;
    glm::vec3 tangent = intersection.tangent;
    glm::vec3 bitangent = intersection.bitangent;

    glm::mat4 worldToObject = glm::transpose(
                glm::mat4(tangent.x,   tangent.y,   tangent.z,   0.0f,
                          bitangent.x, bitangent.y, bitangent.z, 0.0f,
                          normal.x,    normal.y,    normal.z,    0.0f,
                          0.0f,        0.0f,        0.0f,        1.0f)
                );

    return glm::vec3(worldToObject * glm::vec4(world_ray_direction, 0.0f));
}

glm::vec3 objectToWorldSpace(glm::vec3 world_ray_direction, Intersection intersection) {
    glm::vec3 normal = intersection.normal;
    glm::vec3 tangent = intersection.tangent;
    glm::vec3 bitangent = intersection.bitangent;

    glm::mat4 objectToWorld = glm::mat4(
                tangent.x,   tangent.y,   tangent.z,   0.0f,
                bitangent.x, bitangent.y, bitangent.z, 0.0f,
                normal.x,    normal.y,    normal.z,    0.0f,
                0.0f,        0.0f,        0.0f,        1.0f
                );

    return glm::vec3(objectToWorld * glm::vec4(world_ray_direction, 0.0f));
}
glm::vec3 TotalLightingIntegrator::TraceRay(Ray r, unsigned int depth){
    glm::vec3 color = glm::vec3(0.0f);
        // If recursion depth max hit, return black.
        if (depth > max_depth) {
            return color;
        }
        float OFFSET = 0.001f;
        Intersection intersection = intersection_engine->GetIntersection(r);
        glm::vec3 offset_point = intersection.point + (intersection.normal * OFFSET);
        // If no object intersected or the object is in shadow, return black.
        if (!intersection.object_hit) {
            return color;
        }

        // If we hit a light, just return the color of the light * energy.
        if (intersection.object_hit->material->is_light_source) {
            return intersection.object_hit->material->base_color
                    *intersection.object_hit->material->EvaluateScatteredEnergy(intersection, glm::vec3(0), -r.direction);
        }

        // Do integrated lighting, updating the following variables.
        glm::vec3 light_accum(0.f);
        glm::vec3 multiplier(1.f);
        Ray current_ray = r;
        Intersection current_intersection = intersection;
        int bounces = 0;
        float throughput = 1.f;

        while (true) {

            // Direct component.
            glm::vec3 new_direction;
            float pdf;
            glm::vec3 energy;
            glm::vec3 direct_lighting = DirectLights(current_ray, new_direction, pdf, current_intersection, energy);
            if (direct_lighting.x > 1.f) direct_lighting.x = 1.f;
            if (direct_lighting.y > 1.f) direct_lighting.y = 1.f;
            if (direct_lighting.z > 1.f) direct_lighting.z = 1.f;

            if (energy.x > 1.f) energy.x = 1.f;
            if (energy.y > 1.f) energy.y = 1.f;
            if (energy.z > 1.f) energy.z = 1.f;

            energy = direct_lighting;
            light_accum += multiplier * energy;

    //        glm::vec3 energy = intersection.object_hit->material->SampleAndEvaluateScatteredEnergy(
    //                    intersection, worldToObjectSpace(-current_ray.direction, current_intersection),
    //                    new_direction, pdf);

            if (fequal(pdf, 0.f) || (fequal(energy.x, 0.f) && fequal(energy.y, 0.f) && fequal(energy.z, 0.f))) {
                return light_accum;
            }

            // Ray may or may not be to light.
            glm::vec3 offset_point;// = current_intersection.point + (current_intersection.normal * OFFSET);
            glm::vec3 wo_L = worldToObjectSpace(-current_ray.direction, current_intersection);
            bool entering = wo_L.z > 0.0f;
            if(bounces == 0){
                offset_point = current_intersection.point + (current_intersection.normal) * OFFSET;
            }
            else{
                offset_point = current_intersection.point + (current_ray.direction) * OFFSET;
            }
            Ray bounced_ray(offset_point, new_direction);

            // Get intersection with bounced ray.
            Intersection bounce_intersection = intersection_engine->GetIntersection(bounced_ray);

            // Terminate if we hit empty space or a light.
            if (!bounce_intersection.object_hit
                    || bounce_intersection.object_hit->material->is_light_source) {
                return light_accum;
            }

            // Factor based on angle.
            float cosine_component = glm::abs(glm::dot(bounced_ray.direction, current_intersection.normal));

            // LTE term for this iteration;
            glm::vec3 lte_term = energy * cosine_component / pdf;

            // Terminate if russian roulette murders ray.
            if(throughput > 1.f) throughput = 1.f;
            if ((bounces > 2) && (throughput < (float(rand()) / float(RAND_MAX)))) {
    //        if (bounces > 5) {
                break;
            }

            throughput *= fmax(fmax(lte_term.x, lte_term.y), lte_term.z);
            multiplier *= lte_term;
            current_ray = bounced_ray;
            current_intersection = bounce_intersection;
            bounces++;
        }
        return light_accum;
}
