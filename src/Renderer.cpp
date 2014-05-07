#include "PrecompiledHeader.h"
#include "Renderer.h"

Renderer::Renderer()
{
	m_VSync = false;
#ifdef DEBUG
	m_DrawNormals = false;
	m_DrawWireframe = false;
	m_DrawBounds = true;
#else
	m_DrawNormals = false;
	m_DrawWireframe = false;
	m_DrawBounds = false;
#endif

	m_ShadowMapRes = 2048;
	m_SunPosition = glm::vec3(0, 3.5f, 10);
	m_SunTarget = glm::vec3(0, 0, 0);
	m_SunProjection = glm::ortho<float>(-100, 100, -100, 100, -100, 100);
	Lights = 0;
}

void Renderer::Initialize()
{
	// Initialize GLFW
	if (!glfwInit())
	{
		LOG_ERROR("GLFW: Initialization failed");
		exit(EXIT_FAILURE);
	}

	// Create a window
	WIDTH = 1280;
	HEIGHT = 720;
	// Antialiasing
	//glfwWindowHint(GLFW_SAMPLES, 16);
	m_Window = glfwCreateWindow(WIDTH, HEIGHT, "OpenGL", nullptr, nullptr);
	if (!m_Window)
	{
		LOG_ERROR("GLFW: Failed to create window");
		exit(EXIT_FAILURE);
	}
	glfwMakeContextCurrent(m_Window);

	// GL version info
	glGetIntegerv(GL_MAJOR_VERSION, &m_glVersion[0]);
	glGetIntegerv(GL_MINOR_VERSION, &m_glVersion[1]);
	m_glVendor = (GLchar*)glGetString(GL_VENDOR);
	std::stringstream ss;
	ss << m_glVendor << " OpenGL " << m_glVersion[0] << "." << m_glVersion[1];
#ifdef DEBUG
	ss << " DEBUG";
#endif
	LOG_INFO(ss.str().c_str());
	glfwSetWindowTitle(m_Window, ss.str().c_str());

	// Initialize GLEW
	if (glewInit() != GLEW_OK)
	{
		LOG_ERROR("GLEW: Initialization failed");
		exit(EXIT_FAILURE);
	}

	// Create Camera
	m_Camera = std::make_shared<Camera>(45.f, (float)WIDTH / HEIGHT, 0.01f, 1000.f);
	m_Camera->Position(glm::vec3(0.0f, 0.0f, 2.f));

	glfwSwapInterval(m_VSync);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glEnable(GL_DEPTH_TEST);

	LoadContent();
}

void Renderer::LoadContent()
{
	auto standardVS = std::shared_ptr<Shader>(new VertexShader("Shaders/Vertex.glsl"));
	auto standardFS = std::shared_ptr<Shader>(new FragmentShader("Shaders/Fragment.glsl"));

	m_ShaderProgram.AddShader(standardVS);
	m_ShaderProgram.AddShader(standardFS);
	m_ShaderProgram.Compile();
	m_ShaderProgram.Link();

	m_ShaderProgramNormals.AddShader(std::shared_ptr<Shader>(new GeometryShader("Shaders/Normals.geo.glsl")));
	m_ShaderProgramNormals.AddShader(standardVS);
	m_ShaderProgramNormals.AddShader(std::shared_ptr<Shader>(new FragmentShader("Shaders/Normals.frag.glsl")));
	m_ShaderProgramNormals.Compile();
	m_ShaderProgramNormals.Link();

	m_ShaderProgramShadows.AddShader(std::shared_ptr<Shader>(new VertexShader("Shaders/ShadowMap.vert.glsl")));
	m_ShaderProgramShadows.AddShader(std::shared_ptr<Shader>(new FragmentShader("Shaders/ShadowMap.frag.glsl")));
	m_ShaderProgramShadows.Compile();
	m_ShaderProgramShadows.Link();

	m_ShaderProgramShadowsDrawDepth.AddShader(std::shared_ptr<Shader>(new VertexShader("Shaders/VisualizeDepth.vert.glsl")));
	m_ShaderProgramShadowsDrawDepth.AddShader(std::shared_ptr<Shader>(new FragmentShader("Shaders/VisualizeDepth.frag.glsl")));
	m_ShaderProgramShadowsDrawDepth.Compile();
	m_ShaderProgramShadowsDrawDepth.Link();

	m_ShaderProgramDebugAABB.AddShader(standardVS);
	m_ShaderProgramDebugAABB.AddShader(std::shared_ptr<Shader>(new FragmentShader("Shaders/AABB.frag.glsl")));
	m_ShaderProgramDebugAABB.Compile();
	m_ShaderProgramDebugAABB.Link();

	m_ShaderProgramSkybox.AddShader(std::shared_ptr<Shader>(new VertexShader("Shaders/Skybox.vert.glsl")));
	m_ShaderProgramSkybox.AddShader(std::shared_ptr<Shader>(new FragmentShader("Shaders/Skybox.frag.glsl")));
	m_ShaderProgramSkybox.Compile();
	m_ShaderProgramSkybox.Link();

	m_Skybox = std::make_shared<Skybox>("Textures/Skybox/Sunset", "jpg");

	m_DebugAABB = CreateAABB();
	m_ScreenQuad = CreateQuad();
	CreateShadowMap(m_ShadowMapRes);
}

