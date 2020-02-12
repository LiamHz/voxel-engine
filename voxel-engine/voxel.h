#ifndef VOXEL_H
#define VOXEL_H

struct chunkInfo {
    unsigned int VBO = 0;
    long vboOffset = 0;
    int nAttrib = 0;
    chunkInfo(unsigned int _VBO, long _vboOffset, int _nAttrib)
              : VBO(_VBO), vboOffset(_vboOffset), nAttrib(_nAttrib)
    {}
};

std::vector<float> get_cube(int x, int y, int z) {
    float vertices[] = {
        // Postion         // TexCoords
        0.0f, 0.0f, 0.0f,  0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,  1.0f, 0.0f,
        1.0f, 1.0f, 0.0f,  1.0f, 1.0f,
        1.0f, 1.0f, 0.0f,  1.0f, 1.0f,
        0.0f, 1.0f, 0.0f,  0.0f, 1.0f,
        0.0f, 0.0f, 0.0f,  0.0f, 0.0f,
        
        0.0f, 0.0f, 1.0f,  0.0f, 0.0f,
        1.0f, 0.0f, 1.0f,  1.0f, 0.0f,
        1.0f, 1.0f, 1.0f,  1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,  1.0f, 1.0f,
        0.0f, 1.0f, 1.0f,  0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,  0.0f, 0.0f,
        
        0.0f, 1.0f, 1.0f,  1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,  1.0f, 1.0f,
        0.0f, 0.0f, 0.0f,  0.0f, 1.0f,
        0.0f, 0.0f, 0.0f,  0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,  0.0f, 0.0f,
        0.0f, 1.0f, 1.0f,  1.0f, 0.0f,
        
        1.0f, 1.0f, 1.0f,  1.0f, 0.0f,
        1.0f, 1.0f, 0.0f,  1.0f, 1.0f,
        1.0f, 0.0f, 0.0f,  0.0f, 1.0f,
        1.0f, 0.0f, 0.0f,  0.0f, 1.0f,
        1.0f, 0.0f, 1.0f,  0.0f, 0.0f,
        1.0f, 1.0f, 1.0f,  1.0f, 0.0f,
        
        0.0f, 0.0f, 0.0f,  0.0f, 1.0f,
        1.0f, 0.0f, 0.0f,  1.0f, 1.0f,
        1.0f, 0.0f, 1.0f,  1.0f, 0.0f,
        1.0f, 0.0f, 1.0f,  1.0f, 0.0f,
        0.0f, 0.0f, 1.0f,  0.0f, 0.0f,
        0.0f, 0.0f, 0.0f,  0.0f, 1.0f,
        
        0.0f, 1.0f, 0.0f,  0.0f, 1.0f,
        1.0f, 1.0f, 0.0f,  1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,  1.0f, 0.0f,
        1.0f, 1.0f, 1.0f,  1.0f, 0.0f,
        0.0f, 1.0f, 1.0f,  0.0f, 0.0f,
        0.0f, 1.0f, 0.0f,  0.0f, 1.0f,
    };
    
    std::vector<float> cube;
    int arrSize = sizeof(vertices)/sizeof(float);
    for (int i = 0; i < arrSize; i++) {
        int r = i % 5;
        cube.push_back((r == 0) ? vertices[i] + x :
                       (r == 1) ? vertices[i] + y :
                       (r == 2) ? vertices[i] + z :
                                  vertices[i]);
    }
    
    return cube;
}

std::vector<float> get_chunk(int x, int y, int z, int length, int width, int height) {
    std::vector<float> chunk;
    std::vector<float> cube;
    
    for (int l = 0; l < length; l++)
        for (int w = 0; w < width; w++)
            for (int h = 0; h < height; h++) {
                cube = get_cube(x + l, y + w, z + h);
                chunk.insert(chunk.end(), cube.begin(), cube.end());
            }
    
    return chunk;
}

int set_vertex_attribs(long vboOffset, int nAttrib) {
    // Configure vertex position and texture attributes
    glVertexAttribPointer(nAttrib, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(vboOffset));
    glEnableVertexAttribArray(nAttrib);
    glVertexAttribPointer(nAttrib+1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(vboOffset + 3 * sizeof(float)));
    glEnableVertexAttribArray(nAttrib+1);
    
    return 0;
}

void draw_chunk(std::vector<int> pos, std::vector<int> dim, chunkInfo &ci) {
    std::vector<float> chunk;
    set_vertex_attribs(ci.vboOffset, ci.nAttrib);
    ci.nAttrib += 2;
    
    int x = pos[0];
    int y = pos[1];
    int z = pos[2];
    
    int length = dim[0];
    int width  = dim[1];
    int height = dim[2];
    
    chunk = get_chunk(x, y, z, length, width, height);
    
    long chunkMemSize = chunk.size() * sizeof(float);
    glBufferSubData(GL_ARRAY_BUFFER, ci.vboOffset, chunkMemSize, chunk.data());
    
    ci.vboOffset += chunkMemSize;
}

#endif
