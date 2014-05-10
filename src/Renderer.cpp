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
/*	Lights = 0;*/
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
	/*auto standardVS = std::shared_ptr<Shader>(new VertexShader("Shaders/Vertex.glsl"));
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
	m_ShaderProgramSkybox.Link();*/

	m_FirstPassProgram.AddShader(std::shared_ptr<Shader>(new VertexShader("Shaders/Vertex.glsl")));
	m_FirstPassProgram.AddShader(std::shared_ptr<Shader>(new FragmentShader("Shaders/Fragment.glsl")));
	m_FirstPassProgram.Compile();
	glBindFragDataLocation(m_FirstPassProgram.GetHandle(), 0, "frag_Diffuse");
	glBindFragDataLocation(m_FirstPassProgram.GetHandle(), 1, "frag_Position");
	glBindFragDataLocation(m_FirstPassProgram.GetHandle(), 2, "frag_Normal");
	m_FirstPassProgram.Link();

	m_SecondPassProgram.AddShader(std::shared_ptr<Shader>(new VertexShader("Shaders/Vertex2.glsl")));
	m_SecondPassProgram.AddShader(std::shared_ptr<Shader>(new FragmentShader("Shaders/Fragment2.glsl")));
	m_SecondPassProgram.Compile();
	m_SecondPassProgram.Link();

	m_SecondPassProgram_Debug.AddShader(std::shared_ptr<Shader>(new VertexShader("Shaders/Vertex2.glsl")));
	m_SecondPassProgram_Debug.AddShader(std::shared_ptr<Shader>(new FragmentShader("Shaders/Fragment2-Debug.glsl")));
	m_SecondPassProgram_Debug.Compile();
	m_SecondPassProgram_Debug.Link();

	m_FinalPassProgram.AddShader(std::shared_ptr<Shader>(new VertexShader("Shaders/FinalPass.vert.glsl")));
	m_FinalPassProgram.AddShader(std::shared_ptr<Shader>(new FragmentShader("Shaders/FinalPass.frag.glsl")));
	m_FinalPassProgram.Compile();
	m_FinalPassProgram.Link();
	Gamma = 1;
	m_ScreenQuad = CreateQuad();

	FrameBufferTextures();
}

void Renderer::Draw(double dt)
{

	if(glfwGetKey(m_Window, GLFW_KEY_F1))
	{
		m_QuadView = false;
	}
	if(glfwGetKey(m_Window, GLFW_KEY_F2))
	{
		m_QuadView = true;
	}

	if(glfwGetKey(m_Window, GLFW_KEY_KP_1))
	{
		Gamma -= 0.3f * dt; 
		LOG_INFO("Gamma_UP: %f", Gamma);
	}
	if(glfwGetKey(m_Window, GLFW_KEY_KP_4))
	{
		Gamma += 0.3f * dt;
		LOG_INFO("Gamma_DOWN: %f", Gamma);
	}

	glDisable(GL_BLEND);

	DrawFBO();

	ClearStuff();
	glfwSwapBuffers(m_Window);
}

#pragma region TempRegion

void Renderer::DrawSkybox()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, WIDTH, HEIGHT);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	m_ShaderProgramSkybox.Bind();
	glm::mat4 cameraMatrix = m_Camera->ProjectionMatrix() * glm::toMat4(glm::inverse(m_Camera->Orientation()));
	glUniformMatrix4fv(glGetUniformLocation(m_ShaderProgramSkybox.GetHandle(), "MVP"), 1, GL_FALSE, glm::value_ptr(cameraMatrix));
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	m_Skybox->Draw();
}

void Renderer::DrawScene()
{
// 	glBindFramebuffer(GL_FRAMEBUFFER, 0);
// 	glViewport(0, 0, WIDTH, HEIGHT);

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
	glUniform1i(glGetUniformLocation(m_ShaderProgram.GetHandle(), "numberOfLights"), Lights.size());
// 	glUniform3fv(glGetUniformLocation(m_ShaderProgram.GetHandle(), "position"), Lights, Light_position.data());
// 	glUniform3fv(glGetUniformLocation(m_ShaderProgram.GetHandle(), "specular"), Lights, Light_specular.data());
// 	glUniform3fv(glGetUniformLocation(m_ShaderProgram.GetHandle(), "diffuse"), Lights, Light_diffuse.data());
	glUniform1fv(glGetUniformLocation(m_ShaderProgram.GetHandle(), "constantAttenuation"), Lights.size(), Light_constantAttenuation.data());
	glUniform1fv(glGetUniformLocation(m_ShaderProgram.GetHandle(), "linearAttenuation"), Lights.size(), Light_linearAttenuation.data());
	glUniform1fv(glGetUniformLocation(m_ShaderProgram.GetHandle(), "quadraticAttenuation"), Lights.size(), Light_quadraticAttenuation.data());
	glUniform1fv(glGetUniformLocation(m_ShaderProgram.GetHandle(), "spotExponent"), Lights.size(), Light_spotExponent.data());
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
	glEnable(GL_DEPTH_TEST);//Tests where objects are and display them correctly
	glEnable(GL_CULL_FACE);	//removes triangles on the wrong side of the object
	glCullFace(GL_FRONT);	//Make it so that only the back faces are rendered

	//Binds the FBO and sets the veiwport, witch in effect is how large the shadowmap is and what resolution it has.
	glBindFramebuffer(GL_FRAMEBUFFER, m_ShadowFrameBuffer);
	glViewport(0, 0, m_ShadowMapRes, m_ShadowMapRes);

	glClear(GL_DEPTH_BUFFER_BIT);
	//glClearColor(0.0f, 0.0f, 0.0f, 0.0f);


	//Creates the "camera" for the shadowmap from the direction of the sun.
	glm::mat4 depthViewMatrix = glm::lookAt(m_SunPosition, m_SunTarget, glm::vec3(0, 1, 0)) * glm::translate(-m_Camera->Position() * glm::vec3(1, 0, 0));
//	glm::mat4 depthViewMatrix = glm::lookAt(m_SunPosition, m_SunTarget, glm::vec3(0, 1, 0));
	glm::mat4 depthCamera = m_SunProjection * depthViewMatrix;

	//glm::mat4 cameraMatrix = depthProjectionMatrix * m_Camera->ViewMatrix();

	glm::mat4 MVP;

	m_ShaderProgramShadows.Bind();
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); //Draws filled polygons

	//For each model, render them to the shadowmap
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

