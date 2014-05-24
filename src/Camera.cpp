#include "PrecompiledHeader.h"
#include "Camera.h"

Camera::Camera(float yFOV, float aspectRatio, float nearClip, float farClip)
{
	m_FOV = yFOV;
	m_AspectRatio = aspectRatio;
	m_NearClip = nearClip;
	m_FarClip = farClip;

	m_Position = glm::vec3(0.0);

	UpdateViewMatrix();
}

//glm::vec3 Camera::Forward()
//{
//	return glm::rotate(glm::vec3(0.f, 0.f, -1.f), -m_Yaw, glm::vec3(0.f, 1.f, 0.f));
//}
//
//glm::vec3 Camera::Right()
//{
//	return glm::rotate(glm::vec3(1.f, 0.f, 0.f), -m_Yaw, glm::vec3(0.f, 1.f, 0.f));
//}

//glm::mat4 Camera::Orientation()
//{
//	glm::mat4 orientation(1.f);
//	orientation = glm::rotate(orientation, m_Pitch, glm::vec3(1.f, 0.f, 0.f));
//	orientation = glm::rotate(orientation, m_Yaw, glm::vec3(0.f, 1.f, 0.f));
//	return orientation;
//}

void Camera::Position(glm::vec3 val)
{
	m_Position = val;
	UpdateViewMatrix();
}


void Camera::Orientation(glm::quat val)
{
	m_Orientation = val;
	UpdateViewMatrix();
}

//void Camera::Pitch(float val)
//{
//	m_Pitch = val;
//	UpdateViewMatrix();
//}
//
//void Camera::Yaw(float val)
//{
//	m_Yaw = val;
//	UpdateViewMatrix();
//}

void Camera::UpdateViewMatrix()
{
	m_ViewMatrix = glm::toMat4(glm::inverse(m_Orientation)) * glm::translate(-m_Position);
}

void Camera::FOV(float val)
{
	m_FOV = val;
}

void Camera::NearClip(float val)
{
	m_NearClip = val;
}

void Camera::FarClip(float val)
{
	m_FarClip = val;
}

glm::mat4 Camera::ProjectionMatrix(float aspectRatio)
{
	return glm::perspective(
		m_FOV,
		aspectRatio,
		m_NearClip,
		m_FarClip
		);
}
