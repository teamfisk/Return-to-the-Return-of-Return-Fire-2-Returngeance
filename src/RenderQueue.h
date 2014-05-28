#ifndef RenderQueue_h__
#define RenderQueue_h__

#include <cstdint>
#include <forward_list>

#include "ResourceManager.h"
#include "Texture.h"
#include "Model.h"

class RenderQueue;

struct RenderJob
{
	friend class RenderQueue;

protected:
	uint64_t Hash;

	virtual void CalculateHash() = 0;

	bool operator<(const RenderJob& rhs)
	{
		return this->Hash < rhs.Hash;
	}
};

struct ModelJob : RenderJob
{
	unsigned int ShaderID;
	unsigned int TextureID;

	GLuint DiffuseTexture;
	GLuint NormalTexture;
	GLuint SpecularTexture;
	glm::vec4 Color;
	GLuint VAO;
	unsigned int StartIndex;
	unsigned int EndIndex;
	glm::mat4 ModelMatrix;

	void CalculateHash() override
	{
		Hash = TextureID;
	}
};

struct BlendMapModelJob : ModelJob
{
	GLuint BlendMapTextureRed;
	GLuint BlendMapTextureGreen;
	GLuint BlendMapTextureBlue;
	float TextureRepeat;
};

struct SpriteJob : RenderJob
{
	unsigned int ShaderID;
	unsigned int TextureID;

	GLuint Texture;
	glm::vec4 Color;
	glm::mat4 ModelMatrix;

	void CalculateHash() override
	{
		Hash = TextureID;
	}
};

class RenderQueue
{
public:
	template <typename T>
	void Add(T &job)
	{
		job.CalculateHash();
		m_Jobs.push_front(std::shared_ptr<T>(new T(job)));
		m_Jobs.sort();
	}

	void Clear()
	{
		m_Jobs.clear();
	}

	std::forward_list<std::shared_ptr<RenderJob>>::const_iterator begin()
	{
		return m_Jobs.begin();
	}

	std::forward_list<std::shared_ptr<RenderJob>>::const_iterator end()
	{
		return m_Jobs.end();
	}

private:
	std::forward_list<std::shared_ptr<RenderJob>> m_Jobs;
};

#endif // RenderQueue_h__