void Renderer::CreateShadowMap(int resolution)
{
	glGenFramebuffers(1, &m_ShadowFrameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, m_ShadowFrameBuffer);

	// Depth texture
	glGenTextures(1, &m_ShadowDepthTexture);
	glBindTexture(GL_TEXTURE_2D, m_ShadowDepthTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, resolution, resolution, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

	//glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_NONE );
	//glTexParameteri( GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE, GL_INTENSITY );

	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, m_ShadowDepthTexture, 0);
	glDrawBuffer(GL_NONE);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		LOG_ERROR("Framebuffer incomplete!");
		return;
	}
}
void Renderer::Draw(double dt)
{
	glDisable(GL_BLEND);

	DrawSkybox();
	DrawShadowMap();
	DrawScene();

#ifdef DEBUG
	// Draw bounding boxes
	if (m_DrawBounds)
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_ONE_MINUS_DST_COLOR, GL_ZERO);
		m_ShaderProgramDebugAABB.Bind();
		for (auto tuple : AABBsToRender)
		{
			glm::mat4 modelMatrix;
			bool colliding;
			std::tie(modelMatrix, colliding) = tuple;
			// Model matrix
			glm::mat4 cameraMatrix = m_Camera->ProjectionMatrix() * m_Camera->ViewMatrix();
			glm::mat4 MVP = cameraMatrix * modelMatrix;
			glUniformMatrix4fv(glGetUniformLocation(m_ShaderProgramDebugAABB.GetHandle(), "MVP"), 1, GL_FALSE, glm::value_ptr(MVP));
			// Color
			glm::vec4 color(1.f, 1.f, 1.f, 0.f);
			if (colliding)
				color = glm::vec4(1.f, 0.f, 0.f, 0.f);
			glUniform4fv(glGetUniformLocation(m_ShaderProgramDebugAABB.GetHandle(), "Color"), 1, glm::value_ptr(color));
			glBindVertexArray(m_DebugAABB);
			glDrawArrays(GL_LINES, 0, 24);
		}
	}

	DrawDebugShadowMap();
#endif

	ClearStuff();
	glfwSwapBuffers(m_Window);
}

void Renderer::DrawSkybox()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, WIDTH, HEIGHT);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	m_ShaderProgramSkybox.Bind();
	glm::mat4 cameraMatrix = m_Camera->ProjectionMatrix() * glm::toMat4(m_Camera->Orientation());
	glUniformMatrix4fv(glGetUniformLocation(m_ShaderProgramSkybox.GetHandle(), "MVP"), 1, GL_FALSE, glm::value_ptr(cameraMatrix));
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	m_Skybox->Draw();
}

void Renderer::DrawScene()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, WIDTH, HEIGHT);

	glClear(GL_DEPTH_BUFFER_BIT);
	//glClearColor(1.0f, 1.0f, 0.0f, 1.0f);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
