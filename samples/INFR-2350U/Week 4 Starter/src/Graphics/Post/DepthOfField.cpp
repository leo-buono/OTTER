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

    index = int(_buffers.size());
    _buffers.push_back(new Framebuffer());
    _buffers[index]->AddColorTarget(GL_RGBA8);
    _buffers[index]->AddDepthTarget();
    _buffers[index]->Init(width, height);

    //Loads the shaders
    index = int(_shaders.size());
    _shaders.push_back(Shader::Create());
    _shaders[index]->LoadShaderPartFromFile("shaders/passthrough_vert.glsl", GL_VERTEX_SHADER);
    _shaders[index]->LoadShaderPartFromFile("shaders/Post/dof_blend_frag.glsl", GL_FRAGMENT_SHADER);
    _shaders[index]->Link();

    _width = width;
    _height = height;

}

void DepthOfFieldEffect::ApplyEffect(PostEffect* buffer) 
{
    BindShader(0);

    buffer->BindColorAsTexture(0, 0, 0); 

    _buffers[0]->RenderToFSQ();

    UnbindShader();

    BindShader(1);
    _shaders[1]->SetUniform("u_lightPos", _lightPos); 
    _shaders[1]->SetUniform("u_colour", _colour);
    //_shaders[1]->SetUniform("S1", _S1);
    _shaders[1]->SetUniform("F", _F);   
    _shaders[1]->SetUniform("A", _A);
    _shaders[1]->SetUniform("zNear", _zNear);
    _shaders[1]->SetUniform("zFar", _zFar);
    //_shaders[1]->SetUniform("MAXCoC", _MAXCoC);
    _shaders[1]->SetUniform("u_planeInFocus", _planeInFocus);
    _shaders[1]->SetUniform("u_WindowWidth", _width);
    _shaders[1]->SetUniform("u_WindowHeight", _height);  
       
    buffer->BindDepthAsTexture(0, 0);
    //BindColorAsTexture(0, 0, 1);
    

    _buffers[1]->RenderToFSQ();

    buffer->UnbindTexture(0);
     
    UnbindShader(); 

    for (unsigned i = 0; i < _passes; i++)
    {
        //Horizontal pass 
        BindShader(2);

        //BindColorAsTexture(0, 0, 0);
        BindColorAsTexture(0, 0, 0);  
               
        _buffers[2]->RenderToFSQ();

        buffer->UnbindTexture(0);    

        UnbindShader();

        //Vertical pass
        BindShader(3);
         
        //BindColorAsTexture(0, 0, 0);
        BindColorAsTexture(2, 0, 0);

        _buffers[1]->RenderToFSQ();   
         
        buffer->UnbindTexture(0);

        UnbindShader();
    } 
    buffer->UnbindTexture(0);
    //I don't know why, I don't want to know why, I shouldn't have to wonder why,  but things stop working when this is uncommented
    //buffer->UnbindTexture(1);
    BindShader(4);

    buffer->BindColorAsTexture(0, 0, 0);
    BindColorAsTexture(1, 0, 1);   
    BindColorAsTexture(2, 0, 2);
    _shaders[4]->SetUniform("simulateLerp", _threshold);


    _buffers[0]->RenderToFSQ();
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

void DepthOfFieldEffect::SetFocalDistance(float x)
{
    _S1 = x;
}

float DepthOfFieldEffect::GetFocalDist()
{
    return _S1;
}

void DepthOfFieldEffect::SetFocalLength(float x)
{
    _F = x;
}

float DepthOfFieldEffect::GetFocalLength()
{
    return _F;
}

void DepthOfFieldEffect::SetAperture(float x)
{
    _A = x;
}

float DepthOfFieldEffect::GetAperture()
{  
    return _A;
}

void DepthOfFieldEffect::SetMaxCoC(float x)
{
    _MAXCoC = x;
}

float DepthOfFieldEffect::GetMaxCoC()
{
    return _MAXCoC;
}

void DepthOfFieldEffect::SetFarZ(float x)
{
    _zFar = x;
}

float DepthOfFieldEffect::GetFarZ()
{
    return _zFar;
}

void DepthOfFieldEffect::SetNearZ(float x)
{
    _zNear = x;
}

float DepthOfFieldEffect::GetNearZ()
{
    return _zNear;
}

void DepthOfFieldEffect::SetPlaneInFocus(float x)
{
    _planeInFocus = x;
}

float DepthOfFieldEffect::GetPlaneInFocus()
{
    return _planeInFocus;
}
