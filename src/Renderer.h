#ifndef Renderer_h__
#define Renderer_h__

#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

#include "Camera.h"
#include "ShaderProgram.h"
#include "Model.h"
#include "Components/PointLight.h"
#include "Skybox.h"

class Renderer
{
public:
	
	glm::mat4 viewMatrix;
	glm::mat4 projectionMatrix;

	int HEIGHT, WIDTH;

	std::list<std::tuple<Model*, glm::mat4, bool, bool>> ModelsToRender;
	int Lights;
	std::vector<float> Light_position;
	std::vector<float> Light_specular;
	std::vector<float> Light_diffuse;
	std::vector<float> Light_constantAttenuation;
	std::vector<float> Light_linearAttenuation;
	std::vector<float> Light_quadraticAttenuation;
	std::vector<float> Light_spotExponent;
	std::list<std::tuple<glm::mat4, bool>> AABBsToRender;

	Renderer();

	void Initialize();
	void Draw(double dt);
	void DrawText();

	void AddModelToDraw(std::shared_ptr<Model> model, glm::vec3 position, glm::quat orientation, glm::vec3 scale, bool visible, bool shadowCaster);
	void AddTextToDraw();
	void AddPointLightToDraw(
		glm::vec3 _position,
		glm::vec3 _specular, 
		glm::vec3 _diffuse, 
		float _constantAttenuation, 
		float _linearAttenuation, 
		float _quadraticAttenuation, 
		float _spotExponent
		);
	void AddAABBToDraw(glm::vec3 origin, glm::vec3 volumeVector, bool colliding);

	void LoadContent();

	GLFWwindow* GetWindow() const { return m_Window; }
	std::shared_ptr<Camera> GetCamera() const { return m_Camera; }

	bool DrawNormals() const { return m_DrawNormals; }
	void DrawNormals(bool val) { m_DrawNormals = val; }
	bool DrawWireframe() const { return m_DrawWireframe; }
	void DrawWireframe(bool val) { m_DrawWireframe = val; }
	bool DrawBounds() const { return m_DrawBounds; }
	void DrawBounds(bool val) { m_DrawBounds = val; }
	void DrawSkybox();

	

private:
	GLFWwindow* m_Window;
	GLint m_glVersion[2];
	GLchar* m_glVendor;
	bool m_VSync;
	bool m_DrawNormals;
	bool m_DrawWireframe;
	bool m_DrawBounds;

	std::shared_ptr<Skybox> m_Skybox;

	int m_ShadowMapRes;
	glm::vec3 m_SunPosition;
	glm::vec3 m_SunTarget;
	glm::mat4 m_SunProjection;

	GLuint m_DebugAABB;
	GLuint m_ScreenQuad;
	GLuint m_ShadowFrameBuffer;
	GLuint m_ShadowDepthTexture;

	std::shared_ptr<Camera> m_Camera;

	ShaderProgram m_ShaderProgram;
	ShaderProgram m_ShaderProgramNormals;
	ShaderProgram m_ShaderProgramShadows;
	ShaderProgram m_ShaderProgramShadowsDrawDepth;
	ShaderProgram m_ShaderProgramDebugAABB;
	ShaderProgram m_ShaderProgramSkybox;
	
	void ClearStuff();
	void DrawScene();
	void DrawModels(ShaderProgram &shader);
	void DrawShadowMap();
	void CreateShadowMap(int resolution);
	GLuint CreateQuad();
	void DrawDebugShadowMap();
	GLuint CreateAABB();
	GLuint CreateSkybox(void);

};

#endif // Renderer_h__