#ifdef DEBUG
	glDisable(GL_CULL_FACE);
	glPolygonMode(GL_BACK, GL_LINE);
#endif

	// Draw models
	glm::mat4 depthViewMatrix = glm::lookAt(m_SunPosition, m_SunTarget, glm::vec3(0, 1, 0)) * glm::translate(-m_Camera->Position() * glm::vec3(1, 0, 0));
	glm::mat4 depthCamera = m_SunProjection * depthViewMatrix;
	glm::mat4 biasMatrix(
	    0.5, 0.0, 0.0, 0.0,
	    0.0, 0.5, 0.0, 0.0,
	    0.0, 0.0, 0.5, 0.0,
	    0.5, 0.5, 0.5, 1.0
	);

	m_ShaderProgram.Bind();
	glUniform1i(glGetUniformLocation(m_ShaderProgram.GetHandle(), "numberOfLights"), Lights);
	glUniform3fv(glGetUniformLocation(m_ShaderProgram.GetHandle(), "position"), Lights, Light_position.data());
	glUniform3fv(glGetUniformLocation(m_ShaderProgram.GetHandle(), "specular"), Lights, Light_specular.data());
	glUniform3fv(glGetUniformLocation(m_ShaderProgram.GetHandle(), "diffuse"), Lights, Light_diffuse.data());
	glUniform1fv(glGetUniformLocation(m_ShaderProgram.GetHandle(), "constantAttenuation"), Lights, Light_constantAttenuation.data());
	glUniform1fv(glGetUniformLocation(m_ShaderProgram.GetHandle(), "linearAttenuation"), Lights, Light_linearAttenuation.data());
	glUniform1fv(glGetUniformLocation(m_ShaderProgram.GetHandle(), "quadraticAttenuation"), Lights, Light_quadraticAttenuation.data());
	glUniform1fv(glGetUniformLocation(m_ShaderProgram.GetHandle(), "spotExponent"), Lights, Light_spotExponent.data());
	if (m_DrawWireframe)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_ShadowDepthTexture);
	//DrawModels(m_ShaderProgram);
	glm::mat4 cameraMatrix = m_Camera->ProjectionMatrix() * m_Camera->ViewMatrix();
	glm::mat4 depthCameraMatrix = biasMatrix * depthCamera;
	glm::mat4 MVP;
	glm::mat4 depthMVP;
	for (auto tuple : ModelsToRender)
	{
		Model* model;
		glm::mat4 modelMatrix;
		bool visible;
		std::tie(model, modelMatrix, visible, std::ignore) = tuple;
		if (!visible)
			continue;

		MVP = cameraMatrix * modelMatrix;
		depthMVP = depthCameraMatrix * modelMatrix;
		glUniformMatrix4fv(glGetUniformLocation(m_ShaderProgram.GetHandle(), "MVP"), 1, GL_FALSE, glm::value_ptr(MVP));
		glUniformMatrix4fv(glGetUniformLocation(m_ShaderProgram.GetHandle(), "DepthMVP"), 1, GL_FALSE, glm::value_ptr(depthMVP));
		glUniformMatrix4fv(glGetUniformLocation(m_ShaderProgram.GetHandle(), "model"), 1, GL_FALSE, glm::value_ptr(modelMatrix));
		glUniformMatrix4fv(glGetUniformLocation(m_ShaderProgram.GetHandle(), "view"), 1, GL_FALSE, glm::value_ptr(m_Camera->ViewMatrix()));
		glBindVertexArray(model->VAO);
		for (auto texGroup : model->TextureGroups)
		{
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, *texGroup.Texture);
			glDrawArrays(GL_TRIANGLES, texGroup.StartIndex, texGroup.EndIndex - texGroup.StartIndex + 1);
		}
	}

	for (auto tuple : TexturesToRender)
	{
		Texture* texture;
		glm::mat4 modelMatrix;
		glm::mat4 billboardMatrix;
		std::tie(texture, modelMatrix, billboardMatrix) = tuple;
		
		
		//MVP = cameraMatrix * glm::inverse(glm::toMat4(m_Camera->Orientation()) * modelMatrix );
		MVP = cameraMatrix * billboardMatrix * modelMatrix;
		
		depthMVP = depthCameraMatrix * modelMatrix;
		glUniformMatrix4fv(glGetUniformLocation(m_ShaderProgram.GetHandle(), "MVP"), 1, GL_FALSE, glm::value_ptr(MVP));
		glUniformMatrix4fv(glGetUniformLocation(m_ShaderProgram.GetHandle(), "DepthMVP"), 1, GL_FALSE, glm::value_ptr(depthMVP));
		glUniformMatrix4fv(glGetUniformLocation(m_ShaderProgram.GetHandle(), "model"), 1, GL_FALSE, glm::value_ptr(modelMatrix));
		glUniformMatrix4fv(glGetUniformLocation(m_ShaderProgram.GetHandle(), "view"), 1, GL_FALSE, glm::value_ptr(m_Camera->ViewMatrix()));
		
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, *texture);
		glBindVertexArray(m_ScreenQuad);
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}

	


