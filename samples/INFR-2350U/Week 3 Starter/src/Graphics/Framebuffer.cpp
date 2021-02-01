#include "Framebuffer.h"

GLuint Framebuffer::_fullScreenQuadVAO = 0;
GLuint Framebuffer::_fulllscreenQuadVBO = 0;

int Framebuffer::_maxColorAttachments = 0;
bool Framebuffer::_isInitFSQ = false;

DepthTarget::~DepthTarget()
{
	//unloads the depth target
	Unload();
}

void DepthTarget::Unload()
{
	//deletes the texture at the specific handle
	glDeleteTextures(1, &_texture.GetHandle());
}

ColorTarget::~ColorTarget()
{
	Unload();
}

void ColorTarget::Unload()
{
	glDeleteTextures(_numAttachments, &_textures[0].GetHandle());
;}

Framebuffer::Framebuffer()
{
	Unload();
}

Framebuffer::~Framebuffer()
{

}

void Framebuffer::Unload()
{
	glDeleteFramebuffers(1, &_FBO);

	_isInit = false;
}

void Framebuffer::Init(unsigned width, unsigned height)
{
	SetSize(width, height);
	Init();
}

void Framebuffer::Init()
{
	glGenFramebuffers(1, &_FBO);

	glBindBuffer(GL_FRAMEBUFFER, _FBO);

	if (_depthActive)
	{
		//becuse we have depth we need to clear our depth bit
		_clearflag |= GL_DEPTH_BUFFER_BIT;

		glGenTextures(1, &_depth._texture.GetHandle());

		glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH_COMPONENT24, _width, _height);

		glTextureParameteri(_depth._texture.GetHandle(), GL_TEXTURE_MIN_FILTER, _filter);
		glTextureParameteri(_depth._texture.GetHandle(), GL_TEXTURE_MAG_FILTER, _filter);
		glTextureParameteri(_depth._texture.GetHandle(), GL_TEXTURE_WRAP_S, _wrap);
		glTextureParameteri(_depth._texture.GetHandle(), GL_TEXTURE_WRAP_T, _wrap);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, _depth._texture.GetHandle(), 0);

		glBindTexture(GL_TEXTURE_2D, GL_NONE);
	}
	if (_color._numAttachments)
	{
		//Because we have a color target we inclide a color buffer bit into clear flag
		_clearflag |= GL_COLOR_BUFFER_BIT;

		GLuint* textureHandles = new GLuint[_color._numAttachments];


		glGenTextures(_color._numAttachments, textureHandles);

		//loops through them
		for (unsigned i = 0; i < _color._numAttachments; i++)
		{
			_color._textures[i].GetHandle() = textureHandles[i];
			//binds the texture 
			glBindTexture(GL_TEXTURE_2D, _color._textures[i].GetHandle());
			//sets the texture storage
			glTexStorage2D(GL_TEXTURE_2D, 1, _color._formats[i], _width, _height);

			glTextureParameteri(_color._textures[i].GetHandle(), GL_TEXTURE_MIN_FILTER, _filter);
			glTextureParameteri(_color._textures[i].GetHandle(), GL_TEXTURE_MAG_FILTER, _filter);
			glTextureParameteri(_color._textures[i].GetHandle(), GL_TEXTURE_WRAP_S, _wrap);
			glTextureParameteri(_color._textures[i].GetHandle(), GL_TEXTURE_WRAP_T, _wrap);

			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, _color._textures[i].GetHandle(), 0);


		}
		delete[] textureHandles;
	}
	CheckFBO();
	glBindFramebuffer(GL_FRAMEBUFFER, GL_NONE);

	_isInit = true;
	//glBindTexture(GL_TEXTURE_2D, GL_NONE);
}

void Framebuffer::AddDepthTarget()
{
	if (_depth._texture.GetHandle())
	{
		_depth.Unload();
	}
	//make depth active true
	_depthActive = true;
}

void Framebuffer::AddColorTarget(GLenum format)
{
	_color._textures.resize(_color._numAttachments + 1);

	_color._formats.push_back(format);

	_color._buffers.push_back(GL_COLOR_ATTACHMENT0 + _color._numAttachments);

	_color._numAttachments++;
}

void Framebuffer::BindDepthTexture(int textureSlot) const
{
	_depth._texture.Bind(textureSlot);
}

