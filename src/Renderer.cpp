#include "PrecompiledHeader.h"
#include "Renderer.h"

Renderer::Renderer(std::shared_ptr<::ResourceManager> resourceManager)
	: ResourceManager(resourceManager)
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
	Gamma = 0.85f;
	CAtt = 1.0f;
	LAtt = 0.0f;
	QAtt = 3.0f;
	m_ShadowMapRes = 2048*2;
	m_SunPosition = glm::vec3(0.f, 1.0f, 0.5f);
	m_SunTarget = glm::vec3(0, 0, 0);
	m_SunProjection_height = glm::vec2(-40.f, 40.f);
	m_SunProjection_width = glm::vec2(-40.f, 40.f);
	m_SunProjection_length = glm::vec2(-500.f, 500.f);
	m_SunProjection = glm::ortho<float>(m_SunProjection_width.x, m_SunProjection_width.y, m_SunProjection_height.x, m_SunProjection_height.y, m_SunProjection_length.x, m_SunProjection_length.y);
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
	m_Width = 1280;
	m_Height = 720;
	// Antialiasing
	//glfwWindowHint(GLFW_SAMPLES, 16);
	m_Window = glfwCreateWindow(m_Width, m_Height, "OpenGL", nullptr, nullptr);
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
	m_Camera = std::make_shared<Camera>(45.f, 0.01f, 1000.f);
	m_Camera->SetPosition(glm::vec3(0.0f, 0.0f, 2.f));

	glfwSwapInterval(m_VSync);

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
	
	m_ShaderProgramShadowsDrawDepth.AddShader(std::shared_ptr<Shader>(new VertexShader("Shaders/VisualizeDepth.vert.glsl")));
	m_ShaderProgramShadowsDrawDepth.AddShader(std::shared_ptr<Shader>(new FragmentShader("Shaders/VisualizeDepth.frag.glsl")));
	m_ShaderProgramShadowsDrawDepth.Compile();
	m_ShaderProgramShadowsDrawDepth.Link();

	m_ShaderProgramDebugAABB.AddShader(standardVS);
	m_ShaderProgramDebugAABB.AddShader(std::shared_ptr<Shader>(new FragmentShader("Shaders/AABB.frag.glsl")));
	m_ShaderProgramDebugAABB.Compile();
	m_ShaderProgramDebugAABB.Link();*/

	m_FinalForwardPassProgram.AddShader(std::shared_ptr<Shader>(new VertexShader("Shaders/FinalForwardPass.vert.glsl")));
	m_FinalForwardPassProgram.AddShader(std::shared_ptr<Shader>(new FragmentShader("Shaders/FinalForwardPass.frag.glsl")));
	m_FinalForwardPassProgram.Compile();
	m_FinalForwardPassProgram.Link();

 	m_BlendMapProgram.AddShader(std::shared_ptr<Shader>(new VertexShader("Shaders/BlendMap.vert.glsl")));
 	m_BlendMapProgram.AddShader(std::shared_ptr<Shader>(new FragmentShader("Shaders/BlendMap.frag.glsl")));
 	m_BlendMapProgram.Compile();
 	m_BlendMapProgram.Link();

	m_ShaderProgramSkybox.AddShader(std::shared_ptr<Shader>(new VertexShader("Shaders/Skybox.vert.glsl")));
	m_ShaderProgramSkybox.AddShader(std::shared_ptr<Shader>(new FragmentShader("Shaders/Skybox.frag.glsl")));
	m_ShaderProgramSkybox.Compile();
	m_ShaderProgramSkybox.Link();

	m_SunPassProgram.AddShader(std::shared_ptr<Shader>(new VertexShader("Shaders/SunPass.vert.glsl")));
	m_SunPassProgram.AddShader(std::shared_ptr<Shader>(new FragmentShader("Shaders/SunPass.frag.glsl")));
	m_SunPassProgram.Compile();
	m_SunPassProgram.Link();

	m_ForwardRendering.AddShader(std::shared_ptr<Shader>(new VertexShader("Shaders/ForwardRendering.vert.glsl")));
	m_ForwardRendering.AddShader(std::shared_ptr<Shader>(new FragmentShader("Shaders/ForwardRendering.frag.glsl")));
	m_ForwardRendering.Compile();
	glBindFragDataLocation(m_ForwardRendering.GetHandle(), 0, "frag_Diffuse");
	m_ForwardRendering.Link();

	m_ShaderProgramShadows.AddShader(std::shared_ptr<Shader>(new VertexShader("Shaders/ShadowMap.vert.glsl")));
	m_ShaderProgramShadows.AddShader(std::shared_ptr<Shader>(new FragmentShader("Shaders/ShadowMap.frag.glsl")));
	m_ShaderProgramShadows.Compile();
	m_ShaderProgramShadows.Link();

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
	m_ScreenQuad = CreateQuad();
	CreateShadowMap(m_ShadowMapRes);
	FrameBufferTextures();

	m_sphereModel = ResourceManager->Load<Model>("Model", "Models/Placeholders/PhysicsTest/Sphere.obj");
	m_Skybox = std::make_shared<Skybox>("Textures/Skybox/Sky34", "jpg");
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

	//if(glfwGetKey(m_Window, GLFW_KEY_KP_1))
	//{
	//	Gamma -= 0.3f * dt; 
	//	LOG_INFO("Gamma_UP: %f", Gamma);
	//}
	//if(glfwGetKey(m_Window, GLFW_KEY_KP_4))
	//{
	//	Gamma += 0.3f * dt;
	//	LOG_INFO("Gamma_DOWN: %f", Gamma);
	//}

	if(glfwGetKey(m_Window, GLFW_KEY_KP_7))
	{
		m_SunProjection_height.x += 10.f * dt;
		LOG_INFO("Heightx+: %f", m_SunProjection_height);
		m_SunProjection = glm::ortho<float>(m_SunProjection_width.x, m_SunProjection_width.y, m_SunProjection_height.x, m_SunProjection_height.y, m_SunProjection_length.x, m_SunProjection_length.y);
	}
	else if(glfwGetKey(m_Window, GLFW_KEY_KP_4))
	{
		m_SunProjection_height.x -= 10.f * dt;
		LOG_INFO("Heightx-: %f", m_SunProjection_height);
		m_SunProjection = glm::ortho<float>(m_SunProjection_width.x, m_SunProjection_width.y, m_SunProjection_height.x, m_SunProjection_height.y, m_SunProjection_length.x, m_SunProjection_length.y);
	}

	if(glfwGetKey(m_Window, GLFW_KEY_KP_8))
	{
		m_SunProjection_height.y += 10.f * dt;
		LOG_INFO("Heightx+: %f", m_SunProjection_height);
		m_SunProjection = glm::ortho<float>(m_SunProjection_width.x, m_SunProjection_width.y, m_SunProjection_height.x, m_SunProjection_height.y, m_SunProjection_length.x, m_SunProjection_length.y);
	}
	else if(glfwGetKey(m_Window, GLFW_KEY_KP_5))
	{
		m_SunProjection_height.y -= 10.f * dt;
		LOG_INFO("Heightx-: %f", m_SunProjection_height);
		m_SunProjection = glm::ortho<float>(m_SunProjection_width.x, m_SunProjection_width.y, m_SunProjection_height.x, m_SunProjection_height.y, m_SunProjection_length.x, m_SunProjection_length.y);
	}


	if(glfwGetKey(m_Window, GLFW_KEY_1))
	{
		if(glfwGetKey(m_Window, GLFW_KEY_KP_ADD))
		{
			CAtt += 0.5f * dt;
			LOG_INFO("Const: %f", CAtt);
		}
		if(glfwGetKey(m_Window, GLFW_KEY_KP_SUBTRACT))
		{
			CAtt -= 0.5f * dt;
			LOG_INFO("Const: %f", CAtt);
		}
	}
	if(glfwGetKey(m_Window, GLFW_KEY_2))
	{
		if(glfwGetKey(m_Window, GLFW_KEY_KP_ADD))
		{
			LAtt += 0.5f * dt;
			LOG_INFO("Linear: %f", LAtt);
		}
		if(glfwGetKey(m_Window, GLFW_KEY_KP_SUBTRACT))
		{
			LAtt -= 0.5f * dt;
			LOG_INFO("Linear: %f", LAtt);
		}
	}
	if(glfwGetKey(m_Window, GLFW_KEY_3))
	{
		if(glfwGetKey(m_Window, GLFW_KEY_KP_ADD))
		{
			QAtt += 0.5f * dt;
			LOG_INFO("Quadratic: %f", QAtt);
		}
		if(glfwGetKey(m_Window, GLFW_KEY_KP_SUBTRACT))
		{
			QAtt -= 0.5f * dt;
			LOG_INFO("Quadratic: %f", QAtt);
		}
	}
	
	glDisable(GL_BLEND);

	DrawFBO();

	ClearStuff();
	glfwSwapBuffers(m_Window);
}

