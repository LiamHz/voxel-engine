#ifndef SCENES_H
#define SCENES_H

using std::vector;

// Structs
struct chunkVecs {
    vector<vector<int>> &pos;
    vector<vector<int>> &dim;
    vector<int> &col;
    chunkVecs(vector<vector<int>> &_pos, vector<vector<int>> &_dim, vector<int> &_col)
              : pos(_pos), dim(_dim), col(_col) {}
};

// Functions
void create_chunk(vector<int> p, vector<int> d, int t, chunkVecs &cv);
void add_rgb_to_palette(std::vector<glm::vec3> &palette, int r, int g, int b);

void draw_king(chunkVecs &cv, std::vector<glm::vec3> &palette);
void draw_pizza(chunkVecs &cv, std::vector<glm::vec3> &palette);

void create_olive(int x, int z, int t, chunkVecs cv);
void create_pepper_r(int x, int z, int t, chunkVecs cv);
void create_pepper_l(int x, int z, int t, chunkVecs cv);
void create_pepperoni(int x, int z, int t, chunkVecs cv);

void draw_scene(std::string sceneName, chunkVecs &cv, std::vector<glm::vec3> &palette) {
    if (sceneName == "pizza")
        draw_pizza(cv, palette);
    else if (sceneName == "king")
        draw_king(cv, palette);
}

void create_chunk(vector<int> p, vector<int> d, int t, chunkVecs &cv){
    cv.pos.push_back(p);
    cv.dim.push_back(d);
    cv.col.push_back(t);
}

void add_rgb_to_palette(std::vector<glm::vec3> &palette, int r, int g, int b) {
    palette.push_back(glm::vec3(r / 255.0f, g / 255.0f, b / 255.0f));
}

void draw_king(chunkVecs &cv, std::vector<glm::vec3> &palette) {
    add_rgb_to_palette(palette, 128,   0,   0);
    add_rgb_to_palette(palette, 155,  28,  49);
    add_rgb_to_palette(palette, 255, 255, 255);
    add_rgb_to_palette(palette, 255, 229, 180);
    add_rgb_to_palette(palette, 250, 218,  94);
    
    // Left leg
    create_chunk({0, 0, 0}, {1, 1, 1}, 0, cv);
    create_chunk({0, 1, 0}, {1, 2, 1}, 1, cv);
    
    // Right leg
    create_chunk({3, 0, 0}, {1, 1, 1}, 0, cv);
    create_chunk({3, 1, 0}, {1, 2, 1}, 1, cv);
    
    // Torso
    create_chunk({-1, 3, -1}, {6, 1, 3}, 1, cv);
    create_chunk({-1, 4, -1}, {6, 1, 3}, 0, cv);
    create_chunk({-1, 5, -1}, {6, 3, 3}, 1, cv);
    
    // Beard
    create_chunk({ 1, 6,  2}, {2, 2, 1}, 2, cv);
    create_chunk({ 1, 8, -1}, {2, 1, 4}, 2, cv);
    create_chunk({ 1,10, -1}, {2, 1, 3}, 2, cv);
    
    // Face
    create_chunk({ 1, 9, -1}, {2, 1, 3}, 3, cv);
    
    // Crown
    create_chunk({ 1,10,  2}, {2, 1, 1}, 4, cv);
    create_chunk({ 1,10, -2}, {2, 1, 1}, 4, cv);
    create_chunk({ 0,10, -1}, {1, 1, 3}, 4, cv);
    create_chunk({ 3,10, -1}, {1, 1, 3}, 4, cv);
    create_chunk({ 0,11, -1}, {1, 1, 1}, 4, cv);
    create_chunk({ 3,11, -1}, {1, 1, 1}, 4, cv);
    create_chunk({ 0,11,  1}, {1, 1, 1}, 4, cv);
    create_chunk({ 3,11,  1}, {1, 1, 1}, 4, cv);
    
    // Left arm
    create_chunk({ -2, 5, 0}, {1, 2, 1}, 1, cv);
//    create_chunk({ -3, 5, 0}, {1, 1, 1}, 1, cv);
    create_chunk({ -2, 4, 0}, {1, 1, 1}, 3, cv);
    
    // Right arm
    create_chunk({  5, 6, 0}, {1, 1, 1}, 1, cv);
    create_chunk({  6, 5, 0}, {1, 1, 1}, 1, cv);
    create_chunk({  6, 4, 0}, {1, 1, 1}, 3, cv);
    
    // Cape
    create_chunk({ -2, 0,-2}, {8, 2, 1}, 0, cv);
    create_chunk({  0, 0,-3}, {4, 6, 1}, 0, cv);
    create_chunk({  1, 6,-3}, {2, 1, 1}, 0, cv);
    create_chunk({ -1, 2,-2}, {6, 4, 1}, 0, cv);
    create_chunk({  0, 6,-2}, {4, 2, 1}, 0, cv);
}

