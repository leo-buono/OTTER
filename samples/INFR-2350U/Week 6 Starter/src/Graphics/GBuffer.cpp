#include "GBuffer.h"

void GBuffer::Init(unsigned width, unsigned height)
{
	_windowWidth = width;
	_windowHeight = height;

	_gBuffer.AddColorTarget(GL_RGBA8); // albedo buffer
	_gBuffer.AddColorTarget(GL_RGB8); // normal buffer
	_gBuffer.AddColorTarget(GL_RGB8); //spec buffer

	//important note, get pos Data with depth buffer
	//But using position buffer
	_gBuffer.AddColorTarget(GL_RGB32F);

	//add depth buffer
	_gBuffer.AddDepthTarget();

	_gBuffer.Init(width, height);

	_passThrough = Shader::Create();
	_passThrough->LoadShaderPartFromFile("shaders/passthrough_vert.glsl", GL_VERTEX_SHADER);
	_passThrough->LoadShaderPartFromFile("shaders/passthrough_frag.glsl", GL_FRAGMENT_SHADER);
	_passThrough->Link();


}

void GBuffer::Bind()
{
	_gBuffer.Bind();
}

void GBuffer::BindLighting()
{
	_gBuffer.BindColorAsTexture(Target::ALBEDO, 0);
	_gBuffer.BindColorAsTexture(Target::NORMAL, 1);
	_gBuffer.BindColorAsTexture(Target::SPECULAR, 2);
	_gBuffer.BindColorAsTexture(Target::POSITION, 3);
}

void GBuffer::Clear()
{
	_gBuffer.Clear();
}

void GBuffer::Unbind()
{
	_gBuffer.Unbind();
}

void GBuffer::UnbindLighting()
{
	_gBuffer.UnbindTexture(0);
	_gBuffer.UnbindTexture(1);
	_gBuffer.UnbindTexture(2);
	_gBuffer.UnbindTexture(3);
}

void GBuffer::DrawBuffersToScreen()
{
	_passThrough->Bind();

	glViewport(0, _windowHeight / 2.f, _windowWidth / 2.f, _windowHeight / 2.f);
	_gBuffer.BindColorAsTexture(Target::ALBEDO, 0);
	_gBuffer.DrawFullscreenQuad();
	_gBuffer.UnbindTexture(0);

	glViewport(_windowWidth / 2.f, _windowHeight / 2.f, _windowWidth / 2.f, _windowHeight / 2.f);
	_gBuffer.BindColorAsTexture(Target::NORMAL, 0);
	_gBuffer.DrawFullscreenQuad();
	_gBuffer.UnbindTexture(0);

	glViewport(0, 0, _windowWidth / 2.f, _windowHeight / 2.f);
	_gBuffer.BindColorAsTexture(Target::SPECULAR, 0);
	_gBuffer.DrawFullscreenQuad();
	_gBuffer.UnbindTexture(0);

	glViewport(_windowWidth / 2.f, 0, _windowWidth / 2.f, _windowHeight / 2.f);
	_gBuffer.BindColorAsTexture(Target::POSITION, 0);
	_gBuffer.DrawFullscreenQuad();
	_gBuffer.UnbindTexture(0);

	//unbind passthrough
	_passThrough->UnBind();
}

void GBuffer::Reshape(unsigned width, unsigned height)
{
	_windowWidth = width;
	_windowHeight = height;

	_gBuffer.Reshape(width, height);
}