void Renderer::DrawFrame(RenderQueuePair &rq)
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(m_Viewport.X, m_Height - m_Viewport.Y - m_Viewport.Height, m_Viewport.Width, m_Viewport.Height);
	glScissor(m_Viewport.X, m_Height - m_Viewport.Y - m_Viewport.Height, m_Viewport.Width, m_Viewport.Height);

	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glClearColor(0.0f, 0.5f, 0.0f, 1.0f);

	glDisable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//glm::mat4 cameraMatrix = m_Camera->ProjectionMatrix((float)m_Width / m_Height) * m_Camera->ViewMatrix();
	//glm::mat4 MVP;

	m_ForwardRendering.Bind();

	//for (auto tuple : ModelsToRender) //// Todo: Add so it's TransparentModelsToRender
	//{
	//	Model* model;
	//	glm::mat4 modelMatrix;
	//	bool visible;
	//	std::tie(model, modelMatrix, visible, std::ignore) = tuple;
	//	if (!visible)
	//		continue;

	//	MVP = cameraMatrix * modelMatrix;
	//	glUniformMatrix4fv(glGetUniformLocation(m_ForwardRendering.GetHandle(), "MVP"), 1, GL_FALSE, glm::value_ptr(MVP));
	//	glUniformMatrix4fv(glGetUniformLocation(m_ForwardRendering.GetHandle(), "M"), 1, GL_FALSE, glm::value_ptr(modelMatrix));
	//	glUniformMatrix4fv(glGetUniformLocation(m_ForwardRendering.GetHandle(), "V"), 1, GL_FALSE, glm::value_ptr(m_Camera->ViewMatrix()));
	//	glUniformMatrix4fv(glGetUniformLocation(m_ForwardRendering.GetHandle(), "P"), 1, GL_FALSE, glm::value_ptr(m_Camera->ProjectionMatrix((float)m_Width / m_Height)));

	//	glBindVertexArray(model->VAO);
	//	for (auto texGroup : model->TextureGroups)
	//	{
	//		glActiveTexture(GL_TEXTURE0);
	//		glBindTexture(GL_TEXTURE_2D, *texGroup.Texture);
	//		glDrawArrays(GL_TRIANGLES, texGroup.StartIndex, texGroup.EndIndex - texGroup.StartIndex + 1);
	//	}
	//}

	for (auto &job : rq.Forward)
	{
		//auto modelJob = std::dynamic_pointer_cast<ModelJob>(job);
		//if (modelJob)
		//{
		//	glm::mat4 modelMatrix = modelJob->ModelMatrix;

		//	MVP = cameraMatrix * modelMatrix;
		//	depthMVP = depthCameraMatrix * modelMatrix;
		//	glUniformMatrix4fv(glGetUniformLocation(m_FirstPassProgram.GetHandle(), "MVP"), 1, GL_FALSE, glm::value_ptr(MVP));
		//	glUniformMatrix4fv(glGetUniformLocation(m_FirstPassProgram.GetHandle(), "DepthMVP"), 1, GL_FALSE, glm::value_ptr(depthMVP));
		//	glUniformMatrix4fv(glGetUniformLocation(m_FirstPassProgram.GetHandle(), "M"), 1, GL_FALSE, glm::value_ptr(modelMatrix));
		//	glUniformMatrix4fv(glGetUniformLocation(m_FirstPassProgram.GetHandle(), "V"), 1, GL_FALSE, glm::value_ptr(m_Camera->ViewMatrix()));
		//	glUniformMatrix4fv(glGetUniformLocation(m_FirstPassProgram.GetHandle(), "P"), 1, GL_FALSE, glm::value_ptr(cameraProjection));
		//	//glUniform3fv(glGetUniformLocation(m_SecondPassProgram.GetHandle(), "SunDirection_cameraspace"), 1, glm::value_ptr(sunDirection_cameraview));

		//	glBindVertexArray(modelJob->VAO);
		//	glActiveTexture(GL_TEXTURE0);
		//	glBindTexture(GL_TEXTURE_2D, modelJob->DiffuseTexture);
		//	if (modelJob->NormalTexture != 0)
		//	{
		//		glActiveTexture(GL_TEXTURE2);
		//		glBindTexture(GL_TEXTURE_2D, modelJob->NormalTexture);
		//	}
		//	if (modelJob->SpecularTexture)
		//	{
		//		glActiveTexture(GL_TEXTURE3);
		//		glBindTexture(GL_TEXTURE_2D, modelJob->SpecularTexture);
		//	}
		//	glDrawArrays(GL_TRIANGLES, modelJob->StartIndex, modelJob->EndIndex - modelJob->StartIndex + 1);

		//	continue;
		//}

		auto spriteJob = std::dynamic_pointer_cast<SpriteJob>(job);
		if (spriteJob)
		{
			glUniformMatrix4fv(glGetUniformLocation(m_ForwardRendering.GetHandle(), "MVP"), 1, GL_FALSE, glm::value_ptr(glm::mat4()));
			glUniformMatrix4fv(glGetUniformLocation(m_ForwardRendering.GetHandle(), "M"), 1, GL_FALSE, glm::value_ptr(glm::mat4()));
			glUniformMatrix4fv(glGetUniformLocation(m_ForwardRendering.GetHandle(), "V"), 1, GL_FALSE, glm::value_ptr(glm::mat4()));
			glUniformMatrix4fv(glGetUniformLocation(m_ForwardRendering.GetHandle(), "P"), 1, GL_FALSE, glm::value_ptr(glm::mat4()));
			glUniform4fv(glGetUniformLocation(m_ForwardRendering.GetHandle(), "Color"), 1, glm::value_ptr(spriteJob->Color));

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, spriteJob->Texture);
			glBindVertexArray(m_ScreenQuad);
			glDrawArrays(GL_TRIANGLES, 0, 6);

			continue;
		}
	}
}

