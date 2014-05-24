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
	template <typename JobType>
	friend class RenderQueue<JobType>;

protected:
	uint64_t Hash;

	virtual void CalculateHash() = 0;
	{
		Hash = ViewportID << 58 // 6 bits 
			 | TextureID << 42; // 16 bits
	}

	bool operator<(const RenderJob& rhs)
	{
		return this->Hash < rhs.Hash;
	}
};

struct ModelJob : RenderJob
{
	unsigned int TextureID;
	GLuint DiffuseTexture;
	GLuint NormalTexture;
	GLuint SpecularTexture;
	GLuint VAO;
	unsigned int StartIndex;
	unsigned int EndIndex;
	glm::mat4 ModelMatrix;

	void CalculateHash() override
	{
		Hash = TextureID;
	}
};

struct SpriteJob : RenderJob
{
	unsigned int TextureID;
	GLuint Texture;
	glm::mat4 ModelMatrix;

	void CalculateHash() override
	{
		Hash = TextureID;
	}
};

template <typename JobType>
class RenderQueue
{
public:
	void Add(JobType &job)
	{
		job.CalculateHash();
		m_Jobs.push_front(job);
		m_Jobs.sort();
	}

	void Clear()
	{
		m_Jobs.clear();
	}

private:
	std::forward_list<JobType> m_Jobs;
};

#endif // RenderQueue_h__
