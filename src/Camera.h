#ifndef Camera_h__
#define Camera_h__

class Camera
{
public:
	Camera(float yFOV, float nearClip, float farClip);

	glm::vec3 Forward();
	glm::vec3 Right();

	glm::vec3 Position() const { return m_Position; }
	void SetPosition(glm::vec3 val);

	glm::quat Orientation() const { return m_Orientation; }
	void SetOrientation(glm::quat val);

	/*float Pitch() const { return m_Pitch; }
	void Pitch(float val);
	float Yaw() const { return m_Yaw; }
	void Yaw(float val);*/

	glm::mat4 ProjectionMatrix(float aspectRatio);

	glm::mat4 ViewMatrix() const { return m_ViewMatrix; }

	float FOV() const { return m_FOV; }
	void SetFOV(float val);

	float NearClip() const { return m_NearClip; }
	void SetNearClip(float val);

	float FarClip() const { return m_FarClip; }
	void SetFarClip(float val);

private:
	void UpdateViewMatrix();

	float m_FOV;
	float m_NearClip;
	float m_FarClip;

	glm::vec3 m_Position;
	glm::quat m_Orientation;

	glm::mat4 m_ViewMatrix;
};

#endif // Camera_h__