void Renderer::DrawWorld(RenderQueuePair &rq)
{
	glDisable(GL_BLEND);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glEnable(GL_SCISSOR_TEST);

	//DrawShadowMap(rq.Deferred);

	/*
	Base pass
	*/
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_fbBasePass);
	glViewport(m_Viewport.X, m_Height - m_Viewport.Y - m_Viewport.Height, m_Viewport.Width, m_Viewport.Height);
	glScissor(m_Viewport.X, m_Height - m_Viewport.Y - m_Viewport.Height, m_Viewport.Width, m_Viewport.Height);
	//glViewport(0, 0, m_Width, m_Height);

	// Clear G-buffer
	GLenum windowBuffClear[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
	glDrawBuffers(4, windowBuffClear);
	glClearColor(115.f / 255, 192.f / 255, 255.f / 255, 0.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Execute the first render stage which will fill out the internal buffers with data(??)
	m_FirstPassProgram.Bind();
	GLenum windowBuffOpaque[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
	glDrawBuffers(4, windowBuffOpaque);

	glCullFace(GL_BACK);
	glEnable(GL_DEPTH_TEST);
	//DrawSkybox();
	DrawFBOScene(rq.Deferred);

	/*
	Lighting pass
	*/
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_fbLightingPass);
	glViewport(m_Viewport.X, m_Height - m_Viewport.Y - m_Viewport.Height, m_Viewport.Width, m_Viewport.Height);
	glScissor(m_Viewport.X, m_Height - m_Viewport.Y - m_Viewport.Height, m_Viewport.Width, m_Viewport.Height);
	GLenum lightingPassAttachments[] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, lightingPassAttachments);

	glClearColor(0.f, 0.f, 0.f, 0.f);
	glClear(GL_COLOR_BUFFER_BIT);

	m_SecondPassProgram.Bind();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_fPositionTexture);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_fNormalsTexture);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, m_fSpecularTexture);

	glCullFace(GL_FRONT);
	DrawLightScene(rq.Deferred);
	DrawSunLightScene();

	/*
	Final pass
	*/
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	//glViewport(m_Viewport.X, m_Viewport.Y, m_Viewport.Width, m_Viewport.Height);
	glViewport(0, 0, m_Width, m_Height);
	//glScissor(0, 0, m_Width, m_Height);
	glScissor(m_Viewport.X, m_Height - m_Viewport.Y - m_Viewport.Height, m_Viewport.Width, m_Viewport.Height);

	glClear(GL_DEPTH_BUFFER_BIT);

	m_FinalPassProgram.Bind();

	// Ambient light
	glUniform3fv(glGetUniformLocation(m_FinalPassProgram.GetHandle(), "La"), 1, glm::value_ptr(glm::vec3(0.7f)));
	glUniform1f(glGetUniformLocation(m_FinalPassProgram.GetHandle(), "Gamma"), Gamma);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_fDiffuseTexture);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_fLightingTexture);

	glCullFace(GL_BACK);
	glBindVertexArray(m_ScreenQuad);
	glEnableVertexAttribArray(0);
	glDrawArrays(GL_TRIANGLES, 0, 6);

	/*
		Transparency
	*/
	ForwardRendering(rq.Forward);
}

