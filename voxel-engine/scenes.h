#ifndef SCENES_H
#define SCENES_H

using std::vector;

// Structs
struct chunkVecs {
    vector<vector<int>> &pos;
    vector<vector<int>> &dim;
    vector<int> &tex;
    chunkVecs(vector<vector<int>> &_pos, vector<vector<int>> &_dim, vector<int> &_tex)
              : pos(_pos), dim(_dim), tex(_tex) {}
};

// Functions
void draw_pizza(chunkVecs &cv);
void create_olive(int x, int z, int t, chunkVecs cv);
void create_pepper_r(int x, int z, int t, chunkVecs cv);
void create_pepper_l(int x, int z, int t, chunkVecs cv);
void create_pepperoni(int x, int z, int t, chunkVecs cv);
void create_chunk(vector<int> p, vector<int> d, int t, chunkVecs &cv);

void draw_scene(std::string sceneName, chunkVecs &cv) {
    (sceneName == "pizza") ? draw_pizza(cv)
                           : draw_pizza(cv);
}

void create_chunk(vector<int> p, vector<int> d, int t, chunkVecs &cv){
    cv.pos.push_back(p);
    cv.dim.push_back(d);
    cv.tex.push_back(t);
}

void draw_pizza(chunkVecs &cv) {
    int zIndex = 0;
    int depth;
    
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
