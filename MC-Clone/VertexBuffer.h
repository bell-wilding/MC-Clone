#pragma once

class VertexBuffer {
public:
	VertexBuffer() {};
	VertexBuffer(const void* data, unsigned int size);
	~VertexBuffer();

	void BufferData(const void* data, unsigned int size) const;
	void Bind() const;
	void Unbind() const;

private:
	unsigned int m_RendererID;
};