void Renderer::AddPointLightToDraw(
    glm::vec3 _position,
    glm::vec3 _specular,
    glm::vec3 _diffuse,
    float _specularExponent,
	float _scale
)
{
	Light light;
	light.Position = _position;
	light.Diffuse = _diffuse;
	light.Specular = _specular;
	light.Scale = _scale;
	light.SpecularExponent = _specularExponent;
	light.SphereModelMatrix = CreateLightMatrix(light);
	Lights.push_back(light);
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
	Lights.clear();
}

#pragma endregion

void Renderer::FrameBufferTextures()
{
	m_fbBasePass = 0;
	m_fDepthBuffer = 0;

	glGenFramebuffers(1, &m_fbBasePass);
	glGenRenderbuffers(1, &m_fDepthBuffer);

	glBindRenderbuffer(GL_RENDERBUFFER, m_fDepthBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, WIDTH, HEIGHT);

	//Generate and bind diffuse texture
	glGenTextures(1, &m_fDiffuseTexture);
	glBindTexture(GL_TEXTURE_2D, m_fDiffuseTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, WIDTH, HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	//Generate and bind position texture
	glGenTextures(1, &m_fPositionTexture);
	glBindTexture(GL_TEXTURE_2D, m_fPositionTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, WIDTH, HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	//Generate and bind normal texture
	glGenTextures(1, &m_fNormalsTexture);
	glBindTexture(GL_TEXTURE_2D, m_fNormalsTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB10_A2, WIDTH, HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	//Bind fb
	glBindFramebuffer(GL_FRAMEBUFFER, m_fbBasePass);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_fDepthBuffer);

	//Attach textures to the FB
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_fDiffuseTexture, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, m_fPositionTexture, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, m_fNormalsTexture, 0);

	GLenum fbStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if(fbStatus != GL_FRAMEBUFFER_COMPLETE) 
	{
		LOG_ERROR("DeferredLighting:Init: FrameBuffer incomplete: 0x%x\n", fbStatus);
		//exit(1);
	}

	m_fbLightingPass = 0;
	glGenFramebuffers(1, &m_fbLightingPass);

	glGenTextures(1, &m_fLightingTexture);
	glBindTexture(GL_TEXTURE_2D, m_fLightingTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, WIDTH, HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glBindFramebuffer(GL_FRAMEBUFFER, m_fbLightingPass);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_fLightingTexture, 0);

	fbStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if(fbStatus != GL_FRAMEBUFFER_COMPLETE) 
	{
		LOG_ERROR("DeferredLighting:Init: FrameBuffer incomplete: 0x%x\n", fbStatus);
		//exit(1);
	}


}

void Renderer::DrawFBO()
{
	/*
		Base pass
	*/
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_fbBasePass);

	// Clear G-buffer
	GLenum windowBuffClear[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
	glDrawBuffers(3, windowBuffClear);
	glClearColor(0.f, 0.f, 0.f, 0.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Execute the first render stage which will fill out the internal buffers with data(??)
	m_FirstPassProgram.Bind();
	GLenum windowBuffOpaque[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
	glDrawBuffers(3, windowBuffOpaque);
	
	glCullFace(GL_BACK);
	DrawFBOScene();

	/*
		Lighting pass
	*/
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_fbLightingPass);
	GLenum lightingPassAttachments[] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, lightingPassAttachments);

	glClearColor(0.f, 0.f, 0.f, 0.f);
	glClear(GL_COLOR_BUFFER_BIT);

	m_SecondPassProgram.Bind();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_fPositionTexture);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_fNormalsTexture);

	glCullFace(GL_FRONT);
	DrawLightScene();

	/*
		Final pass
	*/
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m_FinalPassProgram.Bind();

	// Ambient light
	glUniform3fv(glGetUniformLocation(m_FinalPassProgram.GetHandle(), "La"), 1, glm::value_ptr(glm::vec3(0.3f, 0.3f, 0.3f)));
	glUniform1f(glGetUniformLocation(m_FinalPassProgram.GetHandle(), "Gamma"), Gamma);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_fDiffuseTexture);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_fLightingTexture);

	glCullFace(GL_BACK);
	glBindVertexArray(m_ScreenQuad);
	glEnableVertexAttribArray(0);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void Renderer::DrawFBOScene()
{	
	glm::mat4 cameraMatrix = m_Camera->ProjectionMatrix() * m_Camera->ViewMatrix();
	glm::mat4 MVP;

	for (auto tuple : ModelsToRender)
	{
		Model* model;
		glm::mat4 modelMatrix;
		bool visible;
		std::tie(model, modelMatrix, visible, std::ignore) = tuple;
		if (!visible)
			continue;

		MVP = cameraMatrix * modelMatrix;
		glUniformMatrix4fv(glGetUniformLocation(m_FirstPassProgram.GetHandle(), "MVP"), 1, GL_FALSE, glm::value_ptr(MVP));
		glUniformMatrix4fv(glGetUniformLocation(m_FirstPassProgram.GetHandle(), "M"), 1, GL_FALSE, glm::value_ptr(modelMatrix));
		glUniformMatrix4fv(glGetUniformLocation(m_FirstPassProgram.GetHandle(), "V"), 1, GL_FALSE, glm::value_ptr(m_Camera->ViewMatrix()));
		glUniformMatrix4fv(glGetUniformLocation(m_FirstPassProgram.GetHandle(), "P"), 1, GL_FALSE, glm::value_ptr(m_Camera->ProjectionMatrix()));
		glBindVertexArray(model->VAO);
		for (auto texGroup : model->TextureGroups)
		{
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, *texGroup.Texture);
			glDrawArrays(GL_TRIANGLES, texGroup.StartIndex, texGroup.EndIndex - texGroup.StartIndex + 1);
		}
	}
}



