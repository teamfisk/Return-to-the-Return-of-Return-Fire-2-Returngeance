#ifndef Camera_h__
#define Camera_h__

//#include "PrecompiledHeader.h"

class Camera
{
public:
	Camera(float yFOV, float aspectRatio, float nearClip, float farClip);

	glm::vec3 Forward();
	glm::vec3 Right();

	float AspectRatio() const { return m_AspectRatio; }
	void AspectRatio(float val);

	glm::vec3 Position() const { return m_Position; }
	void Position(glm::vec3 val);

	glm::quat Orientation() const { return m_Orientation; }
	void Orientation(glm::quat val);

	/*float Pitch() const { return m_Pitch; }
	void Pitch(float val);
	float Yaw() const { return m_Yaw; }
	void Yaw(float val);*/

	glm::mat4 ProjectionMatrix() const { return m_ProjectionMatrix; }
	void ProjectionMatrix(glm::mat4 val) { m_ProjectionMatrix = val; }

	glm::mat4 ViewMatrix() const { return m_ViewMatrix; }
	void ViewMatrix(glm::mat4 val) { m_ViewMatrix = val; }

	float FOV() const { return m_FOV; }
	void FOV(float val);

	float NearClip() const { return m_NearClip; }
	void NearClip(float val);

	float FarClip() const { return m_FarClip; }
	void FarClip(float val);

private:
	void UpdateProjectionMatrix();
	void UpdateViewMatrix();

	float m_FOV;
	float m_AspectRatio;
	float m_NearClip;
	float m_FarClip;

	glm::vec3 m_Position;
	glm::quat m_Orientation;
	//float m_Pitch;
	//float m_Yaw;

	glm::mat4 m_ProjectionMatrix;
	glm::mat4 m_ViewMatrix;
};

#endif // Camera_h__
