#pragma once
#include <vector>
#include <Texture2D.h>
#include <Shader.h>

struct DepthTarget
{
	//Deconstructor for Depth Target
	//*Unloads texture
	~DepthTarget();

	void Unload();
	Texture2D _texture;

};

struct ColorTarget
{
	//Deconstructor for Color Target
	//*Unloads all the color targets
	~ColorTarget();
	//deletes the texture of the colour targets	
	void Unload();
	//Holds the colour textures
	std::vector<Texture2D> _textures;
	std::vector<GLenum> _formats;
	std::vector<GLenum> _buffers;

	unsigned int _numAttachments = 0;
};

class Framebuffer
{
public:
	Framebuffer();
	~Framebuffer();
	//unloads the framebuffer
	void Unload();

	//Initialize framebuffer with size
	virtual void Init(unsigned width, unsigned height);
	//Initializes framebuffer
	void Init();

	//adds depth target
	//ONLY EVER ONE
	void AddDepthTarget();

	//adds colour target
	//can have as many as you want depends on GPU
	void AddColorTarget(GLenum format);

	//beinds our depth buffer as a texture to a specified slot
	void BindDepthTexture(int textureSlot) const;
	//binds our cololr buffer as a textrure to a specifc slot
	void BindColorAsTexture(unsigned colorBuffer, int textureSlot) const;
	//unbids texture from a specific slot
	void UnbindTexture(int textureSlot) const;
	//reshapes the framebuffer
	void Reshape(unsigned width, unsigned height);
	//size of framebuffer
	void SetSize(unsigned width, unsigned height);

	void SetViewport() const;
	//binds framebuffer
	void Bind() const;
	//unbinds framebuffer
	void Unbind() const;

	void rendertoFSQ() const;

	void DrawToBackbuffer();

	void Clear();

	void CheckFBO();

	//initilisazes fullscreen quad
	//creates VAO for full screen quad
	//covers -1 to 1 range
	static void InitFullscreenQuad();
	//Draws our fullscreen quad
	static void DrawFullscreenQuad();

	//Initial wisth and height is zero
	unsigned int _width = 0;
	unsigned int _height = 0;


protected:
	//OpenGL framebudder handle
	GLuint _FBO;
	//Depth attachment (either one or none)
	DepthTarget _depth;
	//color attachments (can be either 1 or above)
	ColorTarget _color;

	//Default filter is GL_NEAREST;
	GLenum _filter = GL_NEAREST;
	//Default filter is GL_CLAP_TO_FACE
	GLenum _wrap = GL_CLAMP_TO_EDGE;

	//Clearflag is nothing by default
	GLbitfield _clearflag = 0;

	bool _isInit = false;

	bool _depthActive = false;

	static GLuint _fulllscreenQuadVBO;
	//full screen quad VAO handle
	static GLuint _fullScreenQuadVAO;

	//The maximum amount of color attachments
	static int _maxColorAttachments;

	static bool _isInitFSQ;

};