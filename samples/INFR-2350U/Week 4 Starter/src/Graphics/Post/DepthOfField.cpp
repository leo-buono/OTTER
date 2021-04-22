#include "DepthOfField.h"

void DepthOfFieldEffect::Init(unsigned width, unsigned height)
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
    _shaders[index]->LoadShaderPartFromFile("shaders/Post/dof_calculation_frag.glsl", GL_FRAGMENT_SHADER);
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


}

void DepthOfFieldEffect::ApplyEffect(PostEffect* buffer)
{
    BindShader(0);

    buffer->BindColorAsTexture(0, 0, 0);

    _buffers[0]->RenderToFSQ();

    for (unsigned i = 0; i < _passes; i++)
    {
        //Horizontal pass 
        BindShader(2);

        BindColorAsTexture(1, 0, 0);

        _buffers[2]->RenderToFSQ();

        buffer->UnbindTexture(0);

        UnbindShader();

        //Vertical pass
        BindShader(3);

        BindColorAsTexture(2, 0, 0);

        _buffers[3]->RenderToFSQ(); 

        buffer->UnbindTexture(0);

        UnbindShader();
    }
    buffer->UnbindTexture(1);
    buffer->UnbindTexture(0);

    UnbindShader();
}

void DepthOfFieldEffect::SetThreshold(float x)
{
    _threshold = x;
}

float DepthOfFieldEffect::GetThreshold()
{
    return _threshold;
}

void DepthOfFieldEffect::SetBlur(float x[5])
{
    for (int i = 0; i < 5; i++)
    {
        _blurValues[i] = x[i];
    }
}

float* DepthOfFieldEffect::GetBlur()
{
    return _blurValues;
}
