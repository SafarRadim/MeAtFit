/*!
 * @file
 * @brief This file contains implementation of gpu
 *
 * @author Tomáš Milet, imilet@fit.vutbr.cz
 */

#include <student/gpu.hpp>
#include <stdlib.h>

struct Triangle {
    OutVertex points[3];
};

uint32_t computeVertexID(VertexArray const& vao, uint32_t shaderInvocation)
{
    if (!vao.indexBuffer) {
        return shaderInvocation;
    }

    if (vao.indexType == IndexType::UINT32) {
        uint32_t* ind = (uint32_t*)vao.indexBuffer;
        return ind[shaderInvocation];
    }

    if (vao.indexType == IndexType::UINT16) {
        uint16_t* ind = (uint16_t*)vao.indexBuffer;
        return ind[shaderInvocation];
    }

    if (vao.indexType == IndexType::UINT8) {
        uint8_t* ind = (uint8_t*)vao.indexBuffer;
        return ind[shaderInvocation];
    }
}

void computeVertexAttributes(VertexAttrib const attrib[maxAttributes], InVertex *inVertex) 
{
    for (uint32_t i = 0; i < maxAttributes; i++) {
        uint32_t ID = inVertex->gl_VertexID;
        uint64_t stride = (attrib[i].stride);
        uint64_t offset = (attrib[i].offset);
        uint8_t* buffer = (uint8_t*)attrib[i].bufferData;
        uint8_t* ptr;

        switch (attrib[i].type) {
            case AttributeType::EMPTY:
                continue;

            case AttributeType::FLOAT:
                ptr = buffer + offset + stride * ID;
                inVertex->attributes[i].v1 = *((float*)ptr);
                break;

            case AttributeType::VEC2:
                for (uint32_t j = 0; j < 2; j++) {
                    ptr = buffer + offset + stride * ID + j * 4;
                    inVertex->attributes[i].v2[j] = *((float*)ptr);
                }
                break;

            case AttributeType::VEC3:
                for (uint32_t j = 0; j < 3; j++) {
                    ptr = buffer + offset + stride * ID + j * 4;
                    inVertex->attributes[i].v3[j] = *((float*)ptr);
                }
                break;

            case AttributeType::VEC4:
                for (uint32_t j = 0; j < 4; j++) {
                    ptr = buffer + offset + stride * ID + j * 4;
                    inVertex->attributes[i].v4[j] = *((float*)ptr);
                }
                break;
        }
    }
}


void runVertexAseembly(InVertex *inVertex, VertexArray vao, uint32_t shader) 
{
    inVertex->gl_VertexID = computeVertexID(vao, shader);
    computeVertexAttributes(vao.vertexAttrib, inVertex);
}

void runPrimitiveAsembly(Triangle *triangle, VertexArray vao, uint32_t tId, Program prg)
{
    for (int i = 0; i < 3; i++) {
        InVertex inVertex;
        runVertexAseembly(&inVertex, vao, tId + i);
        prg.vertexShader(triangle->points[i], inVertex, prg.uniforms);
    }
}

void runPerspectiveDivision(Triangle* triangle)
{
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            triangle->points[i].gl_Position[j] /= triangle->points[i].gl_Position[3];
        }
    }
}

void runViewportTransformation(Triangle* triangle, Frame frame)
{
    for (int i = 0; i < 3; i++) {
        float* x = &triangle->points[i].gl_Position[0];
        float* y = &triangle->points[i].gl_Position[1];

        *x = (*x * 0.5 + 0.5) * frame.width;
        *y = (*y * 0.5 + 0.5) * frame.height;
    }
}

void createFragment(GPUContext ctx, Triangle* triangle, int x, int y, InFragment *inFragment)
{
    float z0 = triangle->points[0].gl_Position[2];
    float z1 = triangle->points[1].gl_Position[2];
    float z2 = triangle->points[2].gl_Position[2];

    inFragment->gl_FragCoord[0] = x;
    inFragment->gl_FragCoord[1] = y;
    inFragment->gl_FragCoord[2] = z0 + z1 + z2;
}

