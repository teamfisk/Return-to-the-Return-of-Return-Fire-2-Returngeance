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
#include "Util/Rectangle.h"
#include "RenderQueue.h"

class Renderer
{
public:

	glm::mat4 viewMatrix;
	glm::mat4 projectionMatrix;

	
	int Width() const { return m_Width; }
	int Height() const { return m_Height; }

	std::list<std::tuple<Model*, glm::mat4, bool, bool>> ModelsToRender;
	std::list<std::tuple<Texture*, glm::mat4, glm::vec3>> TexturesToRender;
	std::list<std::tuple<glm::mat4, bool>> AABBsToRender;

	Renderer(std::shared_ptr<::ResourceManager> resourceManager);

	void Initialize();
	void Draw(double dt);
	void DrawText();

	void RegisterViewport(int identifier, float left, float top, float right, float bottom);
	void RegisterCamera(int identifier, float FOV, float nearClip, float farClip);
	void UpdateViewport(int viewportIdentifier, int cameraIdentifier);
	void UpdateCamera(int cameraIdentifier, glm::vec3 position, glm::quat orientation, float FOV, float nearClip, float farClip);

#pragma region NEWSTUFF
	void SetResolution(const Rectangle &resolution)
	{
		m_Width = resolution.Width;
		m_Height = resolution.Height;
	}

	void SetFullscreen(bool fullscreen)
	{
		m_Fullscreen = fullscreen;
	}

	void SetViewport(const Rectangle &viewport)
	{
		m_Viewport = viewport;
	}

	void SetScissor(const Rectangle &scissor)
	{
		m_Scissor = scissor;
	}

	void SetCamera(std::shared_ptr<Camera> camera)
	{
		m_Camera = camera;
	}

	void DrawFrame(RenderQueuePair &rq);
	void DrawWorld(RenderQueuePair &rq);
	void Swap();

#pragma endregion

	void AddModelToDraw(Model* model, glm::vec3 position, glm::quat orientation, glm::vec3 scale, bool visible, bool shadowCaster);
	void AddTextureToDraw(Texture* texture, glm::vec3 position, glm::quat orientation, glm::vec3 scale);
	void AddTextToDraw();
	void AddPointLightToDraw(
	    glm::vec3 _position,
	    glm::vec3 _specular,
	    glm::vec3 _diffuse,
	    float _specularExponent,
		float _ConstantAttenuation, 
		float _LinearAttenuation, 
		float _QuadraticAttenuation,
		float _radius
	);
	void ClearPointLights();

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
	std::shared_ptr<::ResourceManager> ResourceManager;

	bool m_Fullscreen;
	int m_Width, m_Height;

	struct Viewport
	{
		float Left;
		float Top;
		float Right;
		float Bottom;
		std::shared_ptr<Camera> Camera;
	};

	std::unordered_map<int, Viewport> m_Viewports;
	std::unordered_map<int, std::shared_ptr<Camera>> m_Cameras;

	Rectangle m_Viewport;
	Rectangle m_Scissor;
	std::shared_ptr<Camera> m_Camera;

	struct Light
	{
		glm::vec3 Position;
		glm::vec3 Specular;
		glm::vec3 Diffuse;
		float SpecularExponent;
		glm::mat4 SphereModelMatrix;
		float ConstantAttenuation, LinearAttenuation, QuadraticAttenuation, Radius;
	};

	float Gamma;

	std::list<Light> Lights;

	GLFWwindow* m_Window;
	GLint m_glVersion[2];
	GLchar* m_glVendor;
	bool m_VSync;
	bool m_DrawNormals;
	bool m_DrawWireframe;
	bool m_DrawBounds;
	float CAtt, LAtt, QAtt;

	std::shared_ptr<Skybox> m_Skybox;

	int m_ShadowMapRes;
	glm::vec3 m_SunPosition;
	glm::vec3 m_SunTarget;
	glm::mat4 m_SunProjection;
	glm::vec2 m_SunProjection_width;
	glm::vec2 m_SunProjection_height;
	glm::vec2 m_SunProjection_length;


	GLuint m_DebugAABB;
	GLuint m_ShadowFrameBuffer;
	GLuint m_ShadowDepthTexture;

	GLuint m_fbBasePass;
	GLuint m_fDiffuseTexture;
	GLuint m_fPositionTexture;
	GLuint m_fNormalsTexture;
	GLuint m_fSpecularTexture;
	GLuint m_fBlendTexture;
	GLuint m_fbLightingPass;
	GLuint m_fLightingTexture;
	GLuint m_fShadowTexture;

	GLuint m_fDepthBuffer;
	GLenum draw_bufs[2];
	GLuint m_ScreenQuad;
	Model* m_sphereModel;

	bool m_QuadView;

	ShaderProgram m_ShaderProgram;
	ShaderProgram m_FirstPassProgram;
	ShaderProgram m_SecondPassProgram;
	ShaderProgram m_SecondPassProgram_Debug;
	ShaderProgram m_FinalPassProgram;
	ShaderProgram m_SunPassProgram;
	ShaderProgram m_ForwardRendering;
	ShaderProgram m_BlendMapProgram;
	ShaderProgram m_FinalForwardPassProgram;

	ShaderProgram m_ShaderProgramNormals;
	ShaderProgram m_ShaderProgramShadows;
	ShaderProgram m_ShaderProgramShadowsDrawDepth;
	ShaderProgram m_ShaderProgramDebugAABB;
	ShaderProgram m_ShaderProgramSkybox;



	void ClearStuff();
	void DrawScene();
	void DrawModels(ShaderProgram &shader);
	void DrawShadowMap(RenderQueue &rq);
	void CreateShadowMap(int resolution);
	void FrameBufferTextures();
	void DrawFBO();
	void DrawFBO2(RenderQueue &rq);
	void DrawFBOScene(RenderQueue &rq);
	void DrawLightScene(RenderQueue &rq);
	void DrawSunLightScene();
	void BindFragDataLocation();
	glm::mat4 CreateLightMatrix(Light &_light);
	void UpdateSunProjection();
	void CreateNormalMapTangent();
	void ForwardRendering(RenderQueue &rq);

	static bool DepthSort(const std::shared_ptr<RenderJob> &i, const std::shared_ptr<RenderJob> &j) { return (i->Depth > j->Depth); }

	GLuint CreateQuad();
	void DrawDebugShadowMap();
	GLuint CreateAABB();
	GLuint CreateSkybox(void);

};

#endif // Renderer_h__