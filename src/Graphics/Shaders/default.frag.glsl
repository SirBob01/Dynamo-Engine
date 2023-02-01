#version 450
#extension GL_ARB_separate_shader_objects:enable
#extension GL_GOOGLE_include_directive:enable

#include "common.glsl"

layout(set=0,binding=1)uniform sampler2D textureSampler;

layout(location=0)in vec4 fragColor;
layout(location=1)in vec2 fragTexCoord;

layout(location=0)out vec4 outColor;

void main(){
    vec4 texture=texture(textureSampler,fragTexCoord);
    vec4 color=fragColor.rgba*texture.rgba;
    
    // Discard the fragment if it is translucent
    if(color.a<.9)discard;
    else outColor=color;
}