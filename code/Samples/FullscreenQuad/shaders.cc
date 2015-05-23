//-----------------------------------------------------------------------------
// #version:22# machine generated, do not edit!
//-----------------------------------------------------------------------------
#include "Pre.h"
#include "shaders.h"

namespace Oryol {
namespace Shaders {
#if ORYOL_OPENGL
const char* vs_glsl100_src = 
"#define tex2Dvs(s, t) texture2D(s, t)\n"
"#define mul(v,m) (m * v)\n"
"#define tex2D(s, t) texture2D(s, t)\n"
"#define static \n"
"#define _color gl_FragColor\n"
"#define texCUBE(s, t) textureCube(s, t)\n"
"#define _position gl_Position\n"
"uniform float time;\n"
"attribute vec4 position;\n"
"attribute vec2 texcoord0;\n"
"varying vec2 uv;\n"
"varying vec3 eye;\n"
"varying vec3 up;\n"
"varying vec3 right;\n"
"varying vec3 fwd;\n"
"varying vec4 sinTime;\n"
"vec3 eyePos(vec3 center) {\n"
"return center + vec3(sin(time * 0.1) * 5.0, 2.0, cos(time * 0.1) * 5.0);\n"
"}\n"
"void lookat(in vec3 eye, in vec3 center, inout vec3 up, out vec3 right, out vec3 fwd) {\n"
"fwd = normalize(center - eye);\n"
"right = normalize(cross(fwd, up));\n"
"up = cross(right, fwd);\n"
"}\n"
"void main() {\n"
"_position = position;\n"
"uv = texcoord0 * 2.0 - 1.0;\n"
"vec3 center = vec3(0.0, 0.0, 0.0);\n"
"eye = eyePos(center);\n"
"up = vec3(0.0, 1.0, 0.0);\n"
"lookat(eye, center, up, right, fwd);\n"
"sinTime.x = sin(time);\n"
"sinTime.y = sin(time * 0.5);\n"
"sinTime.z = sin(time * 0.25);\n"
"sinTime.w = sin(time * 0.125);\n"
"}\n"
;
#endif
#if ORYOL_OPENGL
const char* fs_glsl100_src = 
"precision mediump float;\n"
"#ifdef GL_FRAGMENT_PRECISION_HIGH\n"
"precision highp float;\n"
"#endif\n"
"#define tex2Dvs(s, t) texture2D(s, t)\n"
"#define mul(v,m) (m * v)\n"
"#define tex2D(s, t) texture2D(s, t)\n"
"#define static \n"
"#define _color gl_FragColor\n"
"#define texCUBE(s, t) textureCube(s, t)\n"
"#define _position gl_Position\n"
"varying vec2 uv;\n"
"varying vec3 eye;\n"
"varying vec3 up;\n"
"varying vec3 right;\n"
"varying vec3 fwd;\n"
"varying vec4 sinTime;\n"
"float smin(float a, float b, float k) {\n"
"float h = clamp(0.5 + 0.5 * (b - a)/k, 0.0, 1.0);\n"
"return mix(b, a, h) - k * h * (1.0 - h);\n"
"}\n"
"float sdSphere(vec3 p, float r) {\n"
"return length(p) - r;\n"
"}\n"
"float udBox(vec3 p, vec3 b) {\n"
"return length(max(abs(p) - b, 0.0));\n"
"}\n"
"float sdBox(vec3 p, vec3 b) {\n"
"vec3 d = abs(p) - b;\n"
"return min( max(d.x, max(d.y, d.z)), 0.0) + length(max(d, 0.0));\n"
"}\n"
"float opU(float d1, float d2) {\n"
"return min(d1, d2);\n"
"}\n"
"float opSU(float d1, float d2, float k) {\n"
"return smin(d1, d2, k);\n"
"}\n"
"float opS(float d1, float d2) {\n"
"return max(-d1, d2);\n"
"}\n"
"float opI(float d1, float d2) {\n"
"return max(d1, d2);\n"
"}\n"
"float dScene(vec4 sinTime, vec3 p) {\n"
"vec3 boxPos = p;\n"
"vec3 spherePos0 = p + vec3(-sinTime.x, 0.0, 0.0) * 2.0;\n"
"float c = 0.25;\n"
"vec3 q = mod(p, c) - 0.5*c;\n"
"return opS(sdSphere(q, abs(sinTime.y) * 0.16), opSU(sdSphere(spherePos0, 1.5), sdBox(boxPos, vec3(1.0,1.0,1.0)), 0.2));\n"
"}\n"
"vec3 normal(vec4 sinTime, vec3 p) {\n"
"const float eps = 0.01;\n"
"float x = dScene(sinTime, p + vec3(eps, 0, 0)) - dScene(sinTime, p - vec3(eps, 0, 0));\n"
"float y = dScene(sinTime, p + vec3(0, eps, 0)) - dScene(sinTime, p - vec3(0, eps, 0));\n"
"float z = dScene(sinTime, p + vec3(0, 0, eps)) - dScene(sinTime, p - vec3(0, 0, eps));\n"
"return normalize(vec3(x, y, z));\n"
"}\n"
"void main() {\n"
"const float epsilon = 0.001;\n"
"const float focalLength = 2.0;\n"
"vec3 rayOrigin = eye + fwd * focalLength + right * uv.x + up * uv.y;\n"
"vec3 rayDirection = normalize(rayOrigin - eye);\n"
"vec4 color = vec4(0.5, 0.5, 0.5, 1.0);\n"
"float t = 0.0;\n"
"const int maxSteps = 64;\n"
"for (int i = 0; i < maxSteps; i++) {\n"
"vec3 p = rayOrigin + rayDirection * t;\n"
"float d = dScene(sinTime, p);\n"
"if (d < epsilon) {\n"
"vec3 nrm = normal(sinTime, p) * 0.5 + 0.5;\n"
"color = vec4(nrm, 1);\n"
"break;\n"
"}\n"
"t += d;\n"
"}\n"
"_color = color;\n"
"}\n"
;
#endif
#if ORYOL_OPENGL
const char* vs_glsl120_src = 
"#version 120\n"
"#define tex2Dvs(s, t) texture2D(s, t)\n"
"#define mul(v,m) (m * v)\n"
"#define tex2D(s, t) texture2D(s, t)\n"
"#define static \n"
"#define _color gl_FragColor\n"
"#define texCUBE(s, t) textureCube(s, t)\n"
"#define _position gl_Position\n"
"uniform float time;\n"
"attribute vec4 position;\n"
"attribute vec2 texcoord0;\n"
"varying vec2 uv;\n"
"varying vec3 eye;\n"
"varying vec3 up;\n"
"varying vec3 right;\n"
"varying vec3 fwd;\n"
"varying vec4 sinTime;\n"
"vec3 eyePos(vec3 center) {\n"
"return center + vec3(sin(time * 0.1) * 5.0, 2.0, cos(time * 0.1) * 5.0);\n"
"}\n"
"void lookat(in vec3 eye, in vec3 center, inout vec3 up, out vec3 right, out vec3 fwd) {\n"
"fwd = normalize(center - eye);\n"
"right = normalize(cross(fwd, up));\n"
"up = cross(right, fwd);\n"
"}\n"
"void main() {\n"
"_position = position;\n"
"uv = texcoord0 * 2.0 - 1.0;\n"
"vec3 center = vec3(0.0, 0.0, 0.0);\n"
"eye = eyePos(center);\n"
"up = vec3(0.0, 1.0, 0.0);\n"
"lookat(eye, center, up, right, fwd);\n"
"sinTime.x = sin(time);\n"
"sinTime.y = sin(time * 0.5);\n"
"sinTime.z = sin(time * 0.25);\n"
"sinTime.w = sin(time * 0.125);\n"
"}\n"
;
#endif
#if ORYOL_OPENGL
const char* fs_glsl120_src = 
"#version 120\n"
"#define tex2Dvs(s, t) texture2D(s, t)\n"
"#define mul(v,m) (m * v)\n"
"#define tex2D(s, t) texture2D(s, t)\n"
"#define static \n"
"#define _color gl_FragColor\n"
"#define texCUBE(s, t) textureCube(s, t)\n"
"#define _position gl_Position\n"
"varying vec2 uv;\n"
"varying vec3 eye;\n"
"varying vec3 up;\n"
"varying vec3 right;\n"
"varying vec3 fwd;\n"
"varying vec4 sinTime;\n"
"float smin(float a, float b, float k) {\n"
"float h = clamp(0.5 + 0.5 * (b - a)/k, 0.0, 1.0);\n"
"return mix(b, a, h) - k * h * (1.0 - h);\n"
"}\n"
"float sdSphere(vec3 p, float r) {\n"
"return length(p) - r;\n"
"}\n"
"float udBox(vec3 p, vec3 b) {\n"
"return length(max(abs(p) - b, 0.0));\n"
"}\n"
"float sdBox(vec3 p, vec3 b) {\n"
"vec3 d = abs(p) - b;\n"
"return min( max(d.x, max(d.y, d.z)), 0.0) + length(max(d, 0.0));\n"
"}\n"
"float opU(float d1, float d2) {\n"
"return min(d1, d2);\n"
"}\n"
"float opSU(float d1, float d2, float k) {\n"
"return smin(d1, d2, k);\n"
"}\n"
"float opS(float d1, float d2) {\n"
"return max(-d1, d2);\n"
"}\n"
"float opI(float d1, float d2) {\n"
"return max(d1, d2);\n"
"}\n"
"float dScene(vec4 sinTime, vec3 p) {\n"
"vec3 boxPos = p;\n"
"vec3 spherePos0 = p + vec3(-sinTime.x, 0.0, 0.0) * 2.0;\n"
"float c = 0.25;\n"
"vec3 q = mod(p, c) - 0.5*c;\n"
"return opS(sdSphere(q, abs(sinTime.y) * 0.16), opSU(sdSphere(spherePos0, 1.5), sdBox(boxPos, vec3(1.0,1.0,1.0)), 0.2));\n"
"}\n"
"vec3 normal(vec4 sinTime, vec3 p) {\n"
"const float eps = 0.01;\n"
"float x = dScene(sinTime, p + vec3(eps, 0, 0)) - dScene(sinTime, p - vec3(eps, 0, 0));\n"
"float y = dScene(sinTime, p + vec3(0, eps, 0)) - dScene(sinTime, p - vec3(0, eps, 0));\n"
"float z = dScene(sinTime, p + vec3(0, 0, eps)) - dScene(sinTime, p - vec3(0, 0, eps));\n"
"return normalize(vec3(x, y, z));\n"
"}\n"
"void main() {\n"
"const float epsilon = 0.001;\n"
"const float focalLength = 2.0;\n"
"vec3 rayOrigin = eye + fwd * focalLength + right * uv.x + up * uv.y;\n"
"vec3 rayDirection = normalize(rayOrigin - eye);\n"
"vec4 color = vec4(0.5, 0.5, 0.5, 1.0);\n"
"float t = 0.0;\n"
"const int maxSteps = 64;\n"
"for (int i = 0; i < maxSteps; i++) {\n"
"vec3 p = rayOrigin + rayDirection * t;\n"
"float d = dScene(sinTime, p);\n"
"if (d < epsilon) {\n"
"vec3 nrm = normal(sinTime, p) * 0.5 + 0.5;\n"
"color = vec4(nrm, 1);\n"
"break;\n"
"}\n"
"t += d;\n"
"}\n"
"_color = color;\n"
"}\n"
;
#endif
#if ORYOL_OPENGL
const char* vs_glsl150_src = 
"#version 150\n"
"#define tex2Dvs(s, t) texture(s, t)\n"
"#define mul(v,m) (m * v)\n"
"#define tex2D(s, t) texture(s, t)\n"
"#define static \n"
"#define _color _FragColor\n"
"#define texCUBE(s, t) texture(s, t)\n"
"#define _position gl_Position\n"
"uniform float time;\n"
"in vec4 position;\n"
"in vec2 texcoord0;\n"
"out vec2 uv;\n"
"out vec3 eye;\n"
"out vec3 up;\n"
"out vec3 right;\n"
"out vec3 fwd;\n"
"out vec4 sinTime;\n"
"vec3 eyePos(vec3 center) {\n"
"return center + vec3(sin(time * 0.1) * 5.0, 2.0, cos(time * 0.1) * 5.0);\n"
"}\n"
"void lookat(in vec3 eye, in vec3 center, inout vec3 up, out vec3 right, out vec3 fwd) {\n"
"fwd = normalize(center - eye);\n"
"right = normalize(cross(fwd, up));\n"
"up = cross(right, fwd);\n"
"}\n"
"void main() {\n"
"_position = position;\n"
"uv = texcoord0 * 2.0 - 1.0;\n"
"vec3 center = vec3(0.0, 0.0, 0.0);\n"
"eye = eyePos(center);\n"
"up = vec3(0.0, 1.0, 0.0);\n"
"lookat(eye, center, up, right, fwd);\n"
"sinTime.x = sin(time);\n"
"sinTime.y = sin(time * 0.5);\n"
"sinTime.z = sin(time * 0.25);\n"
"sinTime.w = sin(time * 0.125);\n"
"}\n"
;
#endif
#if ORYOL_OPENGL
const char* fs_glsl150_src = 
"#version 150\n"
"#define tex2Dvs(s, t) texture(s, t)\n"
"#define mul(v,m) (m * v)\n"
"#define tex2D(s, t) texture(s, t)\n"
"#define static \n"
"#define _color _FragColor\n"
"#define texCUBE(s, t) texture(s, t)\n"
"#define _position gl_Position\n"
"in vec2 uv;\n"
"in vec3 eye;\n"
"in vec3 up;\n"
"in vec3 right;\n"
"in vec3 fwd;\n"
"in vec4 sinTime;\n"
"out vec4 _FragColor;\n"
"float smin(float a, float b, float k) {\n"
"float h = clamp(0.5 + 0.5 * (b - a)/k, 0.0, 1.0);\n"
"return mix(b, a, h) - k * h * (1.0 - h);\n"
"}\n"
"float sdSphere(vec3 p, float r) {\n"
"return length(p) - r;\n"
"}\n"
"float udBox(vec3 p, vec3 b) {\n"
"return length(max(abs(p) - b, 0.0));\n"
"}\n"
"float sdBox(vec3 p, vec3 b) {\n"
"vec3 d = abs(p) - b;\n"
"return min( max(d.x, max(d.y, d.z)), 0.0) + length(max(d, 0.0));\n"
"}\n"
"float opU(float d1, float d2) {\n"
"return min(d1, d2);\n"
"}\n"
"float opSU(float d1, float d2, float k) {\n"
"return smin(d1, d2, k);\n"
"}\n"
"float opS(float d1, float d2) {\n"
"return max(-d1, d2);\n"
"}\n"
"float opI(float d1, float d2) {\n"
"return max(d1, d2);\n"
"}\n"
"float dScene(vec4 sinTime, vec3 p) {\n"
"vec3 boxPos = p;\n"
"vec3 spherePos0 = p + vec3(-sinTime.x, 0.0, 0.0) * 2.0;\n"
"float c = 0.25;\n"
"vec3 q = mod(p, c) - 0.5*c;\n"
"return opS(sdSphere(q, abs(sinTime.y) * 0.16), opSU(sdSphere(spherePos0, 1.5), sdBox(boxPos, vec3(1.0,1.0,1.0)), 0.2));\n"
"}\n"
"vec3 normal(vec4 sinTime, vec3 p) {\n"
"const float eps = 0.01;\n"
"float x = dScene(sinTime, p + vec3(eps, 0, 0)) - dScene(sinTime, p - vec3(eps, 0, 0));\n"
"float y = dScene(sinTime, p + vec3(0, eps, 0)) - dScene(sinTime, p - vec3(0, eps, 0));\n"
"float z = dScene(sinTime, p + vec3(0, 0, eps)) - dScene(sinTime, p - vec3(0, 0, eps));\n"
"return normalize(vec3(x, y, z));\n"
"}\n"
"void main() {\n"
"const float epsilon = 0.001;\n"
"const float focalLength = 2.0;\n"
"vec3 rayOrigin = eye + fwd * focalLength + right * uv.x + up * uv.y;\n"
"vec3 rayDirection = normalize(rayOrigin - eye);\n"
"vec4 color = vec4(0.5, 0.5, 0.5, 1.0);\n"
"float t = 0.0;\n"
"const int maxSteps = 64;\n"
"for (int i = 0; i < maxSteps; i++) {\n"
"vec3 p = rayOrigin + rayDirection * t;\n"
"float d = dScene(sinTime, p);\n"
"if (d < epsilon) {\n"
"vec3 nrm = normal(sinTime, p) * 0.5 + 0.5;\n"
"color = vec4(nrm, 1);\n"
"break;\n"
"}\n"
"t += d;\n"
"}\n"
"_color = color;\n"
"}\n"
;
#endif
#if ORYOL_D3D11
const char* vs_hlsl5_src = 
"#define mod(x,y) (x-y*floor(x/y))\n"
"#define fract(x) frac(x)\n"
"#define vec4 float4\n"
"#define vec2 float2\n"
"#define vec3 float3\n"
"#define texCUBE(s, t) s.Sample(s ## _sampler, t)\n"
"#define _position _oPosition\n"
"#define mat4 float4x4\n"
"#define mat2 float2x2\n"
"#define mat3 float3x3\n"
"#define tex2D(s, t) s.Sample(s ## _sampler, t)\n"
"#define mix(a,b,c) lerp(a,b,c)\n"
"#define _color _oColor\n"
"#define tex2Dvs(s, t) s.Load(int3(t.x, t.y, 0))\n"
"float time;\n"
"vec3 eyePos(vec3 center) {\n"
"return center + vec3(sin(time * 0.1) * 5.0, 2.0, cos(time * 0.1) * 5.0);\n"
"}\n"
"void lookat(in vec3 eye, in vec3 center, inout vec3 up, out vec3 right, out vec3 fwd) {\n"
"fwd = normalize(center - eye);\n"
"right = normalize(cross(fwd, up));\n"
"up = cross(right, fwd);\n"
"}\n"
"void main(\n"
"in vec4 position : position,\n"
"in vec2 texcoord0 : texcoord0,\n"
"out vec2 uv : uv,\n"
"out vec3 eye : eye,\n"
"out vec3 up : up,\n"
"out vec3 right : right,\n"
"out vec3 fwd : fwd,\n"
"out vec4 sinTime : sinTime,\n"
"out vec4 _oPosition : SV_POSITION) {\n"
"_position = position;\n"
"uv = texcoord0 * 2.0 - 1.0;\n"
"vec3 center = vec3(0.0, 0.0, 0.0);\n"
"eye = eyePos(center);\n"
"up = vec3(0.0, 1.0, 0.0);\n"
"lookat(eye, center, up, right, fwd);\n"
"sinTime.x = sin(time);\n"
"sinTime.y = sin(time * 0.5);\n"
"sinTime.z = sin(time * 0.25);\n"
"sinTime.w = sin(time * 0.125);\n"
"}\n"
;
#endif
#if ORYOL_D3D11
const char* fs_hlsl5_src = 
"#define mod(x,y) (x-y*floor(x/y))\n"
"#define fract(x) frac(x)\n"
"#define vec4 float4\n"
"#define vec2 float2\n"
"#define vec3 float3\n"
"#define texCUBE(s, t) s.Sample(s ## _sampler, t)\n"
"#define _position _oPosition\n"
"#define mat4 float4x4\n"
"#define mat2 float2x2\n"
"#define mat3 float3x3\n"
"#define tex2D(s, t) s.Sample(s ## _sampler, t)\n"
"#define mix(a,b,c) lerp(a,b,c)\n"
"#define _color _oColor\n"
"#define tex2Dvs(s, t) s.Load(int3(t.x, t.y, 0))\n"
"float smin(float a, float b, float k) {\n"
"float h = clamp(0.5 + 0.5 * (b - a)/k, 0.0, 1.0);\n"
"return mix(b, a, h) - k * h * (1.0 - h);\n"
"}\n"
"float sdSphere(vec3 p, float r) {\n"
"return length(p) - r;\n"
"}\n"
"float udBox(vec3 p, vec3 b) {\n"
"return length(max(abs(p) - b, 0.0));\n"
"}\n"
"float sdBox(vec3 p, vec3 b) {\n"
"vec3 d = abs(p) - b;\n"
"return min( max(d.x, max(d.y, d.z)), 0.0) + length(max(d, 0.0));\n"
"}\n"
"float opU(float d1, float d2) {\n"
"return min(d1, d2);\n"
"}\n"
"float opSU(float d1, float d2, float k) {\n"
"return smin(d1, d2, k);\n"
"}\n"
"float opS(float d1, float d2) {\n"
"return max(-d1, d2);\n"
"}\n"
"float opI(float d1, float d2) {\n"
"return max(d1, d2);\n"
"}\n"
"float dScene(vec4 sinTime, vec3 p) {\n"
"vec3 boxPos = p;\n"
"vec3 spherePos0 = p + vec3(-sinTime.x, 0.0, 0.0) * 2.0;\n"
"float c = 0.25;\n"
"vec3 q = mod(p, c) - 0.5*c;\n"
"return opS(sdSphere(q, abs(sinTime.y) * 0.16), opSU(sdSphere(spherePos0, 1.5), sdBox(boxPos, vec3(1.0,1.0,1.0)), 0.2));\n"
"}\n"
"vec3 normal(vec4 sinTime, vec3 p) {\n"
"const float eps = 0.01;\n"
"float x = dScene(sinTime, p + vec3(eps, 0, 0)) - dScene(sinTime, p - vec3(eps, 0, 0));\n"
"float y = dScene(sinTime, p + vec3(0, eps, 0)) - dScene(sinTime, p - vec3(0, eps, 0));\n"
"float z = dScene(sinTime, p + vec3(0, 0, eps)) - dScene(sinTime, p - vec3(0, 0, eps));\n"
"return normalize(vec3(x, y, z));\n"
"}\n"
"void main(\n"
"in vec2 uv : uv,\n"
"in vec3 eye : eye,\n"
"in vec3 up : up,\n"
"in vec3 right : right,\n"
"in vec3 fwd : fwd,\n"
"in vec4 sinTime : sinTime,\n"
"out vec4 _oColor : SV_TARGET) {\n"
"const float epsilon = 0.001;\n"
"const float focalLength = 2.0;\n"
"vec3 rayOrigin = eye + fwd * focalLength + right * uv.x + up * uv.y;\n"
"vec3 rayDirection = normalize(rayOrigin - eye);\n"
"vec4 color = vec4(0.5, 0.5, 0.5, 1.0);\n"
"float t = 0.0;\n"
"const int maxSteps = 64;\n"
"for (int i = 0; i < maxSteps; i++) {\n"
"vec3 p = rayOrigin + rayDirection * t;\n"
"float d = dScene(sinTime, p);\n"
"if (d < epsilon) {\n"
"vec3 nrm = normal(sinTime, p) * 0.5 + 0.5;\n"
"color = vec4(nrm, 1);\n"
"break;\n"
"}\n"
"t += d;\n"
"}\n"
"_color = color;\n"
"}\n"
;
#endif
ProgramBundleSetup Main::CreateSetup() {
    ProgramBundleSetup setup("Main");
    #if ORYOL_OPENGL
    setup.AddProgramFromSources(0, ShaderLang::GLSL100, vs_glsl100_src, fs_glsl100_src);
    #endif
    #if ORYOL_OPENGL
    setup.AddProgramFromSources(0, ShaderLang::GLSL120, vs_glsl120_src, fs_glsl120_src);
    #endif
    #if ORYOL_OPENGL
    setup.AddProgramFromSources(0, ShaderLang::GLSL150, vs_glsl150_src, fs_glsl150_src);
    #endif
    #if ORYOL_D3D11
    setup.AddProgramFromSources(0, ShaderLang::HLSL5, vs_hlsl5_src, fs_hlsl5_src);
    #endif
    setup.AddUniform("time", Time);
    return setup;
}
}
}