void rasterizeTriangle(Frame frame, Triangle* triangle, Program prg, GPUContext ctx)
{
    // Stolen from student.cpp, izg_lab_03
    int minX = std::min(triangle->points[0].gl_Position[0], std::min(triangle->points[1].gl_Position[0], triangle->points[2].gl_Position[0]));
    int maxX = std::max(triangle->points[0].gl_Position[0], std::max(triangle->points[1].gl_Position[0], triangle->points[2].gl_Position[0]));

    int minY = std::min(triangle->points[0].gl_Position[1], std::min(triangle->points[1].gl_Position[1], triangle->points[2].gl_Position[1]));
    int maxY = std::max(triangle->points[0].gl_Position[1], std::max(triangle->points[1].gl_Position[1], triangle->points[2].gl_Position[1]));

    // Still stealing from that lab
    if (minX < 0) { minX = 0; }
    if (maxX > frame.width) { maxX = frame.width - 1; }
    if (minY < 0) { minY = 0; }
    if (maxY > frame.height) { maxY = frame.height - 1; }

    // The secret ingredient is still a crime
    // 1->2
    int dx1 = triangle->points[0].gl_Position[0] - triangle->points[1].gl_Position[0];
    int dy1 = triangle->points[0].gl_Position[1] - triangle->points[1].gl_Position[0];

    // 2->3
    int dx2 = triangle->points[1].gl_Position[0] - triangle->points[2].gl_Position[0];
    int dy2 = triangle->points[1].gl_Position[0] - triangle->points[2].gl_Position[0];

    // 3->1
    int dx3 = triangle->points[2].gl_Position[0] - triangle->points[0].gl_Position[0];
    int dy3 = triangle->points[2].gl_Position[0] - triangle->points[0].gl_Position[1];
    

    if (ctx.backfaceCulling) {
        // v1x * v2x + v1y * v2y
        uint64_t v1x = triangle->points[0].gl_Position[0] - triangle->points[1].gl_Position[0];
        uint64_t v1y = triangle->points[0].gl_Position[1] - triangle->points[1].gl_Position[1];

        uint64_t v2x = triangle->points[2].gl_Position[0] - triangle->points[1].gl_Position[0];
        uint64_t v2y = triangle->points[2].gl_Position[1] - triangle->points[1].gl_Position[1];

        int dot = v1x * v2x + v1y * v2y;
        if (dot > 0) {
            return;
        }
    }

    for (uint32_t x = 0; x < frame.width; x++) {
        for (uint32_t y = 0; y < frame.height; y++) {
            if( // I really should stop stealing from my own student.cpp, if only it worked as intended tho
                (y - triangle->points[0].gl_Position[0]) * dy1 - (x- triangle->points[0].gl_Position[1]) * dx1 >= 0 &&
                (y - triangle->points[1].gl_Position[0]) * dy2 - (x - triangle->points[1].gl_Position[0]) * dx2 >= 0 &&
                (y - triangle->points[2].gl_Position[0]) * dy3 - (x - triangle->points[2].gl_Position[0]) * dx3 >= 0) {
                
                InFragment inFragment;
                createFragment(ctx, triangle, x, y, &inFragment);

                OutFragment outFragment;
                prg.fragmentShader(outFragment, inFragment, prg.uniforms);
            }
        }
    }
}

//! [drawImpl]
void drawImpl(GPUContext &ctx,uint32_t nofVertices){
    for (uint32_t i = 0; i < nofVertices; i+=3){
        Triangle triangle;
        runPrimitiveAsembly(&triangle, ctx.vao, i, ctx.prg);

        runPerspectiveDivision(&triangle);
        runViewportTransformation(&triangle, ctx.frame);
        rasterizeTriangle(ctx.frame, &triangle, ctx.prg, ctx);
    }
}
//! [drawImpl]

/**
 * @brief This function reads color from texture.
 *
 * @param texture texture
 * @param uv uv coordinates
 *
 * @return color 4 floats
 */
glm::vec4 read_texture(Texture const&texture,glm::vec2 uv){
  if(!texture.data)return glm::vec4(0.f);
  auto uv1 = glm::fract(uv);
  auto uv2 = uv1*glm::vec2(texture.width-1,texture.height-1)+0.5f;
  auto pix = glm::uvec2(uv2);
  //auto t   = glm::fract(uv2);
  glm::vec4 color = glm::vec4(0.f,0.f,0.f,1.f);
  for(uint32_t c=0;c<texture.channels;++c)
    color[c] = texture.data[(pix.y*texture.width+pix.x)*texture.channels+c]/255.f;
  return color;
}

/**
 * @brief This function clears framebuffer.
 *
 * @param ctx GPUContext
 * @param r red channel
 * @param g green channel
 * @param b blue channel
 * @param a alpha channel
 */
void clear(GPUContext&ctx,float r,float g,float b,float a){
  auto&frame = ctx.frame;
  auto const nofPixels = frame.width * frame.height;
  for(size_t i=0;i<nofPixels;++i){
    frame.depth[i] = 10e10f;
    frame.color[i*4+0] = static_cast<uint8_t>(glm::min(r*255.f,255.f));
    frame.color[i*4+1] = static_cast<uint8_t>(glm::min(g*255.f,255.f));
    frame.color[i*4+2] = static_cast<uint8_t>(glm::min(b*255.f,255.f));
    frame.color[i*4+3] = static_cast<uint8_t>(glm::min(a*255.f,255.f));
  }
}