#ifdef DEBUG
	// Debug draw model normals
	if (m_DrawNormals)
	{
		m_ShaderProgramNormals.Bind();
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		DrawModels(m_ShaderProgramNormals);
	}
#endif
}

void Renderer::DrawShadowMap()
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);

	glBindFramebuffer(GL_FRAMEBUFFER, m_ShadowFrameBuffer);
	glViewport(0, 0, m_ShadowMapRes, m_ShadowMapRes);

	glClear(GL_DEPTH_BUFFER_BIT);
	//glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	glm::mat4 depthViewMatrix = glm::lookAt(m_SunPosition, m_SunTarget, glm::vec3(0, 1, 0)) * glm::translate(-m_Camera->Position() * glm::vec3(1, 0, 0));
//	glm::mat4 depthViewMatrix = glm::lookAt(m_SunPosition, m_SunTarget, glm::vec3(0, 1, 0));
	glm::mat4 depthCamera = m_SunProjection * depthViewMatrix;

	//glm::mat4 cameraMatrix = depthProjectionMatrix * m_Camera->ViewMatrix();

	glm::mat4 MVP;

	m_ShaderProgramShadows.Bind();
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	for (auto tuple : ModelsToRender)
	{
		Model* model;
		glm::mat4 modelMatrix;
		bool shadow;
		std::tie(model, modelMatrix, std::ignore, shadow) = tuple;
		if (!shadow)
			continue;

		MVP = depthCamera * modelMatrix;
		glUniformMatrix4fv(glGetUniformLocation(m_ShaderProgramShadows.GetHandle(), "MVP"), 1, GL_FALSE, glm::value_ptr(MVP));

		glBindVertexArray(model->VAO);
		for (auto texGroup : model->TextureGroups)
		{
			glDrawArrays(GL_TRIANGLES, texGroup.StartIndex, texGroup.EndIndex - texGroup.StartIndex + 1);
		}
	}


}

void Renderer::DrawDebugShadowMap()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, 400, 400);

	glClear(GL_DEPTH_BUFFER_BIT);
	//glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	m_ShaderProgramShadowsDrawDepth.Bind();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_ShadowDepthTexture);
	glBindVertexArray(m_ScreenQuad);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void Renderer::DrawModels(ShaderProgram &shader)
{
	/*glm::mat4 cameraMatrix = m_Camera->ProjectionMatrix() * m_Camera->ViewMatrix();

	glm::mat4 MVP;
	for (auto tuple : ModelsToRender)
	{
	Model* model;
	glm::mat4 modelMatrix;
	std::tie(model, modelMatrix) = tuple;

	MVP = cameraMatrix * modelMatrix;
	glUniformMatrix4fv(glGetUniformLocation(shader.GetHandle(), "MVP"), 1, GL_FALSE, glm::value_ptr(MVP));
	glUniformMatrix4fv(glGetUniformLocation(shader.GetHandle(), "model"), 1, GL_FALSE, glm::value_ptr(modelMatrix));
	glUniformMatrix4fv(glGetUniformLocation(shader.GetHandle(), "view"), 1, GL_FALSE, glm::value_ptr(m_Camera->ViewMatrix()));
	glBindVertexArray(model->VAO);
	for (auto texGroup : model->TextureGroups) {
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texGroup.Texture->texture);
	glDrawArrays(GL_TRIANGLES, texGroup.StartIndex, texGroup.EndIndex - texGroup.StartIndex + 1);
	}
	}*/
}