void Framebuffer::BindColorAsTexture(unsigned colorBuffer, int textureSlot) const
{
	_color._textures[colorBuffer].Bind(textureSlot);
}

void Framebuffer::UnbindTexture(int textureSlot) const
{
	glActiveTexture(GL_TEXTURE0 + textureSlot);
	glBindTexture(GL_TEXTURE_2D, GL_NONE);
}

void Framebuffer::Reshape(unsigned width, unsigned height)
{
	SetSize(width, height);

	Unload();

	_depth.Unload();

	_color.Unload();

	Init();
}

void Framebuffer::SetSize(unsigned width, unsigned height)
{
	_width = width;
	_height = height;
}

void Framebuffer::SetViewport() const
{
	glViewport(0, 0, _width, _height);
}

void Framebuffer::Bind() const
{
	glBindFramebuffer(GL_FRAMEBUFFER, _FBO);
	if (_color._numAttachments)
	{
		glDrawBuffers(_color._numAttachments, &_color._buffers[0]);
	}

	}

void Framebuffer::Unbind() const
{
	glBindFramebuffer(GL_FRAMEBUFFER, GL_NONE);
}

void Framebuffer::rendertoFSQ() const
{
	SetViewport();
	Bind();
	DrawFullscreenQuad();
	Unbind();
}

void Framebuffer::DrawToBackbuffer()
{
	glBindFramebuffer(GL_READ_FRAMEBUFFER, _FBO);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, GL_NONE);


	glBlitFramebuffer(0, 0, _width, _height, 0, 0, _width, _height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
	glBindFramebuffer(GL_READ_FRAMEBUFFER, GL_NONE);
}

void Framebuffer::Clear()
{
	glBindFramebuffer(GL_FRAMEBUFFER, _FBO);
	glClear(_clearflag);
	glBindFramebuffer(GL_FRAMEBUFFER, GL_NONE);
}

void Framebuffer::CheckFBO()
{
	Bind();
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		printf("Framebuffer is not working\n");
	}
	//need to complete***
}

void Framebuffer::InitFullscreenQuad()
{
	//A vbo with Uvs and verts from
	//-1 to 1 for verts
	//0 to 1 for UVs
	float VBO_DATA[]
	{
		-1.f, -1.f, 0.f,
		1.f, -1.f, 0.f,
		-1.f, 1.f, 0.f,

		1.f, 1.f, 0.f,
		-1.f, 1.f, 0.f,
		1.f, -1.f, 0.f,

		0.f, 0.f,
		1.f, 0.f,
		0.f, 1.f,

		1.f, 1.f,
		0.f, 1.f,
		1.f, 0.f
	};
	//Vertex size is 6pts * 3 data points * sizeof (float)
	int vertexSize = 6 * 3 * sizeof(float);
	//texcoord size = 6pts * 2 data points * sizeof(float)
	int texCoordSize = 6 * 2 * sizeof(float);

	//Generates vertex array
	glGenVertexArrays(1, &_fulllscreenQuadVBO);
	//Binds VAO
	glBindVertexArray(_fullScreenQuadVAO);

	//Enables 2 vertex attrib array slots
	glEnableVertexAttribArray(0); //Vertices
	glEnableVertexAttribArray(1); //UVS

	//Generates VBO
	glGenBuffers(1, &_fulllscreenQuadVBO);

	//Binds the VBO
	glBindBuffer(GL_ARRAY_BUFFER, _fulllscreenQuadVBO);
	//Buffers the vbo data
	glBufferData(GL_ARRAY_BUFFER, vertexSize + texCoordSize, VBO_DATA, GL_STATIC_DRAW);

#pragma warning(push)
#pragma warning(disable : 4312)
	//Sets first attrib array to point to the beginning of the data
	glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, reinterpret_cast<void*>(0));
	//Sets the second attrib array to point to an offset in the data
	glVertexAttribPointer((GLuint)1, 2, GL_FLOAT, GL_FALSE, 0, reinterpret_cast<void*>(vertexSize));
#pragma warning(pop)

	glBindBuffer(GL_ARRAY_BUFFER, GL_NONE);
	glBindVertexArray(GL_NONE);
}

void Framebuffer::DrawFullscreenQuad()
{
	glBindVertexArray(_fulllscreenQuadVBO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(GL_NONE);
}



