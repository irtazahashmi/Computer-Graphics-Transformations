#pragma once
#include <framework/opengl_includes.h>
// Suppress warnings in third-party code.
#include <framework/disable_all_warnings.h>
DISABLE_WARNINGS_PUSH()
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
DISABLE_WARNINGS_POP()
#include <span>
#include <string>
#include <vector>



static glm::mat4 rotationMatrix(float angle, const glm::vec3& axis)
{
    return glm::rotate(glm::identity<glm::mat4>(), angle, axis);
}
static glm::mat4 translationMatrix(const glm::vec3& translation)
{
    return glm::translate(glm::identity<glm::mat4>(), translation);
}
static glm::mat4 scaleMatrix(const glm::vec3& scale)
{
    return glm::scale(glm::identity<glm::mat4>(), scale);
}

// !!! DO NOT MODIFY THIS STRUCT !!!
// Celestial body = planet / moon / star (including the sun)
struct CelestrialBody {
    // The radius of the sphere that represents the celestial body.
    float radius { 1.0f };
    // A celestial body may spin around it's own axis. The speed at which it rotates is
    //  expressed as the spin period. The spin period is the amount of (in hours) it
    // takes for the body to complete one full rotation.
    float spinPeriod { 0.0f };

    // A celestial body may orbit another celestial body (e.g. a moon orbiting a planet).
    // orbitAround stores the index of the object inside the bodies collection that this
    // celestial body is orbiting. The altitude describes the distance at which the object
    // is orbited (as measured from the center of both objects). The orbit period describes
    // how much time it takes for the celestial body to complete one rotation around the
    // object being orbited.
    //
    // If orbitAround is negative then this celestial body is not orbiting anything and it
    // should instead be placed at the origin (and orbitAltitude/orbitVelocity may be ignored).
    int orbitAround { -1 };
    float orbitAltitude { 0.0f };
    float orbitPeriod { 0.0f };

    // !!! YOU SHOULD IGNORE THE VARIABLES BELOW !!!
    // Only used for UI & drawing.
    std::string name;
    GLuint texture;
};

