#pragma once
// Suppress warnings in third-party code.
#include <framework/disable_all_warnings.h>
DISABLE_WARNINGS_PUSH()
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
DISABLE_WARNINGS_POP()
#include <array>
#include <framework/mesh.h>
#include <framework/opengl_includes.h>
#include <span>
#include <vector>

// ===============================
// =====   HELPER FUNCTIONS  =====
// ===============================
// glm::scale / glm::translate / glm::translate apply the matrices in reverse order.
// E.g. calling glm::rotate(glm::translate(inMatrix, ...), ...) will first rotate and then
// translate (reversed order in which the C++ functions are called). We provide these simple
// helpers methods that return a scale/translation/rotation matrix matrix so that you can
// decide on the multiplication order yourself.
// Example: rotationMatrix(...) * translationMatrix(...)
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

// global var
float x = 0;

void animate()
{
    x += 0.1f;
}

// =========================
// =====   EXERCISE 1  =====
// =========================
void drawTriangle()
{
    // A simple example of a drawing function for a triangle.
    // 1) Try changing its color to red - done
    // 2) Try changing its vertex positions - done
    // 3) Add a second triangle in blue - done
    // 4) Add a global variable (initialized at 0), which represents 
    // the x-coordinate of the first vertex of each triangle - done
    // 5) Go to the function animate and increment this variable by a 
    // small value - observe the animation. - done

    glColor3f(1, 0, 0);
    glNormal3f(0, 0, 1);
    glBegin(GL_TRIANGLES);
    glVertex3f(x, 0, 0);
    glVertex3f(1, 0, 0);
    glVertex3f(1, 1, 0);

    glColor3f(0, 0, 1);
    glNormal3f(0, 0, 1);
    glBegin(GL_TRIANGLES);
    glVertex3f(x, 0, 0);
    glVertex3f(0, 1, 0);
    glVertex3f(0, 0, 1);

    glEnd();
}

void drawUnitFace()
{
    // 1) Draw a unit quad in the x,y plane oriented along the z axis - done
    // 2) Make sure the orientation of the vertices is positive (counter-clock wise) - done
    // 3) What happens if the order is inversed? - done

    glColor3f(1, 1, 1);
    glNormal3f(0, 0, 1);
    glBegin(GL_QUADS);
    glVertex3f(0, 0, 0); // top left
    glVertex3f(1, 0, 0); // top right
    glVertex3f(1, -1, 0); //bottom right
    glVertex3f(0, -1, 0); // bottom left
    glEnd();
}

void drawUnitCube()
{
    // 1) Draw a unit cube using your function drawUnitFace, rely on glm::translate and 
    // glm::rotate to move the faces into position
    // and apply the transformation using "glLoadMatrix(glm::value_ptr(nameOfMatrixVariable));"
    // The cube should span from (0,0,0) to (1,1,1).

    drawUnitFace();

    {
        // Rotate by positive y-axis by 90 degrees
        glm::mat4 matrix = glm::identity<glm::mat4>();
        float angle = glm::radians(90.0f);
        glm::vec3 axis{ 0, 1, 0 };
        glm::mat4 rotation = glm::rotate(matrix, angle, axis);
        glLoadMatrixf(glm::value_ptr(rotation));
        drawUnitFace();
    }

    {
        // Rotate by positive y-axis by 90 degrees
        glm::mat4 matrix = glm::identity<glm::mat4>();
        float angle = glm::radians(90.0f);
        glm::vec3 axis{ 0, 1, 0 };
        glm::mat4 rotation = glm::rotate(matrix, angle, axis);
        glm::mat4 translation = glm::translate(rotation, glm::vec3(0, 1, 0));
        axis = { -1, 0, 0 };
        glm::mat4 rotation2 = glm::rotate(translation, angle, axis);
        glm::mat4 translation2 = glm::translate(rotation2, glm::vec3(0, 0, -1));
        glLoadMatrixf(glm::value_ptr(translation2));
        drawUnitFace();
    }
}

struct ArmSegment {
    // Rotation around the positive x axis (glm::vec3(1,0,0)) in radians.
    float rotationX { 0.0f };
    glm::vec3 boxSize;
};

void drawArm(const ArmSegment& segment1, const ArmSegment& segment2, const ArmSegment& segment3)
{
    // Produce a three-unit arm (upper arm, forearm, hand) making use of your function drawUnitCube to define each of them.
    //1) Use the specified segments to define your arm; use glm::scale(matrix, scale) to achieve different arm length and
    //    use glm::rotate(matrix, angleInRadians, axis) and glm::translate(matrix, translation) to correctly place the elements.
    //   Modify the drawUnitCube function to take a glm::mat4 as an argument which is applied to the cube.

    glm::mat4 matrix = glm::identity<glm::mat4>(); // Creates an identity matrix.
    glLoadMatrixf(glm::value_ptr(matrix)); // Anything drawn after this line will be transformed by the given matrix.
    drawUnitCube();

}

void drawMesh(Mesh mesh)
{
    //1) Use the mesh data structure; each triangle is defined with 3 consecutive indices in the mesh.triangles vector.
    //   These indices correspond to vertices stored in mesh.vertices. Provide a function that draws these triangles.
    //   Each vertex stores a position and some additional data. For this exercise you only need to look at the position.
    //
    // The exact definition of Mesh can be found in "framework/include/framework/mesh.h":
    //
    // struct Vertex {
    //     glm::vec3 position;
    //     glm::vec3 normal; // [initially zero]
    //     ...
    // };
    //
    // struct Mesh {
    //     std::vector<Vertex> vertices;
    //     std::vector<glm::uvec3> triangles;
    // };

    //2) Compute the normals of these triangles

    //3) Try computing a normal per vertex as the average of the adjacent face normals, call glNormal3f with the corresponding
    //   values before each vertex. What do you observe with respect to the lighting?

    //4) Try loading your own model (export it from Blender as a Wavefront obj) and replace the provided mesh file.

}