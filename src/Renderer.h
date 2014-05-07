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
#include "ResourceManager.h"

class Renderer
{
public:

	glm::mat4 viewMatrix;
	glm::mat4 projectionMatrix;

	int HEIGHT, WIDTH;

	std::list<std::tuple<Model*, glm::mat4, bool, bool>> ModelsToRender;
	int Lights;
	std::vector<glm::vec3> Light_position;
	std::vector<glm::vec3> Light_specular;
	std::vector<glm::vec3> Light_diffuse;
	std::vector<float> Light_specularExponent;


 	std::vector<float> Light_constantAttenuation;
 	std::vector<float> Light_linearAttenuation;
 	std::vector<float> Light_quadraticAttenuation;
	std::vector<float> Light_spotExponent;

	std::list<std::tuple<glm::mat4, bool>> AABBsToRender;

	Renderer();

	void Initialize();
	void Draw(double dt);
	void DrawText();

	void AddModelToDraw(Model* model, glm::vec3 position, glm::quat orientation, glm::vec3 scale, bool visible, bool shadowCaster);
	void AddTextToDraw();
	void AddPointLightToDraw(
	    glm::vec3 _position,
	    glm::vec3 _specular,
	    glm::vec3 _diffuse,
	    float _specularExponent
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

	void SetSphereModel(Model* _model);



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
	GLuint m_ShadowFrameBuffer;
	GLuint m_ShadowDepthTexture;

	GLuint m_fbBasePass;
	GLuint m_fDiffuseTexture;
	GLuint m_fPositionTexture;
	GLuint m_fNormalsTexture;
	GLuint m_fBlendTexture;
	GLuint m_fbLightingPass;
	GLuint m_fLightingTexture;

	GLuint m_fDepthBuffer;
	GLenum draw_bufs[2];
	glm::mat4 lM[5];
	GLuint m_ScreenQuad;
	Model* m_sphereModel;

	bool m_QuadView;

	std::shared_ptr<Camera> m_Camera;

	ShaderProgram m_ShaderProgram;
	ShaderProgram m_FirstPassProgram;
	ShaderProgram m_SecondPassProgram;
	ShaderProgram m_SecondPassProgram_Debug;
	ShaderProgram m_FinalPassProgram;

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
	void FrameBufferTextures();
	void DrawFBO();
	void DrawFBOScene();
	void DrawLightScene();
	void BindFragDataLocation();
	void CreateLightMatrix();
	
	GLuint CreateQuad();
	void DrawDebugShadowMap();
	GLuint CreateAABB();
	GLuint CreateSkybox(void);

};

#endif // Renderer_h__