void Renderer::DrawText()
{
	//DrawShitInTextForm
}

void Renderer::AddTextToDraw()
{
	//Add to draw shit vector
}

void Renderer::AddModelToDraw(Model* model, glm::vec3 position, glm::quat orientation, glm::vec3 scale, bool visible, bool shadowCaster)
{
	glm::mat4 modelMatrix = glm::translate(glm::mat4(), position) * glm::toMat4(orientation) * glm::scale(scale);
	// You can now use ModelMatrix to build the MVP matrix
	ModelsToRender.push_back(std::make_tuple(model, modelMatrix, visible, shadowCaster));
}

void Renderer::AddTextureToDraw(Texture* texture, glm::vec3 position, glm::quat orientation, glm::vec3 scale)
{
	glm::mat4 modelMatrix = glm::translate(glm::mat4(), position) * glm::toMat4(orientation) * glm::scale(scale);

	glm::vec3 camToParticle = glm::normalize(m_Camera->Position() - position);
	glm::vec3 up = glm::vec3(0,1,0);
	glm::vec3 rightVec = glm::cross(up, camToParticle);
	
	glm::mat4 billboardMatrix;
	billboardMatrix[0] = glm::vec4(rightVec, 0);
	billboardMatrix[1] = glm::vec4(up, 0);
	billboardMatrix[2] = glm::vec4(camToParticle, 0);
	//billboardMatrix[3] = glm::vec4(position, 0);

	TexturesToRender.push_back(std::make_tuple(texture, modelMatrix, billboardMatrix));
}

void Renderer::AddPointLightToDraw(
    glm::vec3 _position,
    glm::vec3 _specular,
    glm::vec3 _diffuse,
    float _constantAttenuation,
    float _linearAttenuation,
    float _quadraticAttenuation,
    float _spotExponent
)
{
	Light_position.push_back(_position.x);
	Light_position.push_back(_position.y);
	Light_position.push_back(_position.z);
	Light_specular.push_back(_specular.x);
	Light_specular.push_back(_specular.y);
	Light_specular.push_back(_specular.z);
	Light_diffuse.push_back(_diffuse.x);
	Light_diffuse.push_back(_diffuse.y);
	Light_diffuse.push_back(_diffuse.z);
	Light_constantAttenuation.push_back(_constantAttenuation);
	Light_linearAttenuation.push_back(_linearAttenuation);
	Light_quadraticAttenuation.push_back(_quadraticAttenuation);
	Light_spotExponent.push_back(_spotExponent);
	Lights = Light_constantAttenuation.size();
}

void Renderer::AddAABBToDraw(glm::vec3 origin, glm::vec3 volumeVector, bool colliding)
{
	glm::mat4 model;
	model *= glm::translate(origin);
	model *= glm::scale(volumeVector);
	AABBsToRender.push_back(std::make_tuple(model, colliding));
}

GLuint Renderer::CreateQuad()
{
	float quadVertices[] =
	{
		-1.0f, -1.0f, 0.0f,
		1.0f, 1.0f, 0.0f,
		-1.0f, 1.0f, 0.0f,

		-1.0f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		1.0f, 1.0f, 0.0f,
	};
	float quadTexCoords[] =
	{
		0.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,

		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
	};
	GLuint vbo[2], vao;
	glGenBuffers(2, vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, 3 * 6 * sizeof(float), quadVertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, 2 * 6 * sizeof(float), quadTexCoords, GL_STATIC_DRAW);
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	return vao;
}