void Renderer::ForwardRendering(RenderQueue &rq)
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_fbBasePass);

	glViewport(m_Viewport.X, m_Height - m_Viewport.Y - m_Viewport.Height, m_Viewport.Width, m_Viewport.Height);
	glScissor(m_Viewport.X, m_Height - m_Viewport.Y - m_Viewport.Height, m_Viewport.Width, m_Viewport.Height);

	// Clear G-buffer
	GLenum attachments[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
	glDrawBuffers(4, attachments);
	glClearColor(0.f, 0.f, 0.f, 0.f);
	glClear(GL_COLOR_BUFFER_BIT);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_FALSE);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	glm::mat4 cameraProjection = m_Camera->ProjectionMatrix((float)m_Viewport.Width / m_Viewport.Height);
	glm::mat4 cameraMatrix = cameraProjection * m_Camera->ViewMatrix();
	glm::mat4 MVP;

	m_ForwardRendering.Bind();
	GLuint ShaderProgramHandle = m_ForwardRendering.GetHandle();
	for (auto &job : rq)
	{
		auto modelJob = std::dynamic_pointer_cast<ModelJob>(job);
		if (modelJob)
		{
			glm::mat4 modelMatrix = modelJob->ModelMatrix;
			MVP = cameraMatrix * modelMatrix;

			glUniformMatrix4fv(glGetUniformLocation(ShaderProgramHandle, "MVP"), 1, GL_FALSE, glm::value_ptr(MVP));
			glUniformMatrix4fv(glGetUniformLocation(ShaderProgramHandle, "M"), 1, GL_FALSE, glm::value_ptr(modelMatrix));
			glUniformMatrix4fv(glGetUniformLocation(ShaderProgramHandle, "V"), 1, GL_FALSE, glm::value_ptr(m_Camera->ViewMatrix()));
			glUniformMatrix4fv(glGetUniformLocation(ShaderProgramHandle, "P"), 1, GL_FALSE, glm::value_ptr(cameraProjection));
			glUniform4fv(glGetUniformLocation(ShaderProgramHandle, "Color"), 1, glm::value_ptr(modelJob->Color));

			glBindVertexArray(modelJob->VAO);

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, modelJob->DiffuseTexture);
			if (modelJob->NormalTexture != 0)
			{
				glActiveTexture(GL_TEXTURE2);
				glBindTexture(GL_TEXTURE_2D, modelJob->NormalTexture);
			}
			if (modelJob->SpecularTexture)
			{
				glActiveTexture(GL_TEXTURE3);
				glBindTexture(GL_TEXTURE_2D, modelJob->SpecularTexture);
			}
			glDrawArrays(GL_TRIANGLES, modelJob->StartIndex, modelJob->EndIndex - modelJob->StartIndex + 1);

			continue;
		}

		auto spriteJob = std::dynamic_pointer_cast<SpriteJob>(job);
		if (spriteJob)
		{
			glm::mat4 modelMatrix = spriteJob->ModelMatrix;
			MVP = cameraMatrix * modelMatrix;

			glUniformMatrix4fv(glGetUniformLocation(ShaderProgramHandle, "MVP"), 1, GL_FALSE, glm::value_ptr(glm::mat4(MVP)));
			glUniformMatrix4fv(glGetUniformLocation(ShaderProgramHandle, "M"), 1, GL_FALSE, glm::value_ptr(glm::mat4(modelMatrix)));
			glUniformMatrix4fv(glGetUniformLocation(ShaderProgramHandle, "V"), 1, GL_FALSE, glm::value_ptr(glm::mat4(m_Camera->ViewMatrix())));
			glUniformMatrix4fv(glGetUniformLocation(ShaderProgramHandle, "P"), 1, GL_FALSE, glm::value_ptr(glm::mat4(cameraProjection)));
			glUniform4fv(glGetUniformLocation(ShaderProgramHandle, "Color"), 1, glm::value_ptr(spriteJob->Color));

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, spriteJob->Texture);
			glBindVertexArray(m_ScreenQuad);
			glDrawArrays(GL_TRIANGLES, 0, 6);

			continue;
		}
	}
	//glDepthMask (GL_TRUE);
	//glDisable (GL_BLEND);

	/*
	Final pass
	*/
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	glViewport(0, 0, m_Width, m_Height);
	glScissor(0, 0, m_Width, m_Height);

	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	m_FinalForwardPassProgram.Bind();
	//ShaderProgramHandle = m_FinalForwardPassProgram.GetHandle();

	// Ambient light
	//glUniform3fv(glGetUniformLocation(ShaderProgramHandle, "La"), 1, glm::value_ptr(glm::vec3(0.7f)));
	//glUniform1f(glGetUniformLocation(ShaderProgramHandle, "Gamma"), Gamma);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_fDiffuseTexture);

	glCullFace(GL_BACK);
	glBindVertexArray(m_ScreenQuad);
	glEnableVertexAttribArray(0);
	glDrawArrays(GL_TRIANGLES, 0, 6);

	//for (auto tuple : ModelsToRender) //// Todo: Add so it's TransparentModelsToRender
	//{
	//	Model* model;
	//	glm::mat4 modelMatrix;
	//	bool visible;
	//	std::tie(model, modelMatrix, visible, std::ignore) = tuple;
	//	if (!visible)
	//		continue;

	//	MVP = cameraMatrix * modelMatrix;
	//	glUniformMatrix4fv(glGetUniformLocation(ShaderProgramHandle, "MVP"), 1, GL_FALSE, glm::value_ptr(MVP));
	//	glUniformMatrix4fv(glGetUniformLocation(ShaderProgramHandle, "M"), 1, GL_FALSE, glm::value_ptr(modelMatrix));
	//	glUniformMatrix4fv(glGetUniformLocation(ShaderProgramHandle, "V"), 1, GL_FALSE, glm::value_ptr(m_Camera->ViewMatrix()));
	//	glUniformMatrix4fv(glGetUniformLocation(ShaderProgramHandle, "P"), 1, GL_FALSE, glm::value_ptr(m_Camera->ProjectionMatrix((float)m_Width / m_Height)));

	//	glBindVertexArray(model->VAO);
	//	for (auto texGroup : model->TextureGroups)
	//	{
	//		glActiveTexture(GL_TEXTURE0);
	//		glBindTexture(GL_TEXTURE_2D, *texGroup.Texture);
	//		glDrawArrays(GL_TRIANGLES, texGroup.StartIndex, texGroup.EndIndex - texGroup.StartIndex + 1);
	//	}
	//}
}

void Renderer::Swap()
{
	glfwSwapBuffers(m_Window);
}

void Renderer::DrawSkybox()
{
	//glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(m_Viewport.X, m_Height - m_Viewport.Y - m_Viewport.Height, m_Viewport.Width, m_Viewport.Height);
	glScissor(m_Viewport.X, m_Height - m_Viewport.Y - m_Viewport.Height, m_Viewport.Width, m_Viewport.Height);

	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	m_ShaderProgramSkybox.Bind();

	//glm::mat4 cameraProjection = m_Camera->ProjectionMatrix((float)m_Viewport.Width / m_Viewport.Height);
	//glm::mat4 cameraMatrix = cameraProjection * m_Camera->ViewMatrix();

	glm::mat4 cameraMatrix = m_Camera->ProjectionMatrix((float)m_Viewport.Width / m_Viewport.Height) * glm::inverse(glm::toMat4(m_Camera->Orientation()));
	glUniformMatrix4fv(glGetUniformLocation(m_ShaderProgramSkybox.GetHandle(), "MVP"), 1, GL_FALSE, glm::value_ptr(cameraMatrix));
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	m_Skybox->Draw();
}

void Renderer::CreateShadowMap(int resolution)
{
	glGenFramebuffers(1, &m_ShadowFrameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, m_ShadowFrameBuffer);

	// Depth texture
	glGenTextures(1, &m_ShadowDepthTexture);
	glBindTexture(GL_TEXTURE_2D, m_ShadowDepthTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, resolution, resolution, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

	//glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_NONE );
	//glTexParameteri( GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE, GL_INTENSITY );

	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, m_ShadowDepthTexture, 0);
	glDrawBuffer(GL_NONE);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		LOG_ERROR("Framebuffer incomplete!");
		return;

	}
}