void draw_pizza(chunkVecs &cv, std::vector<glm::vec3> &palette) {
    int zIndex = 0;
    int depth;
    
    add_rgb_to_palette(palette, 253, 253, 150);
    add_rgb_to_palette(palette, 255, 105,  96);
    add_rgb_to_palette(palette, 119, 221, 119);
    add_rgb_to_palette(palette,   0,   0,   0);
    add_rgb_to_palette(palette, 131, 105,  83);
    
    // Cheese
    std::vector<int> cheeseDepth{2, 3, 2, 3, 2, 3, 2, 2, 3, 2, 3};
    
    // Cheese grows with pattern
    for (int i = 0; i < cheeseDepth.size(); i++) {
        depth = cheeseDepth[i];
        zIndex += depth;
        create_chunk({-i, 0, -zIndex}, {i*2 + 1, 1, depth}, 0, cv);
    }
    
    // At the back, cheese shrinks in width
    int s = (int)cheeseDepth.size();
    create_chunk({-s + 2, 0, -zIndex - 1}, {(s-2)*2 + 1, 1, 1}, 0, cv);
    create_chunk({-s + 5, 0, -zIndex - 2}, {(s-5)*2 + 1, 1, 1}, 0, cv);
    create_chunk({-s + 9, 0, -zIndex - 3}, {(s-9)*2 + 1, 1, 1}, 0, cv);
    // Cheese drip
    create_chunk({0, -3, -1}, {1, 3, 1}, 0, cv);
    create_chunk({0, -2, -2}, {1, 2, 1}, 0, cv);
    create_chunk({1, -2, -4}, {1, 2, 1}, 0, cv);
    create_chunk({5, -2,-14}, {1, 2, 1}, 0, cv);
    create_chunk({6, -4,-16}, {1, 4, 1}, 0, cv);
    create_chunk({8, -3,-21}, {1, 3, 1}, 0, cv);
    create_chunk({8, -2,-22}, {1, 2, 1}, 0, cv);
    
    // Pepperonis
    create_pepperoni(-1, -7, 1, cv);
    create_pepperoni( 2,-16, 1, cv);
    create_pepperoni(-6,-21, 1, cv);

    // Peppers
    create_pepper_r( 3, -12, 2, cv);
    create_pepper_l(-6, -18, 2, cv);
    create_pepper_l( 4, -24, 2, cv);

    // Olive
    create_olive(-3, -14, 3, cv);
    create_olive( 0, -27, 3, cv);

    // Crust
    // Left
    create_chunk({-11, 0, -28}, {2, 2, 1}, 4, cv);
    create_chunk({-11, 0, -29}, {5, 2, 1}, 4, cv);
    create_chunk({-11, 0, -30}, {9, 2, 1}, 4, cv);
    // Middle
    create_chunk({ -9, 0, -31}, {19, 2, 1},4, cv);
    create_chunk({ -6, 0, -32}, {13, 2, 1},4, cv);
    create_chunk({ -2, 0, -33}, {5, 2, 1}, 4, cv);
    // Right
    create_chunk({ 10, 0, -28}, {2, 2, 1}, 4, cv);
    create_chunk({  7, 0, -29}, {5, 2, 1}, 4, cv);
    create_chunk({  3, 0, -30}, {9, 2, 1}, 4, cv);
    // Top
    create_chunk({-10, 2, -29}, {1, 1, 1}, 4, cv);
    create_chunk({ -9, 2, -30}, {3, 1, 1}, 4, cv);
    create_chunk({ -6, 2, -31}, {4, 1, 1}, 4, cv);
    create_chunk({ -2, 2, -32}, {5, 1, 1}, 4, cv);
    create_chunk({  3, 2, -31}, {4, 1, 1}, 4, cv);
    create_chunk({  7, 2, -30}, {3, 1, 1}, 4, cv);
    create_chunk({ 10, 2, -29}, {1, 1, 1}, 4, cv);
}

void create_pepperoni(int x, int z, int t, chunkVecs cv) {
    create_chunk({  x,  1,  z}, { 3,  1,  1}, t, cv);
    create_chunk({x-1,  1,z-1}, { 5,  1,  1}, t, cv);
    create_chunk({x-2,  1,z-4}, { 7,  1,  3}, t, cv);
    create_chunk({x-1,  1,z-5}, { 5,  1,  1}, t, cv);
    create_chunk({  x,  1,z-6}, { 3,  1,  1}, t, cv);
}

void create_pepper_r(int x, int z, int t, chunkVecs cv) {
    create_chunk({  x,  1,  z}, { 2,  1,  1}, t, cv);
    create_chunk({x-1,  1,z-1}, { 3,  1,  1}, t, cv);
    create_chunk({x-1,  1,z-2}, { 2,  1,  1}, t, cv);
}

void create_pepper_l(int x, int z, int t, chunkVecs cv) {
    create_chunk({  x,  1,  z}, { 2,  1,  1}, t, cv);
    create_chunk({  x,  1,z-1}, { 3,  1,  1}, t, cv);
    create_chunk({x+1,  1,z-2}, { 2,  1,  1}, t, cv);
}

void create_olive(int x, int z, int t, chunkVecs cv) {
    create_chunk({  x,  1,  z}, { 1,  1,  1}, t, cv);
    create_chunk({x-1,  1,z-1}, { 1,  1,  1}, t, cv);
    create_chunk({x+1,  1,z-1}, { 1,  1,  1}, t, cv);
    create_chunk({  x,  1,z-2}, { 1,  1,  1}, t, cv);
}

#endif