GLuint Renderer::CreateAABB()
{
	float vertices[] =
	{
		// Bottom
		-1.0f, -1.0f, 1.0f, // 0
		1.0f, -1.0f, 1.0f, // 1
		1.0f, -1.0f, 1.0f, // 1
		1.0f, -1.0f, -1.0f, // 2
		1.0f, -1.0f, -1.0f, // 2
		-1.0f, -1.0f, -1.0f, // 3
		-1.0f, -1.0f, -1.0f, // 3
		-1.0f, -1.0f, 1.0f, // 0

		// Top
		-1.0f, 1.0f, 1.0f, // 4
		1.0f, 1.0f, 1.0f, // 5
		1.0f, 1.0f, 1.0f, // 5
		1.0f, 1.0f, -1.0f, // 6
		1.0f, 1.0f, -1.0f, // 6
		-1.0f, 1.0f, -1.0f, // 7
		-1.0f, 1.0f, -1.0f, // 7
		-1.0f, 1.0f, 1.0f, // 4

		// Connectors
		-1.0f, -1.0f, 1.0f, // 0
		-1.0f, 1.0f, 1.0f, // 4
		1.0f, -1.0f, 1.0f, // 1
		1.0f, 1.0f, 1.0f, // 5
		1.0f, -1.0f, -1.0f, // 2
		1.0f, 1.0f, -1.0f, // 6
		-1.0f, -1.0f, -1.0f, // 3
		-1.0f, 1.0f, -1.0f, // 7
	};

	GLuint vbo, vao;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	return vao;
}

GLuint Renderer::CreateSkybox()
{
	glm::vec3 skyBoxVertices[] =
	{
		glm::vec3( 1.0f, -1.0f, -1.0f), glm::vec3( 1.0f, -1.0f,  1.0f), glm::vec3( 1.0f,  1.0f,  1.0f), glm::vec3( 1.0f,  1.0f,  1.0f), glm::vec3( 1.0f,  1.0f, -1.0f), glm::vec3( 1.0f, -1.0f, -1.0f),
		glm::vec3(-1.0f, -1.0f,  1.0f), glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec3(-1.0f,  1.0f, -1.0f), glm::vec3(-1.0f,  1.0f, -1.0f), glm::vec3(-1.0f,  1.0f,  1.0f), glm::vec3(-1.0f, -1.0f,  1.0f),
		glm::vec3(-1.0f,  1.0f, -1.0f), glm::vec3( 1.0f,  1.0f, -1.0f), glm::vec3( 1.0f,  1.0f,  1.0f), glm::vec3( 1.0f,  1.0f,  1.0f), glm::vec3(-1.0f,  1.0f,  1.0f), glm::vec3(-1.0f,  1.0f, -1.0f),
		glm::vec3(-1.0f, -1.0f,  1.0f), glm::vec3( 1.0f, -1.0f,  1.0f), glm::vec3( 1.0f, -1.0f, -1.0f), glm::vec3( 1.0f, -1.0f, -1.0f), glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec3(-1.0f, -1.0f,  1.0f),
		glm::vec3( 1.0f, -1.0f,  1.0f), glm::vec3(-1.0f, -1.0f,  1.0f), glm::vec3(-1.0f,  1.0f,  1.0f), glm::vec3(-1.0f,  1.0f,  1.0f), glm::vec3( 1.0f,  1.0f,  1.0f), glm::vec3( 1.0f, -1.0f,  1.0f),
		glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec3( 1.0f, -1.0f, -1.0f), glm::vec3( 1.0f,  1.0f, -1.0f), glm::vec3( 1.0f,  1.0f, -1.0f), glm::vec3(-1.0f,  1.0f, -1.0f), glm::vec3(-1.0f, -1.0f, -1.0f)
	};

	GLuint vbo, vao;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyBoxVertices), skyBoxVertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);

	return vao;
}
void Renderer::ClearStuff()
{
	AABBsToRender.clear();
	ModelsToRender.clear();
	TexturesToRender.clear();
	Light_position.clear();
	Light_specular.clear();
	Light_diffuse.clear();
	Light_constantAttenuation.clear();
	Light_linearAttenuation.clear();
	Light_quadraticAttenuation.clear();
	Light_spotExponent.clear();
	Lights = 0;
}