void Renderer::DrawShadowMap(RenderQueue &rq)
{
	glEnable(GL_DEPTH_TEST);//Tests where objects are and display them correctly
	glEnable(GL_CULL_FACE);	//removes triangles on the wrong side of the object
	glCullFace(GL_FRONT);	//Make it so that only the back faces are rendered

	//Binds the FBO and sets the veiwport, witch in effect is how large the shadowmap is and what resolution it has.
	glBindFramebuffer(GL_FRAMEBUFFER, m_ShadowFrameBuffer);
	glViewport(0, 0, m_ShadowMapRes, m_ShadowMapRes);
	glScissor(0, 0, m_ShadowMapRes, m_ShadowMapRes);

	glClear(GL_DEPTH_BUFFER_BIT);
	//glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	//Creates the "camera" for the shadowmap from the direction of the sun.
	glm::mat4 depthViewMatrix = glm::lookAt(m_SunPosition, m_SunTarget, glm::vec3(0, 1, 0)) * glm::translate(m_Camera->Position() * glm::vec3(1, 1, 1));
	//glm::mat4 depthViewMatrix = glm::lookAt(m_SunPosition, m_SunTarget, glm::vec3(0, 1, 0)) * glm::translate((-m_Camera->Position() + (glm::vec3(40.0) * -m_Camera->Forward())) * glm::vec3(1, 1, 1));
	glm::mat4 depthCamera = m_SunProjection * depthViewMatrix;
	glm::mat4 MVP;

	m_ShaderProgramShadows.Bind();
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); //Draws filled polygons

	//For each model, render them to the shadowmap
	for (auto &job : rq)
	{
		auto modelJob = std::dynamic_pointer_cast<ModelJob>(job);
		if (modelJob)
		{
			glm::mat4 modelMatrix = modelJob->ModelMatrix;

			MVP = depthCamera * modelMatrix;
			glUniformMatrix4fv(glGetUniformLocation(m_ShaderProgramShadows.GetHandle(), "MVP"), 1, GL_FALSE, glm::value_ptr(MVP));
			//glUniform3fv(glGetUniformLocation(m_SecondPassProgram.GetHandle(), "SunDirection_cameraspace"), 1, glm::value_ptr(sunDirection_cameraview));

			glBindVertexArray(modelJob->VAO);
			glDrawArrays(GL_TRIANGLES, modelJob->StartIndex, modelJob->EndIndex - modelJob->StartIndex + 1);

			continue;
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
	//glm::mat4 modelMatrix = glm::translate(glm::mat4(), position) * glm::toMat4(orientation) * glm::scale(scale);

	//glm::vec3 camToParticle = glm::normalize(m_Camera->Position() - position);
	//glm::vec3 up = glm::vec3(0,1,0);
	//glm::vec3 rightVec = glm::normalize(glm::cross(up, camToParticle));
	//glm::vec3 up2 = glm::normalize(glm::cross(camToParticle, rightVec));
	//
	//glm::mat4 billboardMatrix;
	//billboardMatrix[0] = glm::vec4(rightVec, 0);
	//billboardMatrix[1] = glm::vec4(up2, 0);
	//billboardMatrix[2] = glm::vec4(camToParticle, 0);
	////billboardMatrix[3] = glm::vec4(position, 0);

	//TexturesToRender.push_back(std::make_tuple(texture, modelMatrix, billboardMatrix));
}

void Renderer::AddPointLightToDraw(
	glm::vec3 _position,
	glm::vec3 _specular,
	glm::vec3 _diffuse,
	float _specularExponent,
	float _ConstantAttenuation,
	float _LinearAttenuation, 
	float _QuadraticAttenuation,
	float _radius
)
{
	Light light;
	light.Position = _position;
	light.Diffuse = _diffuse;
	light.Specular = _specular;
	light.SpecularExponent = _specularExponent;
	light.ConstantAttenuation = _ConstantAttenuation;
	light.LinearAttenuation = _LinearAttenuation;
	light.QuadraticAttenuation = _QuadraticAttenuation;
	light.Radius = _radius;
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
	TexturesToRender.clear();
	Lights.clear();
}

void Renderer::FrameBufferTextures()
{
	m_fbBasePass = 0;
	m_fDepthBuffer = 0;

	glGenFramebuffers(1, &m_fbBasePass);
	glGenRenderbuffers(1, &m_fDepthBuffer);

	glBindRenderbuffer(GL_RENDERBUFFER, m_fDepthBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, m_Width, m_Height);

	//Generate and bind diffuse texture
	glGenTextures(1, &m_fDiffuseTexture);
	glBindTexture(GL_TEXTURE_2D, m_fDiffuseTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	//Generate and bind position texture
	glGenTextures(1, &m_fPositionTexture);
	glBindTexture(GL_TEXTURE_2D, m_fPositionTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, m_Width, m_Height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	//Generate and bind normal texture
	glGenTextures(1, &m_fNormalsTexture);
	glBindTexture(GL_TEXTURE_2D, m_fNormalsTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, m_Width, m_Height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	//Generate and bind normal texture
	glGenTextures(1, &m_fSpecularTexture);
	glBindTexture(GL_TEXTURE_2D, m_fSpecularTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, m_Width, m_Height, 0, GL_RED, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	/*glGenTextures(1, &m_fShadowTexture);
	glBindTexture(GL_TEXTURE_2D, m_fShadowTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB10_A2, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);*/

	//Bind fb
	glBindFramebuffer(GL_FRAMEBUFFER, m_fbBasePass);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_fDepthBuffer);

	//Attach textures to the FB
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_fDiffuseTexture, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, m_fPositionTexture, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, m_fNormalsTexture, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, m_fSpecularTexture, 0);
	//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, m_fShadowTexture, 0);

	GLenum fbStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if(fbStatus != GL_FRAMEBUFFER_COMPLETE) 
	{
		LOG_ERROR("DeferredLighting:Init: m_fbBasePass incomplete: 0x%x\n", fbStatus);
		//exit(1);
	}

	m_fbLightingPass = 0;
	glGenFramebuffers(1, &m_fbLightingPass);

	glGenTextures(1, &m_fLightingTexture);
	glBindTexture(GL_TEXTURE_2D, m_fLightingTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glBindFramebuffer(GL_FRAMEBUFFER, m_fbLightingPass);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_fLightingTexture, 0);

	fbStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if(fbStatus != GL_FRAMEBUFFER_COMPLETE) 
	{
		LOG_ERROR("DeferredLighting:Init: m_fbLightingPass incomplete: 0x%x\n", fbStatus);
		//exit(1);
	}
}

void Renderer::DrawFBO()
{
	//DrawShadowMap();

	//for (auto &pair : m_Viewports)
	//{
	//	Viewport &viewport = pair.second;
	//	if (!viewport.Camera)
	//		continue;

	//	int x = viewport.Left * m_Width;
	//	int y = viewport.Top * m_Height;
	//	int width = (viewport.Right - viewport.Left) * m_Width;
	//	int height = (viewport.Bottom - viewport.Top) * m_Height;
	//	
	//	/*
	//	Base pass
	//	*/
	//	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_fbBasePass);
	//	glViewport(0, 0, m_Width, m_Height);

	//	// Clear G-buffer
	//	GLenum windowBuffClear[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
	//	glDrawBuffers(4, windowBuffClear);
	//	glClearColor(0.0f, 0.3f, 0.7f, 0.f);
	//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//	// Execute the first render stage which will fill out the internal buffers with data(??)
	//	m_FirstPassProgram.Bind();
	//	GLenum windowBuffOpaque[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
	//	glDrawBuffers(4, windowBuffOpaque);

	//	glCullFace(GL_BACK);
	//	
	//	DrawFBOScene(viewport);

	//	/*
	//	Lighting pass
	//	*/
	//	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_fbLightingPass);
	//	GLenum lightingPassAttachments[] = { GL_COLOR_ATTACHMENT0 };
	//	glDrawBuffers(1, lightingPassAttachments);

	//	glClearColor(0.f, 0.f, 0.f, 0.f);
	//	glClear(GL_COLOR_BUFFER_BIT);

	//	m_SecondPassProgram.Bind();
	//	glActiveTexture(GL_TEXTURE0);
	//	glBindTexture(GL_TEXTURE_2D, m_fPositionTexture);
	//	glActiveTexture(GL_TEXTURE1);
	//	glBindTexture(GL_TEXTURE_2D, m_fNormalsTexture);
	//	glActiveTexture(GL_TEXTURE2);
	//	glBindTexture(GL_TEXTURE_2D, m_fSpecularTexture);

	//	glCullFace(GL_FRONT);
	//	DrawLightScene(viewport);
	DrawSunLightScene();

	//	/*
	//	Final pass
	//	*/
	//	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	//	glViewport(x, y, width, height);
	//	glClear(GL_DEPTH_BUFFER_BIT);

	//	m_FinalPassProgram.Bind();

	//	// Ambient light
	//	glUniform3fv(glGetUniformLocation(m_FinalPassProgram.GetHandle(), "La"), 1, glm::value_ptr(glm::vec3(0.7f)));
	//	glUniform1f(glGetUniformLocation(m_FinalPassProgram.GetHandle(), "Gamma"), Gamma);

	//	glActiveTexture(GL_TEXTURE0);
	//	glBindTexture(GL_TEXTURE_2D, m_fDiffuseTexture);
	//	glActiveTexture(GL_TEXTURE1);
	//	glBindTexture(GL_TEXTURE_2D, m_fLightingTexture);

	//	glCullFace(GL_BACK);
	//	glBindVertexArray(m_ScreenQuad);
	//	glEnableVertexAttribArray(0);
	//	glDrawArrays(GL_TRIANGLES, 0, 6);
	//}
}

void Renderer::DrawFBO2(RenderQueue &rq)
{
}

void Renderer::DrawFBOScene(RenderQueue &rq)
{	
// 	glEnable(GL_DEPTH_TEST);//Tests where objects are and display them correctly
// 	glEnable(GL_CULL_FACE);	//removes triangles on the wrong side of the object
// 	glCullFace(GL_BACK);	//Make it so that only the back faces are rendered
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); //Draws filled polygons

	glm::mat4 cameraProjection = m_Camera->ProjectionMatrix((float)m_Viewport.Width / m_Viewport.Height);
	glm::mat4 cameraMatrix = cameraProjection * m_Camera->ViewMatrix();
	glm::mat4 MVP;
	glm::mat4 biasMatrix(
		0.5, 0.0, 0.0, 0.0,
		0.0, 0.5, 0.0, 0.0,
		0.0, 0.0, 0.5, 0.0,
		0.5, 0.5, 0.5, 1.0
		);

	//glm::mat4 depthViewMatrix = glm::lookAt(m_SunPosition, m_SunTarget, glm::vec3(0, 1, 0)) * glm::translate((-m_Camera->Position() + (glm::vec3(40.0) * -m_Camera->Forward())) * glm::vec3(1, 1, 1));
	glm::mat4 depthViewMatrix = glm::lookAt(m_SunPosition, m_SunTarget, glm::vec3(0, 1, 0)) * glm::translate(-m_Camera->Position() * glm::vec3(1, 1, 1));
	glm::mat4 depthCamera = m_SunProjection * depthViewMatrix;
	glm::mat4 depthCameraMatrix = biasMatrix * depthCamera;
	glm::mat4 depthMVP;

	glm::vec3 sunDirection = m_SunTarget - m_SunPosition;
	glm::vec3 sunDirection_cameraview = glm::vec3(cameraProjection * m_Camera->ViewMatrix() * glm::vec4(sunDirection, 1.0));

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_ShadowDepthTexture);

	for (auto &job : rq)
	{

		auto blendMapJob = std::dynamic_pointer_cast<BlendMapModelJob>(job);
		if(blendMapJob)
		{
			m_BlendMapProgram.Bind();
			GLuint ShaderProgramHandle = m_BlendMapProgram.GetHandle();

			glm::mat4 modelMatrix = blendMapJob->ModelMatrix;

			MVP = cameraMatrix * modelMatrix;
			depthMVP = depthCameraMatrix * modelMatrix;
			glUniformMatrix4fv(glGetUniformLocation(ShaderProgramHandle, "MVP"), 1, GL_FALSE, glm::value_ptr(MVP));
			glUniformMatrix4fv(glGetUniformLocation(ShaderProgramHandle, "DepthMVP"), 1, GL_FALSE, glm::value_ptr(depthMVP));
			glUniformMatrix4fv(glGetUniformLocation(ShaderProgramHandle, "M"), 1, GL_FALSE, glm::value_ptr(modelMatrix));
			glUniformMatrix4fv(glGetUniformLocation(ShaderProgramHandle, "V"), 1, GL_FALSE, glm::value_ptr(m_Camera->ViewMatrix()));
			glUniformMatrix4fv(glGetUniformLocation(ShaderProgramHandle, "P"), 1, GL_FALSE, glm::value_ptr(cameraProjection));
			glUniform3fv(glGetUniformLocation(ShaderProgramHandle, "SunDirection_cameraspace"), 1, glm::value_ptr(sunDirection_cameraview));
			glUniform1f(glGetUniformLocation(ShaderProgramHandle, "TextureRepeats"), blendMapJob->TextureRepeat);

			glBindVertexArray(blendMapJob->VAO);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, blendMapJob->DiffuseTexture);
			if (blendMapJob->NormalTexture != 0)
			{
				glActiveTexture(GL_TEXTURE2);
				glBindTexture(GL_TEXTURE_2D, blendMapJob->NormalTexture);
			}
			if (blendMapJob->SpecularTexture)
			{
				glActiveTexture(GL_TEXTURE3);
				glBindTexture(GL_TEXTURE_2D, blendMapJob->SpecularTexture);
			}

			glActiveTexture(GL_TEXTURE4);
			glBindTexture(GL_TEXTURE_2D, blendMapJob->BlendMapTextureRed);
			glActiveTexture(GL_TEXTURE5);
			glBindTexture(GL_TEXTURE_2D, blendMapJob->BlendMapTextureRedNormal);
			glActiveTexture(GL_TEXTURE6);
			glBindTexture(GL_TEXTURE_2D, blendMapJob->BlendMapTextureRedSpecular);
			glActiveTexture(GL_TEXTURE7);
			glBindTexture(GL_TEXTURE_2D, blendMapJob->BlendMapTextureGreen);
			glActiveTexture(GL_TEXTURE8);
			glBindTexture(GL_TEXTURE_2D, blendMapJob->BlendMapTextureGreenNormal);
			glActiveTexture(GL_TEXTURE9);
			glBindTexture(GL_TEXTURE_2D, blendMapJob->BlendMapTextureGreenSpecular);
			glActiveTexture(GL_TEXTURE10);
			glBindTexture(GL_TEXTURE_2D, blendMapJob->BlendMapTextureBlue);
			glActiveTexture(GL_TEXTURE11);
			glBindTexture(GL_TEXTURE_2D, blendMapJob->BlendMapTextureBlueNormal);
			glActiveTexture(GL_TEXTURE12);
			glBindTexture(GL_TEXTURE_2D, blendMapJob->BlendMapTextureBlueSpecular);

			glDrawArrays(GL_TRIANGLES, blendMapJob->StartIndex, blendMapJob->EndIndex - blendMapJob->StartIndex + 1);

			continue;
		}

		auto modelJob = std::dynamic_pointer_cast<ModelJob>(job);
		if (modelJob)
		{
			m_FirstPassProgram.Bind();
			GLuint ShaderProgramHandle = m_FirstPassProgram.GetHandle();

			glm::mat4 modelMatrix = modelJob->ModelMatrix;

			MVP = cameraMatrix * modelMatrix;
			depthMVP = depthCameraMatrix * modelMatrix;
			glUniformMatrix4fv(glGetUniformLocation(ShaderProgramHandle, "MVP"), 1, GL_FALSE, glm::value_ptr(MVP));
			glUniformMatrix4fv(glGetUniformLocation(ShaderProgramHandle, "DepthMVP"), 1, GL_FALSE, glm::value_ptr(depthMVP));
			glUniformMatrix4fv(glGetUniformLocation(ShaderProgramHandle, "M"), 1, GL_FALSE, glm::value_ptr(modelMatrix));
			glUniformMatrix4fv(glGetUniformLocation(ShaderProgramHandle, "V"), 1, GL_FALSE, glm::value_ptr(m_Camera->ViewMatrix()));
			glUniformMatrix4fv(glGetUniformLocation(ShaderProgramHandle, "P"), 1, GL_FALSE, glm::value_ptr(cameraProjection));
			glUniform3fv(glGetUniformLocation(ShaderProgramHandle, "SunDirection_cameraspace"), 1, glm::value_ptr(sunDirection_cameraview));

			glBindVertexArray(modelJob->VAO);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, modelJob->DiffuseTexture);
			if (modelJob->NormalTexture != 0)
			{
				glActiveTexture(GL_TEXTURE2);
				glBindTexture(GL_TEXTURE_2D, modelJob->NormalTexture);
			}
			if (modelJob->SpecularTexture)
			{
				glActiveTexture(GL_TEXTURE3);
				glBindTexture(GL_TEXTURE_2D, modelJob->SpecularTexture);
			}
			glDrawArrays(GL_TRIANGLES, modelJob->StartIndex, modelJob->EndIndex - modelJob->StartIndex + 1);

			continue;
		}
		
		//auto spriteJob = std::dynamic_pointer_cast<SpriteJob>(job);
		//if (spriteJob)
		//{
		//	Texture* texture;
		//	glm::mat4 modelMatrix;
		//	glm::mat4 billboardMatrix;
		//	std::tie(texture, modelMatrix, billboardMatrix) = tuple;

		//	//MVP = cameraMatrix * glm::inverse(glm::toMat4(m_Camera->Orientation()) * modelMatrix );
		//	MVP = cameraMatrix * modelMatrix * billboardMatrix;

		//	depthMVP = depthCameraMatrix * modelMatrix;
		//	glUniformMatrix4fv(glGetUniformLocation(m_FirstPassProgram.GetHandle(), "MVP"), 1, GL_FALSE, glm::value_ptr(MVP));
		//	glUniformMatrix4fv(glGetUniformLocation(m_FirstPassProgram.GetHandle(), "DepthMVP"), 1, GL_FALSE, glm::value_ptr(depthMVP));
		//	glUniformMatrix4fv(glGetUniformLocation(m_FirstPassProgram.GetHandle(), "M"), 1, GL_FALSE, glm::value_ptr(modelMatrix));
		//	glUniformMatrix4fv(glGetUniformLocation(m_FirstPassProgram.GetHandle(), "V"), 1, GL_FALSE, glm::value_ptr(m_Camera->ViewMatrix()));
		//	glUniformMatrix4fv(glGetUniformLocation(m_FirstPassProgram.GetHandle(), "P"), 1, GL_FALSE, glm::value_ptr(m_Camera->ProjectionMatrix((float)m_Width / m_Height)));

		//	glActiveTexture(GL_TEXTURE0);
		//	glBindTexture(GL_TEXTURE_2D, *texture);
		//	glBindVertexArray(m_ScreenQuad);
		//	glDrawArrays(GL_TRIANGLES, 0, 6);

		//	continue;
		//}
	}
}

