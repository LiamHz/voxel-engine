#ifndef VOXEL_H
#define VOXEL_H

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

void draw_chunk(int x, int y, int z, int length, int width, int height, unsigned int VBO, long &vboOffset) {
    std::vector<float> chunk;
    chunk = get_chunk(x, y, z, length, width, height);
    
    long chunkMemSize = chunk.size() * sizeof(float);
    glBufferSubData(GL_ARRAY_BUFFER, vboOffset, chunkMemSize, chunk.data());
    
    vboOffset += chunkMemSize;
}

#endif
