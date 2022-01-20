#include "stdafx.h"

#include "MeshData.h"

//----------------------------------------------------------
MeshData::MeshData()
:	m_vertices(nullptr)
,	m_VBO(0)
,	m_VAO(0)
{
}

//----------------------------------------------------------
MeshData::~MeshData()
{
	if (m_vertices)
		delete m_vertices;
}

//----------------------------------------------------------
void	MeshData::SetVertices(float *vertices, uint componentNbr, uint size, uint stride, uint mode)
{
	m_size = size;
	m_stride = stride;
	m_mode = mode;
	m_verticeNbr = componentNbr / size;

	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &m_VBO);

	glBindVertexArray(m_VAO);
	// bind, 1 possible in the same time / buffer type
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

	// copies data in the buffers memory
	// GL_STATIC_DRAW: the data will most likely not change at all or very rarely.
	// GL_DYNAMIC_DRAW : the data is likely to change a lot.
	// GL_STREAM_DRAW : the data will change every time it is drawn.
	glBufferData(GL_ARRAY_BUFFER, componentNbr * sizeof(float), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, m_size, GL_FLOAT, GL_FALSE, m_stride, (void*)0);

	glEnableVertexAttribArray(0);
	glBindVertexArray(0);

	m_vertices = nullptr;
}

//----------------------------------------------------------
uint	MeshData::Mode()
{
	return m_mode;
}

//----------------------------------------------------------