void Renderer::DrawLightScene(RenderQueue &rq)
{
	glEnable(GL_BLEND);
	glBlendEquation (GL_FUNC_ADD);
	glBlendFunc(GL_ONE,GL_ONE);

	glDisable (GL_DEPTH_TEST);
	glDepthMask (GL_FALSE);
	glBindVertexArray(m_sphereModel->VAO);

	glm::mat4 cameraProjection = m_Camera->ProjectionMatrix((float)m_Viewport.Width / m_Viewport.Height);
	glm::mat4 cameraMatrix = cameraProjection * m_Camera->ViewMatrix();
	glm::mat4 MVP;
	glm::vec3 sunDirection = m_SunTarget - m_SunPosition;

	m_SecondPassProgram.Bind();
	GLuint ShaderProgramHandle = m_SecondPassProgram.GetHandle();

	for (auto &light : Lights)
	{
		MVP = cameraMatrix * light.SphereModelMatrix;

		glUniform2fv(glGetUniformLocation(ShaderProgramHandle, "ViewportSize"), 1,glm::value_ptr(glm::vec2(m_Width, m_Height)));
		glUniformMatrix4fv(glGetUniformLocation(ShaderProgramHandle, "MVP"), 1, GL_FALSE, glm::value_ptr(MVP));
		glUniformMatrix4fv(glGetUniformLocation(ShaderProgramHandle, "V"), 1, GL_FALSE, glm::value_ptr(m_Camera->ViewMatrix()));
		glUniformMatrix4fv(glGetUniformLocation(ShaderProgramHandle, "P"), 1, GL_FALSE, glm::value_ptr(cameraProjection));
		glUniformMatrix4fv(glGetUniformLocation(ShaderProgramHandle, "M"), 1, GL_FALSE, glm::value_ptr(light.SphereModelMatrix));
		glUniform3fv(glGetUniformLocation(ShaderProgramHandle, "ls"), 1, glm::value_ptr(light.Specular));
		glUniform3fv(glGetUniformLocation(ShaderProgramHandle, "ld"), 1, glm::value_ptr(light.Diffuse));
		glUniform3fv(glGetUniformLocation(ShaderProgramHandle, "lp"), 1, glm::value_ptr(light.Position));
		glUniform3f(glGetUniformLocation(ShaderProgramHandle, "CameraPosition"), m_Camera->Position().x, m_Camera->Position().y, m_Camera->Position().z);
		glUniform1f(glGetUniformLocation(ShaderProgramHandle, "specularExponent"), light.SpecularExponent);
		//glUniform1f(glGetUniformLocation(ShaderProgramHandle, "ConstantAttenuation"), CAtt);
		//glUniform1f(glGetUniformLocation(ShaderProgramHandle, "LinearAttenuation"), LAtt);
		//glUniform1f(glGetUniformLocation(ShaderProgramHandle, "QuadraticAttenuation"), QAtt);
		glUniform1f(glGetUniformLocation(ShaderProgramHandle, "LightRadius"), light.Radius);
		glUniform3fv(glGetUniformLocation(ShaderProgramHandle, "directionToSun"), 1, glm::value_ptr(-sunDirection));

		glDrawArrays(GL_TRIANGLES, 0, m_sphereModel->Vertices.size());
	};
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glDisable(GL_BLEND);
}

