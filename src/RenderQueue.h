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

	glm::mat4 ModelMatrix;
	float Depth;

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
	float Transparent;

	void CalculateHash() override
	{
		Hash = TextureID;
	}
};

struct BlendMapModelJob : ModelJob
{
	GLuint BlendMapTextureRed;
	GLuint BlendMapTextureRedNormal;
	GLuint BlendMapTextureRedSpecular;
	GLuint BlendMapTextureGreen;
	GLuint BlendMapTextureGreenNormal;
	GLuint BlendMapTextureGreenSpecular;
	GLuint BlendMapTextureBlue;
	GLuint BlendMapTextureBlueNormal;
	GLuint BlendMapTextureBlueSpecular;
	float TextureRepeat;
};

struct SpriteJob : RenderJob
{
	unsigned int ShaderID;
	unsigned int TextureID;

	GLuint Texture;
	glm::vec4 Color;

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
		Jobs.push_front(std::shared_ptr<T>(new T(job)));
	}

	void Sort()
	{
		Jobs.sort();
	}

	void Clear()
	{
		Jobs.clear();
	}

	std::forward_list<std::shared_ptr<RenderJob>>::const_iterator begin()
	{
		return Jobs.begin();
	}

	std::forward_list<std::shared_ptr<RenderJob>>::const_iterator end()
	{
		return Jobs.end();
	}

	std::forward_list<std::shared_ptr<RenderJob>> Jobs;
};

struct RenderQueuePair
{
	RenderQueue Deferred;
	RenderQueue Forward;

	void Clear()
	{
		Deferred.Clear();
		Forward.Clear();
	}

	void Sort()
	{
		Deferred.Sort();
		Forward.Sort();
	}
};

#endif // RenderQueue_h__