void Renderer::DrawLightScene()
{
	glEnable(GL_BLEND);
	glBlendEquation (GL_FUNC_ADD);
	glBlendFunc(GL_ONE,GL_ONE);

	glDisable (GL_DEPTH_TEST);
	glDepthMask (GL_FALSE);
	glBindVertexArray(m_sphereModel->VAO);

	glm::mat4 cameraMatrix = m_Camera->ProjectionMatrix() * m_Camera->ViewMatrix();
	glm::mat4 MVP;

	for (auto &light : Lights)
	{
		MVP = cameraMatrix * light.SphereModelMatrix;
		
		glUniform2fv(glGetUniformLocation(m_SecondPassProgram.GetHandle(), "ViewportSize"), 1,glm::value_ptr(glm::vec2(WIDTH, HEIGHT)));
		glUniformMatrix4fv(glGetUniformLocation(m_SecondPassProgram.GetHandle(), "MVP"), 1, GL_FALSE, glm::value_ptr(MVP));
		glUniformMatrix4fv(glGetUniformLocation(m_SecondPassProgram.GetHandle(), "V"), 1, GL_FALSE, glm::value_ptr(m_Camera->ViewMatrix()));
		glUniformMatrix4fv(glGetUniformLocation(m_SecondPassProgram.GetHandle(), "P"), 1, GL_FALSE, glm::value_ptr(m_Camera->ProjectionMatrix()));
		glUniformMatrix4fv(glGetUniformLocation(m_SecondPassProgram.GetHandle(), "M"), 1, GL_FALSE, glm::value_ptr(light.SphereModelMatrix));
		glUniform3fv(glGetUniformLocation(m_SecondPassProgram.GetHandle(), "ls"), 1, glm::value_ptr(light.Specular));
		glUniform3fv(glGetUniformLocation(m_SecondPassProgram.GetHandle(), "ld"), 1, glm::value_ptr(light.Diffuse));
		glUniform3fv(glGetUniformLocation(m_SecondPassProgram.GetHandle(), "lp"), 1, glm::value_ptr(light.Position));
		glUniform1f(glGetUniformLocation(m_SecondPassProgram.GetHandle(), "LightRadius"), light.Scale/2.f);
		glUniform3f(glGetUniformLocation(m_SecondPassProgram.GetHandle(), "CameraPosition"), m_Camera->Position().x, m_Camera->Position().y, m_Camera->Position().z);
		glUniform1f(glGetUniformLocation(m_SecondPassProgram.GetHandle(), "specularExponent"), light.SpecularExponent);
 		glDrawArrays(GL_TRIANGLES, 0, m_sphereModel->Vertices.size());
	};
	glEnable (GL_DEPTH_TEST);
	glDepthMask (GL_TRUE);
	glDisable (GL_BLEND);
}

void Renderer::SetSphereModel( Model* _model )
{
	m_sphereModel = _model;
}

glm::mat4 Renderer::CreateLightMatrix(Light &_light)
{
	glm::mat4 model;
	model *= glm::translate(_light.Position);
	model *= glm::scale(glm::vec3(_light.Scale));
	return model;
}