void Renderer::DrawSunLightScene()
{
	glCullFace(GL_BACK);

	glEnable(GL_BLEND);
	glBlendEquation(GL_FUNC_ADD);
	glBlendFunc(GL_ONE,GL_ONE);

	glDisable (GL_DEPTH_TEST);
	glDepthMask (GL_FALSE);
	//glBindVertexArray(m_sphereModel->VAO);

	glm::mat4 cameraProjection = m_Camera->ProjectionMatrix((float)m_Viewport.Width / m_Viewport.Height);
	glm::mat4 cameraMatrix = cameraProjection * m_Camera->ViewMatrix();
	glm::mat4 MVP;

	m_SunPassProgram.Bind();
	GLuint ShaderProgramHandle = m_SunPassProgram.GetHandle();
	

	glUniform2fv(glGetUniformLocation(ShaderProgramHandle, "ViewportSize"), 1, glm::value_ptr(glm::vec2(m_Width, m_Height)));
	glUniformMatrix4fv(glGetUniformLocation(ShaderProgramHandle, "MVP"), 1, GL_FALSE, glm::value_ptr(MVP));
	glUniformMatrix4fv(glGetUniformLocation(ShaderProgramHandle, "V"), 1, GL_FALSE, glm::value_ptr(m_Camera->ViewMatrix()));
	glUniformMatrix4fv(glGetUniformLocation(ShaderProgramHandle, "P"), 1, GL_FALSE, glm::value_ptr(cameraProjection));
	glUniform3f(glGetUniformLocation(ShaderProgramHandle, "CameraPosition"), m_Camera->Position().x, m_Camera->Position().y, m_Camera->Position().z);
	glUniform3fv(glGetUniformLocation(ShaderProgramHandle, "directionToSun"), 1, glm::value_ptr(glm::normalize(m_SunPosition)));

	glBindVertexArray(m_ScreenQuad);
	glEnableVertexAttribArray(0);
	glDrawArrays(GL_TRIANGLES, 0, 6);

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
// 	float c = _light.ConstantAttenuation;
// 	float l = _light.LinearAttenuation;
// 	float q = _light.QuadraticAttenuation;
	//float c = CAtt;
	//float l = LAtt;
	//float q = QAtt;
	//float cutOffRadius = abs(sqrt((-4*c*q) + pow(l, 2) + (1024*q) - l) / (2*q));

	glm::mat4 model;
	model *= glm::translate(_light.Position);
	model *= glm::scale(glm::vec3(_light.Radius*2));
	return model;
}

