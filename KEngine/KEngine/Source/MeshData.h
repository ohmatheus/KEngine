#pragma once

//----------------------------------------------------------
class MeshData final
{
public:
	MeshData();
	~MeshData();

	uint	VAO() { return m_VAO; }
	uint	VBO() { return m_VBO; }
	uint	Mode();

	void	SetVertices(float *vertices, uint componentNbr, uint size, uint stride, uint mode);

	uint	VerticesNbr() { return m_verticeNbr; }

//protected:
	uint	m_VBO;
	uint	m_VAO;
	uint	m_size;		// Specifies the number of components per generic vertex attribute.
	uint	m_stride;	// Specifies the byte offset between consecutive generic vertex attributes.
	uint	m_mode;
	uint	m_verticeNbr;
	float	*m_vertices;
};

//----------------------------------------------------------
