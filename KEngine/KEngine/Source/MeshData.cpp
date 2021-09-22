#include "stdafx.h"

#include "MeshData.h"

//----------------------------------------------------------
MeshData::MeshData()
:	m_Vertices(nullptr)
,	m_VBO(0)
,	m_VAO(0)
{
}

//----------------------------------------------------------
MeshData::~MeshData()
{
	if (m_Vertices)
		delete m_Vertices;
}

//----------------------------------------------------------
void	MeshData::SetVertices(float *vertices, uint componentNbr, uint size, uint stride, uint mode)
{
	m_Vertices = (float*)malloc(componentNbr * sizeof(float));
	std::memcpy(m_Vertices, vertices, componentNbr * sizeof(float)); // to remove, vertices arg is enought
	m_Size = size;
	m_Stride = stride;
	m_Mode = mode;
	m_VerticeNbr = componentNbr / size;

	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &m_VBO);

	glBindVertexArray(m_VAO);
	// bind, 1 possible in the same time / buffer type
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

	// copies data in the buffers memory
	// GL_STATIC_DRAW: the data will most likely not change at all or very rarely.
	// GL_DYNAMIC_DRAW : the data is likely to change a lot.
	// GL_STREAM_DRAW : the data will change every time it is drawn.
	glBufferData(GL_ARRAY_BUFFER, componentNbr * sizeof(float), m_Vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, m_Size, GL_FLOAT, GL_FALSE, m_Stride, (void*)0);

	glEnableVertexAttribArray(0);
	glBindVertexArray(0);

	delete m_Vertices;
	m_Vertices = nullptr;
}

//----------------------------------------------------------
uint	MeshData::Mode()
{
	return m_Mode;
}

//----------------------------------------------------------