void Renderer::UpdateSunProjection()
{
	glm::vec3 NDCCube[] =
	{
		glm::vec3(-1.f, -1.f, -1.f), 
		glm::vec3(1.f, -1.f, -1.f), 
		glm::vec3(-1.f, 1.f, -1.f), 
		glm::vec3(1.f, 1.f, -1.f), 
		glm::vec3(-1.f, -1.f, 1.f),
		glm::vec3(1.f, -1.f, 1.f), 
		glm::vec3(-1.f, 1.f, 1.f), 
		glm::vec3(1.f, 1.f, 1.f)
	};

	glm::mat4 inverseProjectionViewMatrix = glm::inverse(m_Camera->ViewMatrix()) * glm::inverse(m_Camera->ProjectionMatrix((float)m_Width / m_Height));
	//Also * with world matrix for light

	for(auto corner : NDCCube)
	{
		//corner *= inverseProjectionViewMatrix;
	}

	//Calculate the bounding box of the transformed frustum corners. This will be the view frustum for the shadow map.

	//Pass the bounding box's extents to glOrtho or similar to set up the orthographic projection matrix for the shadow map.
}

void Renderer::RegisterCamera(int identifier, float FOV, float nearClip, float farClip)
{
	m_Cameras[identifier] = std::make_shared<Camera>(FOV, nearClip, farClip);
}

void Renderer::UpdateViewport(int viewportIdentifier, int cameraIdentifier)
{
	//auto &viewport = m_Viewports[viewportIdentifier];
	//auto camera = m_Cameras[cameraIdentifier];
	//camera->AspectRatio(((viewport.Right - viewport.Left) * m_Width) / ((viewport.Bottom - viewport.Top) * m_Height));
	//viewport.Camera = camera;
}

void Renderer::UpdateCamera(int cameraIdentifier, glm::vec3 position, glm::quat orientation, float FOV, float nearClip, float farClip)
{
	/*m_Cameras[cameraIdentifier]->Position(position);
	m_Cameras[cameraIdentifier]->Orientation(orientation);
	m_Cameras[cameraIdentifier]->FOV(FOV);
	m_Cameras[cameraIdentifier]->NearClip(nearClip);
	m_Cameras[cameraIdentifier]->FarClip(farClip);*/
}

void Renderer::ClearPointLights()
{
	Lights.clear();
}
