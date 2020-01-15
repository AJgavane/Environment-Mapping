#pragma once
#include <iomanip>
#include <glm\glm.hpp>
#include <glm\gtx\transform.hpp>

class Camera
{
public:
    Camera(const glm::vec3& pos, float fov, float width, float height, float zNear, float zFar, const glm::vec3& center, float bbox) {
        m_perspective = glm::perspective(fov, width / height, zNear, zFar);
        m_orthographic = glm::ortho(-bbox, bbox, -bbox, bbox, zNear, zFar);
        m_cameraPosition = pos;
        m_center = center;
        m_up = glm::vec3(0.0f, 1.0f, 0.0f);
    }

    inline glm::mat4 GetView() const
    {
        return glm::lookAt(m_cameraPosition, m_center, m_up);
    }

    inline glm::mat4 GetOrthoProj() const
    {
        return m_orthographic;
    }

    inline glm::mat4 GetPerspProj() const
    {
        return m_perspective;
    }

    inline glm::mat4 GetPersViewProj() const
    {
        return m_perspective * GetView();
    }

    inline void printMatrix(glm::mat4 M) {
        std::cout << std::setprecision(3) << std::fixed;
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                std::cout << M[i][j] << "\t";
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }

private:
    glm::mat4 m_perspective;
    glm::mat4 m_orthographic;
    glm::vec3 m_cameraPosition;
    glm::vec3 m_center;
    glm::vec3 m_up;
};

