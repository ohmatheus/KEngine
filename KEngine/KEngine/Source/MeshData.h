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

	uint	VerticesNbr() { return m_VerticeNbr; }

//protected:
	uint	m_VBO;
	uint	m_VAO;
	uint	m_Size;		// Specifies the number of components per generic vertex attribute.
	uint	m_Stride;	// Specifies the byte offset between consecutive generic vertex attributes.
	uint	m_Mode;		// Specifies what kind of primitives to render.
	uint	m_VerticeNbr;
	float	*m_Vertices;
};

//----------------------------------------------------------
