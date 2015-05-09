//-----------------------------------------------------------------------------
// #version:15# machine generated, do not edit!
//-----------------------------------------------------------------------------
#include "Pre.h"
#include "shaders.h"

namespace Oryol {
namespace Shaders {
const char* drawVS_100_src = 
"#define _TEXTURE2D texture2D\n"
"#define _POSITION gl_Position\n"
"#ifdef GL_FRAGMENT_PRECISION_HIGH\n"
"precision highp sampler2D;\n"
"#endif\n"
"uniform mat4 mvp;\n"
"uniform vec2 bufDims;\n"
"uniform sampler2D particleTex;\n"
"attribute vec4 position;\n"
"attribute vec4 color0;\n"
"attribute float instance0;\n"
"varying vec4 color;\n"
"vec2 posUvFromParticleId(float particleId, vec2 bufferDims) {\n"
"float numParticlesAlongX = bufferDims.x * 0.5;\n"
"float f = particleId / numParticlesAlongX;\n"
"float fragCoordX = fract(f) * numParticlesAlongX * 2.0;\n"
"float fragCoordY = floor(f);\n"
"vec2 posUV = vec2(fragCoordX, fragCoordY) / bufferDims;\n"
"return posUV;\n"
"}\n"
"void main() {\n"
"float particleId = instance0;\n"
"vec2 posUv = posUvFromParticleId(particleId, bufDims);\n"
"vec4 particlePos = vec4(_TEXTURE2D(particleTex, posUv).xyz, 0.0);\n"
"_POSITION = mvp * (position + particlePos);\n"
"color = color0;\n"
"}\n"
;
const char* fsqVS_100_src = 
"#define _POSITION gl_Position\n"
"attribute vec4 position;\n"
"void main() {\n"
"_POSITION = position;\n"
"}\n"
;
const char* initFS_100_src = 
"precision mediump float;\n"
"#ifdef GL_FRAGMENT_PRECISION_HIGH\n"
"precision highp float;\n"
"#endif\n"
"#define _COLOR gl_FragColor\n"
"uniform vec2 bufDims;\n"
"uniform float time;\n"
"float getParticleId(float bufferWidth, vec2 fragCoord) {\n"
"vec2 xy = floor(fragCoord);\n"
"return (xy.y * bufferWidth * 0.5) + (xy.x * 0.5);\n"
"}\n"
"bool atPosition(vec2 fragCoord) {\n"
"return mod(floor(fragCoord.x), 2.0) < 0.5;\n"
"}\n"
"void getParticleStateUvs(bool atPos, vec2 bufDims, vec2 fragCoord, out vec2 posUv, out vec2 velUv) {\n"
"if (atPos) {\n"
"posUv = fragCoord / bufDims;\n"
"velUv = (fragCoord + vec2(1.0, 0.0)) / bufDims;\n"
"}\n"
"else {\n"
"posUv = (fragCoord + vec2(-1.0, 0.0)) / bufDims;\n"
"velUv = fragCoord / bufDims;\n"
"}\n"
"}\n"
"vec3 mod289(vec3 x) {\n"
"return x - floor(x * (1.0 / 289.0)) * 289.0;\n"
"}\n"
"vec2 mod289(vec2 x) {\n"
"return x - floor(x * (1.0 / 289.0)) * 289.0;\n"
"}\n"
"vec3 permute(vec3 x) {\n"
"return mod289(((x*34.0)+1.0)*x);\n"
"}\n"
"float snoise(vec2 v)\n"
"{\n"
"const vec4 C = vec4(0.211324865405187,\n"
"0.366025403784439,\n"
"-0.577350269189626,\n"
"0.024390243902439);\n"
"vec2 i  = floor(v + dot(v, C.yy) );\n"
"vec2 x0 = v -   i + dot(i, C.xx);\n"
"vec2 i1;\n"
"i1 = (x0.x > x0.y) ? vec2(1.0, 0.0) : vec2(0.0, 1.0);\n"
"vec4 x12 = x0.xyxy + C.xxzz;\n"
"x12.xy -= i1;\n"
"i = mod289(i);\n"
"vec3 p = permute( permute( i.y + vec3(0.0, i1.y, 1.0 ))\n"
"+ i.x + vec3(0.0, i1.x, 1.0 ));\n"
"vec3 m = max(0.5 - vec3(dot(x0,x0), dot(x12.xy,x12.xy), dot(x12.zw,x12.zw)), 0.0);\n"
"m = m*m ;\n"
"m = m*m ;\n"
"vec3 x = 2.0 * fract(p * C.www) - 1.0;\n"
"vec3 h = abs(x) - 0.5;\n"
"vec3 ox = floor(x + 0.5);\n"
"vec3 a0 = x - ox;\n"
"m *= 1.79284291400159 - 0.85373472095314 * ( a0*a0 + h*h );\n"
"vec3 g;\n"
"g.x  = a0.x  * x0.x  + h.x  * x0.y;\n"
"g.yz = a0.yz * x12.xz + h.yz * x12.yw;\n"
"return 130.0 * dot(m, g);\n"
"}\n"
"void main() {\n"
"if (atPosition(gl_FragCoord.xy)) {\n"
"_COLOR = vec4(0.0, 0.0, 0.0, 0.0);\n"
"}\n"
"else {\n"
"float particleId = getParticleId(bufDims.x, gl_FragCoord.xy);\n"
"vec3 rnd = vec3(snoise(vec2(particleId, 0.0)),\n"
"snoise(vec2(0.0, particleId * 0.1)),\n"
"snoise(vec2(particleId * 0.1, particleId * 0.01)));\n"
"rnd *= 0.5;\n"
"rnd.y += 2.0;\n"
"_COLOR = vec4(rnd, 0.0);\n"
"}\n"
"}\n"
;
const char* updateFS_100_src = 
"precision mediump float;\n"
"#ifdef GL_FRAGMENT_PRECISION_HIGH\n"
"precision highp float;\n"
"#endif\n"
"#define _TEXTURE2D texture2D\n"
"#define _COLOR gl_FragColor\n"
"uniform vec2 bufDims;\n"
"uniform sampler2D prevState;\n"
"uniform float numParticles;\n"
"varying vec2 uv;\n"
"float getParticleId(float bufferWidth, vec2 fragCoord) {\n"
"vec2 xy = floor(fragCoord);\n"
"return (xy.y * bufferWidth * 0.5) + (xy.x * 0.5);\n"
"}\n"
"bool atPosition(vec2 fragCoord) {\n"
"return mod(floor(fragCoord.x), 2.0) < 0.5;\n"
"}\n"
"void getParticleStateUvs(bool atPos, vec2 bufDims, vec2 fragCoord, out vec2 posUv, out vec2 velUv) {\n"
"if (atPos) {\n"
"posUv = fragCoord / bufDims;\n"
"velUv = (fragCoord + vec2(1.0, 0.0)) / bufDims;\n"
"}\n"
"else {\n"
"posUv = (fragCoord + vec2(-1.0, 0.0)) / bufDims;\n"
"velUv = fragCoord / bufDims;\n"
"}\n"
"}\n"
"void main() {\n"
"const float frameTime = 1.0 / 60.0;\n"
"float particleId = getParticleId(bufDims.x, gl_FragCoord.xy);\n"
"if (particleId >= numParticles) {\n"
"discard;\n"
"}\n"
"bool atPos = atPosition(gl_FragCoord.xy);\n"
"vec2 posUv, velUv;\n"
"getParticleStateUvs(atPos, bufDims.xy, gl_FragCoord.xy, posUv, velUv);\n"
"vec4 pos = _TEXTURE2D(prevState, posUv);\n"
"vec4 vel = _TEXTURE2D(prevState, velUv);\n"
"if (atPos) {\n"
"pos += vel * frameTime;\n"
"_COLOR = pos;\n"
"}\n"
"else {\n"
"vec4 vel = _TEXTURE2D(prevState, velUv);\n"
"vel.y -= 1.0 * frameTime;\n"
"if ((pos.y < -1.0) && (vel.y < 0.0)) {\n"
"vel.y = -vel.y;\n"
"vel *= 0.8;\n"
"}\n"
"_COLOR = vel;\n"
"}\n"
"}\n"
;
const char* drawFS_100_src = 
"precision mediump float;\n"
"#define _COLOR gl_FragColor\n"
"varying vec4 color;\n"
"void main() {\n"
"_COLOR = color;\n"
"}\n"
;
const char* drawVS_120_src = 
"#version 120\n"
"#define _TEXTURE2D texture2D\n"
"#define _POSITION gl_Position\n"
"uniform mat4 mvp;\n"
"uniform vec2 bufDims;\n"
"uniform sampler2D particleTex;\n"
"attribute vec4 position;\n"
"attribute vec4 color0;\n"
"attribute float instance0;\n"
"varying vec4 color;\n"
"vec2 posUvFromParticleId(float particleId, vec2 bufferDims) {\n"
"float numParticlesAlongX = bufferDims.x * 0.5;\n"
"float f = particleId / numParticlesAlongX;\n"
"float fragCoordX = fract(f) * numParticlesAlongX * 2.0;\n"
"float fragCoordY = floor(f);\n"
"vec2 posUV = vec2(fragCoordX, fragCoordY) / bufferDims;\n"
"return posUV;\n"
"}\n"
"void main() {\n"
"float particleId = instance0;\n"
"vec2 posUv = posUvFromParticleId(particleId, bufDims);\n"
"vec4 particlePos = vec4(_TEXTURE2D(particleTex, posUv).xyz, 0.0);\n"
"_POSITION = mvp * (position + particlePos);\n"
"color = color0;\n"
"}\n"
;
const char* fsqVS_120_src = 
"#version 120\n"
"#define _POSITION gl_Position\n"
"attribute vec4 position;\n"
"void main() {\n"
"_POSITION = position;\n"
"}\n"
;
const char* initFS_120_src = 
"#version 120\n"
"#define _COLOR gl_FragColor\n"
"uniform vec2 bufDims;\n"
"uniform float time;\n"
"float getParticleId(float bufferWidth, vec2 fragCoord) {\n"
"vec2 xy = floor(fragCoord);\n"
"return (xy.y * bufferWidth * 0.5) + (xy.x * 0.5);\n"
"}\n"
"bool atPosition(vec2 fragCoord) {\n"
"return mod(floor(fragCoord.x), 2.0) < 0.5;\n"
"}\n"
"void getParticleStateUvs(bool atPos, vec2 bufDims, vec2 fragCoord, out vec2 posUv, out vec2 velUv) {\n"
"if (atPos) {\n"
"posUv = fragCoord / bufDims;\n"
"velUv = (fragCoord + vec2(1.0, 0.0)) / bufDims;\n"
"}\n"
"else {\n"
"posUv = (fragCoord + vec2(-1.0, 0.0)) / bufDims;\n"
"velUv = fragCoord / bufDims;\n"
"}\n"
"}\n"
"vec3 mod289(vec3 x) {\n"
"return x - floor(x * (1.0 / 289.0)) * 289.0;\n"
"}\n"
"vec2 mod289(vec2 x) {\n"
"return x - floor(x * (1.0 / 289.0)) * 289.0;\n"
"}\n"
"vec3 permute(vec3 x) {\n"
"return mod289(((x*34.0)+1.0)*x);\n"
"}\n"
"float snoise(vec2 v)\n"
"{\n"
"const vec4 C = vec4(0.211324865405187,\n"
"0.366025403784439,\n"
"-0.577350269189626,\n"
"0.024390243902439);\n"
"vec2 i  = floor(v + dot(v, C.yy) );\n"
"vec2 x0 = v -   i + dot(i, C.xx);\n"
"vec2 i1;\n"
"i1 = (x0.x > x0.y) ? vec2(1.0, 0.0) : vec2(0.0, 1.0);\n"
"vec4 x12 = x0.xyxy + C.xxzz;\n"
"x12.xy -= i1;\n"
"i = mod289(i);\n"
"vec3 p = permute( permute( i.y + vec3(0.0, i1.y, 1.0 ))\n"
"+ i.x + vec3(0.0, i1.x, 1.0 ));\n"
"vec3 m = max(0.5 - vec3(dot(x0,x0), dot(x12.xy,x12.xy), dot(x12.zw,x12.zw)), 0.0);\n"
"m = m*m ;\n"
"m = m*m ;\n"
"vec3 x = 2.0 * fract(p * C.www) - 1.0;\n"
"vec3 h = abs(x) - 0.5;\n"
"vec3 ox = floor(x + 0.5);\n"
"vec3 a0 = x - ox;\n"
"m *= 1.79284291400159 - 0.85373472095314 * ( a0*a0 + h*h );\n"
"vec3 g;\n"
"g.x  = a0.x  * x0.x  + h.x  * x0.y;\n"
"g.yz = a0.yz * x12.xz + h.yz * x12.yw;\n"
"return 130.0 * dot(m, g);\n"
"}\n"
"void main() {\n"
"if (atPosition(gl_FragCoord.xy)) {\n"
"_COLOR = vec4(0.0, 0.0, 0.0, 0.0);\n"
"}\n"
"else {\n"
"float particleId = getParticleId(bufDims.x, gl_FragCoord.xy);\n"
"vec3 rnd = vec3(snoise(vec2(particleId, 0.0)),\n"
"snoise(vec2(0.0, particleId * 0.1)),\n"
"snoise(vec2(particleId * 0.1, particleId * 0.01)));\n"
"rnd *= 0.5;\n"
"rnd.y += 2.0;\n"
"_COLOR = vec4(rnd, 0.0);\n"
"}\n"
"}\n"
;
const char* updateFS_120_src = 
"#version 120\n"
"#define _TEXTURE2D texture2D\n"
"#define _COLOR gl_FragColor\n"
"uniform vec2 bufDims;\n"
"uniform sampler2D prevState;\n"
"uniform float numParticles;\n"
"varying vec2 uv;\n"
"float getParticleId(float bufferWidth, vec2 fragCoord) {\n"
"vec2 xy = floor(fragCoord);\n"
"return (xy.y * bufferWidth * 0.5) + (xy.x * 0.5);\n"
"}\n"
"bool atPosition(vec2 fragCoord) {\n"
"return mod(floor(fragCoord.x), 2.0) < 0.5;\n"
"}\n"
"void getParticleStateUvs(bool atPos, vec2 bufDims, vec2 fragCoord, out vec2 posUv, out vec2 velUv) {\n"
"if (atPos) {\n"
"posUv = fragCoord / bufDims;\n"
"velUv = (fragCoord + vec2(1.0, 0.0)) / bufDims;\n"
"}\n"
"else {\n"
"posUv = (fragCoord + vec2(-1.0, 0.0)) / bufDims;\n"
"velUv = fragCoord / bufDims;\n"
"}\n"
"}\n"
"void main() {\n"
"const float frameTime = 1.0 / 60.0;\n"
"float particleId = getParticleId(bufDims.x, gl_FragCoord.xy);\n"
"if (particleId >= numParticles) {\n"
"discard;\n"
"}\n"
"bool atPos = atPosition(gl_FragCoord.xy);\n"
"vec2 posUv, velUv;\n"
"getParticleStateUvs(atPos, bufDims.xy, gl_FragCoord.xy, posUv, velUv);\n"
"vec4 pos = _TEXTURE2D(prevState, posUv);\n"
"vec4 vel = _TEXTURE2D(prevState, velUv);\n"
"if (atPos) {\n"
"pos += vel * frameTime;\n"
"_COLOR = pos;\n"
"}\n"
"else {\n"
"vec4 vel = _TEXTURE2D(prevState, velUv);\n"
"vel.y -= 1.0 * frameTime;\n"
"if ((pos.y < -1.0) && (vel.y < 0.0)) {\n"
"vel.y = -vel.y;\n"
"vel *= 0.8;\n"
"}\n"
"_COLOR = vel;\n"
"}\n"
"}\n"
;
const char* drawFS_120_src = 
"#version 120\n"
"#define _COLOR gl_FragColor\n"
"varying vec4 color;\n"
"void main() {\n"
"_COLOR = color;\n"
"}\n"
;
const char* drawVS_150_src = 
"#version 150\n"
"#define _TEXTURE2D texture\n"
"#define _POSITION gl_Position\n"
"uniform mat4 mvp;\n"
"uniform vec2 bufDims;\n"
"uniform sampler2D particleTex;\n"
"in vec4 position;\n"
"in vec4 color0;\n"
"in float instance0;\n"
"out vec4 color;\n"
"vec2 posUvFromParticleId(float particleId, vec2 bufferDims) {\n"
"float numParticlesAlongX = bufferDims.x * 0.5;\n"
"float f = particleId / numParticlesAlongX;\n"
"float fragCoordX = fract(f) * numParticlesAlongX * 2.0;\n"
"float fragCoordY = floor(f);\n"
"vec2 posUV = vec2(fragCoordX, fragCoordY) / bufferDims;\n"
"return posUV;\n"
"}\n"
"void main() {\n"
"float particleId = instance0;\n"
"vec2 posUv = posUvFromParticleId(particleId, bufDims);\n"
"vec4 particlePos = vec4(_TEXTURE2D(particleTex, posUv).xyz, 0.0);\n"
"_POSITION = mvp * (position + particlePos);\n"
"color = color0;\n"
"}\n"
;
const char* fsqVS_150_src = 
"#version 150\n"
"#define _POSITION gl_Position\n"
"in vec4 position;\n"
"void main() {\n"
"_POSITION = position;\n"
"}\n"
;
const char* initFS_150_src = 
"#version 150\n"
"#define _COLOR _FragColor\n"
"uniform vec2 bufDims;\n"
"uniform float time;\n"
"out vec4 _FragColor;\n"
"float getParticleId(float bufferWidth, vec2 fragCoord) {\n"
"vec2 xy = floor(fragCoord);\n"
"return (xy.y * bufferWidth * 0.5) + (xy.x * 0.5);\n"
"}\n"
"bool atPosition(vec2 fragCoord) {\n"
"return mod(floor(fragCoord.x), 2.0) < 0.5;\n"
"}\n"
"void getParticleStateUvs(bool atPos, vec2 bufDims, vec2 fragCoord, out vec2 posUv, out vec2 velUv) {\n"
"if (atPos) {\n"
"posUv = fragCoord / bufDims;\n"
"velUv = (fragCoord + vec2(1.0, 0.0)) / bufDims;\n"
"}\n"
"else {\n"
"posUv = (fragCoord + vec2(-1.0, 0.0)) / bufDims;\n"
"velUv = fragCoord / bufDims;\n"
"}\n"
"}\n"
"vec3 mod289(vec3 x) {\n"
"return x - floor(x * (1.0 / 289.0)) * 289.0;\n"
"}\n"
"vec2 mod289(vec2 x) {\n"
"return x - floor(x * (1.0 / 289.0)) * 289.0;\n"
"}\n"
"vec3 permute(vec3 x) {\n"
"return mod289(((x*34.0)+1.0)*x);\n"
"}\n"
"float snoise(vec2 v)\n"
"{\n"
"const vec4 C = vec4(0.211324865405187,\n"
"0.366025403784439,\n"
"-0.577350269189626,\n"
"0.024390243902439);\n"
"vec2 i  = floor(v + dot(v, C.yy) );\n"
"vec2 x0 = v -   i + dot(i, C.xx);\n"
"vec2 i1;\n"
"i1 = (x0.x > x0.y) ? vec2(1.0, 0.0) : vec2(0.0, 1.0);\n"
"vec4 x12 = x0.xyxy + C.xxzz;\n"
"x12.xy -= i1;\n"
"i = mod289(i);\n"
"vec3 p = permute( permute( i.y + vec3(0.0, i1.y, 1.0 ))\n"
"+ i.x + vec3(0.0, i1.x, 1.0 ));\n"
"vec3 m = max(0.5 - vec3(dot(x0,x0), dot(x12.xy,x12.xy), dot(x12.zw,x12.zw)), 0.0);\n"
"m = m*m ;\n"
"m = m*m ;\n"
"vec3 x = 2.0 * fract(p * C.www) - 1.0;\n"
"vec3 h = abs(x) - 0.5;\n"
"vec3 ox = floor(x + 0.5);\n"
"vec3 a0 = x - ox;\n"
"m *= 1.79284291400159 - 0.85373472095314 * ( a0*a0 + h*h );\n"
"vec3 g;\n"
"g.x  = a0.x  * x0.x  + h.x  * x0.y;\n"
"g.yz = a0.yz * x12.xz + h.yz * x12.yw;\n"
"return 130.0 * dot(m, g);\n"
"}\n"
"void main() {\n"
"if (atPosition(gl_FragCoord.xy)) {\n"
"_COLOR = vec4(0.0, 0.0, 0.0, 0.0);\n"
"}\n"
"else {\n"
"float particleId = getParticleId(bufDims.x, gl_FragCoord.xy);\n"
"vec3 rnd = vec3(snoise(vec2(particleId, 0.0)),\n"
"snoise(vec2(0.0, particleId * 0.1)),\n"
"snoise(vec2(particleId * 0.1, particleId * 0.01)));\n"
"rnd *= 0.5;\n"
"rnd.y += 2.0;\n"
"_COLOR = vec4(rnd, 0.0);\n"
"}\n"
"}\n"
;
const char* updateFS_150_src = 
"#version 150\n"
"#define _TEXTURE2D texture\n"
"#define _COLOR _FragColor\n"
"uniform vec2 bufDims;\n"
"uniform sampler2D prevState;\n"
"uniform float numParticles;\n"
"in vec2 uv;\n"
"out vec4 _FragColor;\n"
"float getParticleId(float bufferWidth, vec2 fragCoord) {\n"
"vec2 xy = floor(fragCoord);\n"
"return (xy.y * bufferWidth * 0.5) + (xy.x * 0.5);\n"
"}\n"
"bool atPosition(vec2 fragCoord) {\n"
"return mod(floor(fragCoord.x), 2.0) < 0.5;\n"
"}\n"
"void getParticleStateUvs(bool atPos, vec2 bufDims, vec2 fragCoord, out vec2 posUv, out vec2 velUv) {\n"
"if (atPos) {\n"
"posUv = fragCoord / bufDims;\n"
"velUv = (fragCoord + vec2(1.0, 0.0)) / bufDims;\n"
"}\n"
"else {\n"
"posUv = (fragCoord + vec2(-1.0, 0.0)) / bufDims;\n"
"velUv = fragCoord / bufDims;\n"
"}\n"
"}\n"
"void main() {\n"
"const float frameTime = 1.0 / 60.0;\n"
"float particleId = getParticleId(bufDims.x, gl_FragCoord.xy);\n"
"if (particleId >= numParticles) {\n"
"discard;\n"
"}\n"
"bool atPos = atPosition(gl_FragCoord.xy);\n"
"vec2 posUv, velUv;\n"
"getParticleStateUvs(atPos, bufDims.xy, gl_FragCoord.xy, posUv, velUv);\n"
"vec4 pos = _TEXTURE2D(prevState, posUv);\n"
"vec4 vel = _TEXTURE2D(prevState, velUv);\n"
"if (atPos) {\n"
"pos += vel * frameTime;\n"
"_COLOR = pos;\n"
"}\n"
"else {\n"
"vec4 vel = _TEXTURE2D(prevState, velUv);\n"
"vel.y -= 1.0 * frameTime;\n"
"if ((pos.y < -1.0) && (vel.y < 0.0)) {\n"
"vel.y = -vel.y;\n"
"vel *= 0.8;\n"
"}\n"
"_COLOR = vel;\n"
"}\n"
"}\n"
;
const char* drawFS_150_src = 
"#version 150\n"
"#define _COLOR _FragColor\n"
"in vec4 color;\n"
"out vec4 _FragColor;\n"
"void main() {\n"
"_COLOR = color;\n"
"}\n"
;
ProgramBundleSetup InitParticles::CreateSetup() {
    ProgramBundleSetup setup("InitParticles");
    setup.AddProgramFromSources(0, ShaderLang::GLSL100, fsqVS_100_src, initFS_100_src);
    setup.AddProgramFromSources(0, ShaderLang::GLSL120, fsqVS_120_src, initFS_120_src);
    setup.AddProgramFromSources(0, ShaderLang::GLSL150, fsqVS_150_src, initFS_150_src);
    setup.AddUniform("bufDims", BufferDims);
    setup.AddUniform("time", Time);
    return setup;
}
ProgramBundleSetup DrawParticles::CreateSetup() {
    ProgramBundleSetup setup("DrawParticles");
    setup.AddProgramFromSources(0, ShaderLang::GLSL100, drawVS_100_src, drawFS_100_src);
    setup.AddProgramFromSources(0, ShaderLang::GLSL120, drawVS_120_src, drawFS_120_src);
    setup.AddProgramFromSources(0, ShaderLang::GLSL150, drawVS_150_src, drawFS_150_src);
    setup.AddUniform("mvp", ModelViewProjection);
    setup.AddUniform("bufDims", BufferDims);
    setup.AddTextureUniform("particleTex", ParticleState);
    return setup;
}
ProgramBundleSetup UpdateParticles::CreateSetup() {
    ProgramBundleSetup setup("UpdateParticles");
    setup.AddProgramFromSources(0, ShaderLang::GLSL100, fsqVS_100_src, updateFS_100_src);
    setup.AddProgramFromSources(0, ShaderLang::GLSL120, fsqVS_120_src, updateFS_120_src);
    setup.AddProgramFromSources(0, ShaderLang::GLSL150, fsqVS_150_src, updateFS_150_src);
    setup.AddUniform("bufDims", BufferDims);
    setup.AddTextureUniform("prevState", PrevState);
    setup.AddUniform("numParticles", NumParticles);
    return setup;
}
}
}
