#include "config.h"
#include "camera.h"


namespace Display
{

    Camera* Camera::instance = 0;

    Camera::Camera()
    {
        m_width = 1024;
        m_height = 768;

        projection = projection.setPerspective(45.0f, (m_width / m_height), 1.0f, 100.0f);

        position.setValues(0, 0, 2);
        origin.setValues(0, 0, 0);
        headUp.setValues(0, 1, 0);
        camFront.setValues(0.0f, 0.0f, 1.0f);
        yaw = -90.0f;    // Yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right (due to how Eular angles work) so we initially rotate a bit to the left.
        pitch = 0.0f;

        oldPosX = m_width / 2.0f;
        oldPosY = m_height / 2.0f;
    }


    Camera* Camera::GetInstance()
    {
        if (instance == 0)
        {
            instance = new Camera();
        }
        return instance;
    }


    void Camera::UpdatePerspective(float w, float h)
    {
        m_width = w;
        m_height = h;
        projection = projection.setPerspective(45.0f, (m_width / m_height), 1.0f, 100.0f);
    }
}