// ==========================
// =====    EXERCISE    =====
// ==========================
// Compute the positions/orientations of the solar system (list of celestial bodies) at the given time.
// Orbiting should not impact the spin of the body in any way. E.g. if the body had zero spin and was
// orbiting around another body then it should not rotate at all (only translate). If you implement
// orbit/spin correctly then the earth should always see the same side of the moon.
//
// Both spin and orbiting should be performed around the positive Y axis (glm::vec3(0, 1, 0)).
std::vector<glm::mat4> computeCelestrialBodyTransformations(std::span<const CelestrialBody> celestialBodies, float time)
{
    // For each celestial body, compute the matrix which scales the object (relative to a sphere at the origin with radius=1),
    //  translates and rotates it such that it respects the orbit / spin / scale stored in the input.
    std::vector<glm::mat4> transforms;

    // go through each celestial body
    for (const CelestrialBody& body : celestialBodies) {

        // identity matrix
        glm::mat4 identityMatrix = glm::identity<glm::mat4>();

        // initalizing matrices that will be used to do transformations, rotations etc.
        glm::mat4 scaleBodyToSizeMatrix = identityMatrix; //1
        glm::mat4 spinBodyMatrix = identityMatrix;//2
        glm::mat4 counterSpinBodyMatrix = identityMatrix;//3
        glm::mat4 orbitalTransformationMatrix = identityMatrix;//4
        glm::mat4 orbitalRotationMatrix = identityMatrix;//5
        glm::mat4 counterSpinMoonMatrix = identityMatrix;//6
        glm::mat4 translationRotationMoonMatrix = identityMatrix;//7
        glm::mat4 spinSunMatrix = identityMatrix;//8

        
        // 1 === Scale the body to its radius ===
        // use the vector {radius, radius, radius} to scale the body
        glm::vec3 scalingBodyVector = glm::vec3(body.radius);
        scaleBodyToSizeMatrix = scaleMatrix(scalingBodyVector);




        // 8 === if the body is SUN, rotate it using the positive y axis===
        if (body.orbitAround == -1) {
            float bodyPeriodOfSpin = (2.0f * glm::pi<float>()) / body.spinPeriod;
            float angleOfSpin = bodyPeriodOfSpin * time;
            // spinning the body on the positive y-axis with the angle of spin
            spinSunMatrix = rotationMatrix(angleOfSpin, glm::vec3{ 0, 1, 0 });



        } else { // if body is a PLANET or MOONe
      

            // if its a a MOON
            if (celestialBodies[body.orbitAround].orbitAround != -1) {

                // 6 === Counter Spin Moon with respect to its parent planet ===
                float orbitPeriodSpin = (2.0f * glm::pi<float>()) / celestialBodies[body.orbitAround].orbitPeriod;
                float orbitAngle = -(orbitPeriodSpin * time);
                // rotate it with respect to positive y-axis
                glm::vec3 rotationVector = { 0, 1, 0 };
                counterSpinMoonMatrix = rotationMatrix(orbitAngle, rotationVector);




                // 7 === Translate the moon and rotate the moon with respect to its parent planet ===
                // Applying the actual spin on the moon
                float orbitPeriodSpin2 = (2.0f * glm::pi<float>()) / celestialBodies[body.orbitAround].orbitPeriod;
                float orbitAngle2 = orbitPeriodSpin2 * time;
                // rotate it with respect to positive y-axis
                glm::vec3 rotationVector2 = { 0, 1, 0 };
                glm::mat4 rotationMatrix = glm::rotate(identityMatrix, orbitAngle2, rotationVector2);

                // translating the moon with respect to its parent planet using the positive x axis
                translationRotationMoonMatrix = glm::translate(rotationMatrix,
                    glm::vec3{ celestialBodies[body.orbitAround].orbitAltitude , 0, 0 });
            }



            // 2 ==== Implementing spinning of planets around themselves using positive y-axis as the axis of rotation ====
            // 2pi/spinperiod is body period of spin as it is a sphere -> given by assignment instructions
            float bodyPeriodOfSpin1 = (2.0f * glm::pi<float>()) / body.spinPeriod;
            float angleOfSpin1 = bodyPeriodOfSpin1 * time;
            // spinning the body on the positive y-axis with the angle of spin
            spinBodyMatrix = rotationMatrix(angleOfSpin1, glm::vec3{ 0, 1, 0 });
            


            // 3 === Implementing counter spinning for planets and moons ===
            float bodyPeriodOfSpin = (2.0f * glm::pi<float>()) / body.orbitPeriod;
            float angleOfSpin = -(bodyPeriodOfSpin * time);
            // rotate along the positive y axis
            counterSpinBodyMatrix = rotationMatrix(angleOfSpin, glm::vec3{ 0, 1, 0 });



            // 4 === Translate (move) the body to its orbital path ====
            // translating along the x-axis
            glm::vec3 orbitalTranslationVector = { body.orbitAltitude, 0, 0 };
            orbitalTransformationMatrix = translationMatrix(orbitalTranslationVector);
            


            // 5 === Orbit of the body ===
            float bodyPeriodOfOrbit = (2.0f * glm::pi<float>()) / body.orbitPeriod;
            float angleOfOrbit = bodyPeriodOfOrbit * time;
            // rotating the body along the positive y acis
            orbitalRotationMatrix = rotationMatrix(angleOfOrbit, glm::vec3{ 0, 1, 0 });

        }

       
        // multiply the matrices
        // matrix that will be used to apply transformations and rotations etc.
        glm::mat4 resultMatrix = identityMatrix;

        // multiply in the reverse order to get the desired result
        resultMatrix = resultMatrix * spinSunMatrix; //8
        resultMatrix = resultMatrix * translationRotationMoonMatrix; // 7
        resultMatrix = resultMatrix * counterSpinMoonMatrix; // 6
        resultMatrix = resultMatrix * orbitalRotationMatrix; // 5
        resultMatrix = resultMatrix * orbitalTransformationMatrix; // 4
        resultMatrix = resultMatrix * counterSpinBodyMatrix; // 3
        resultMatrix = resultMatrix * spinBodyMatrix; // 2
        resultMatrix = resultMatrix * scaleBodyToSizeMatrix; // 1

        transforms.push_back(resultMatrix);
    }
    return transforms;
}