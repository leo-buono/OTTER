#include "BloomEffect.h"

void BloomEffect::Init(unsigned width, unsigned height)
{
    int index = int(_buffers.size());
    _buffers.push_back(new Framebuffer());
    _buffers[index]->AddColorTarget(GL_RGBA8);
    _buffers[index]->AddDepthTarget();
    _buffers[index]->Init(width, height);

    //Loads the shaders 
    index = int(_shaders.size());
    _shaders.push_back(Shader::Create());
    _shaders[index]->LoadShaderPartFromFile("shaders/passthrough_vert.glsl", GL_VERTEX_SHADER);
    _shaders[index]->LoadShaderPartFromFile("shaders/passthrough_frag.glsl", GL_FRAGMENT_SHADER);
    _shaders[index]->Link();

    index = int(_buffers.size());
    _buffers.push_back(new Framebuffer());
    _buffers[index]->AddColorTarget(GL_RGBA8);
    _buffers[index]->AddDepthTarget();
    _buffers[index]->Init(width, height);

    //Loads the shaders
    index = int(_shaders.size());
    _shaders.push_back(Shader::Create());
    _shaders[index]->LoadShaderPartFromFile("shaders/passthrough_vert.glsl", GL_VERTEX_SHADER);
    _shaders[index]->LoadShaderPartFromFile("shaders/Post/bloom_frag.glsl", GL_FRAGMENT_SHADER);
    _shaders[index]->Link();

   index = int(_buffers.size());
   _buffers.push_back(new Framebuffer());
   _buffers[index]->AddColorTarget(GL_RGBA8);
   _buffers[index]->AddDepthTarget();
   _buffers[index]->Init(width, height);
   
   //Loads the shaders
   index = int(_shaders.size());
   _shaders.push_back(Shader::Create());
   _shaders[index]->LoadShaderPartFromFile("shaders/passthrough_vert.glsl", GL_VERTEX_SHADER);
   _shaders[index]->LoadShaderPartFromFile("shaders/Post/blur_fragH.glsl", GL_FRAGMENT_SHADER); 
   _shaders[index]->Link();

   index = int(_buffers.size());
   _buffers.push_back(new Framebuffer());
   _buffers[index]->AddColorTarget(GL_RGBA8);
   _buffers[index]->AddDepthTarget();
   _buffers[index]->Init(width, height);

   //Loads the shaders
   index = int(_shaders.size());
   _shaders.push_back(Shader::Create());
   _shaders[index]->LoadShaderPartFromFile("shaders/passthrough_vert.glsl", GL_VERTEX_SHADER);
   _shaders[index]->LoadShaderPartFromFile("shaders/Post/blur_fragV.glsl", GL_FRAGMENT_SHADER);
   _shaders[index]->Link();
   
   index = int(_buffers.size());
   _buffers.push_back(new Framebuffer());
   _buffers[index]->AddColorTarget(GL_RGBA8);
   _buffers[index]->AddDepthTarget();
   _buffers[index]->Init(width, height); 

   //Loads the shaders
   index = int(_shaders.size()); 
   _shaders.push_back(Shader::Create());    
   _shaders[index]->LoadShaderPartFromFile("shaders/passthrough_vert.glsl", GL_VERTEX_SHADER);
   _shaders[index]->LoadShaderPartFromFile("shaders/Post/blend_frag.glsl", GL_FRAGMENT_SHADER); 
   _shaders[index]->Link();

    _pixelSize = glm::vec2(1.f / width, 1.f / height);
}

void BloomEffect::ApplyEffect(PostEffect* buffer)
{
    BindShader(0);
    //_shaders[0]->SetUniform("u_Intensity", _intensity);
   

    buffer->BindColorAsTexture(0, 0, 0); 

    _buffers[0]->RenderToFSQ();

    BindShader(1);
    _shaders[1]->SetUniform("u_threshold", _threshold);   
    _shaders[1]->SetUniform("u_strength", 1.f);  

    buffer->BindColorAsTexture(0, 0, 0);

    _buffers[1]->RenderToFSQ();

    buffer->UnbindTexture(0);

    UnbindShader();
     
    for (unsigned i = 0; i < _passes; i++)
    {
        //Horizontal pass 
        BindShader(2);
        //_shaders[2]->SetUniform("uPixelSize", _pixelSize.x);
       // _shaders[2]->SetUniform("u_horizontal", true);
        //_shaders[2]->SetUniform("weight1", _blurValues[0]);
        //_shaders[2]->SetUniform("weight2", _blurValues[1]);
        //_shaders[2]->SetUniform("weight3", _blurValues[2]);
        //_shaders[2]->SetUniform("weight4", _blurValues[3]);
        //_shaders[2]->SetUniform("weight5", _blurValues[4]);

        BindColorAsTexture(1, 0, 0);

        _buffers[2]->RenderToFSQ();

        buffer->UnbindTexture(0);

        UnbindShader();

        //Vertical pass
        BindShader(3);
        //_shaders[3]->SetUniform("uPixelSize", _pixelSize.y);
        //_shaders[3]->SetUniform("u_horizontal", false);
        //_shaders[3]->SetUniform("weight1", _blurValues[0]);
        //_shaders[3]->SetUniform("weight2", _blurValues[1]);
        //_shaders[3]->SetUniform("weight3", _blurValues[2]);
        //_shaders[3]->SetUniform("weight4", _blurValues[3]);
        //_shaders[3]->SetUniform("weight5", _blurValues[4]);

        BindColorAsTexture(2, 0, 0);

        _buffers[1]->RenderToFSQ();

        buffer->UnbindTexture(0); 

        UnbindShader();
    }
    BindShader(4);

	buffer->BindColorAsTexture(0, 0, 0); 
	BindColorAsTexture(1, 0, 1);

	_buffers[0]->RenderToFSQ();
	buffer->UnbindTexture(1);
	buffer->UnbindTexture(0);

	UnbindShader();
}

void BloomEffect::SetThreshold(float x)
{
    _threshold = x;
}

float BloomEffect::GetThreshold()
{
    return _threshold;
}

void BloomEffect::SetBlur(float x[5])
{
    for (int i = 0; i < 5; i++)
    {
        _blurValues[i] = x[i];
    }
}

float* BloomEffect::GetBlur()
{
    return _blurValues;
}
