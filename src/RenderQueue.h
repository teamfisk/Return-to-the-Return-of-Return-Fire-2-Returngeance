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

	unsigned int Layer;
	unsigned int ViewportID;
	unsigned int TextureID;

	GLuint DiffuseTexture;
	GLuint NormalTexture;
	GLuint SpecularTexture;
	GLuint VAO;
	unsigned int StartIndex;
	unsigned int EndIndex;
	glm::mat4 ModelMatrix;

protected:
	uint64_t Hash;

	void CalculateHash()
	{
		Hash = ViewportID << 58 // 6 bits 
			 | TextureID << 42; // 16 bits
	}

	bool operator<(const RenderJob& rhs)
	{
		return this->Hash < rhs.Hash;
	}
};

class RenderQueue
{
public:
	void Add(RenderJob &job)
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
	std::forward_list<RenderJob> m_Jobs;
};

#endif // RenderQueue_h__
