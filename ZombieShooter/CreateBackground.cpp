#include "ZombieArena.h"

int createBackground(VertexArray& rVA, IntRect arena)
{
    const int TILE_SIZE = 50;
    const int TILE_TYPES = 3;
    const int VERTS_IN_QUAD = 6; // 2 triangles = 6 vertices

    int worldWidth = arena.size.x / TILE_SIZE;
    int worldHeight = arena.size.y / TILE_SIZE;

    // SFML 3: no Quads.
    // Cada tile estará formado por 2 triangles.
    rVA.setPrimitiveType(PrimitiveType::Triangles);

    rVA.resize(worldWidth * worldHeight * VERTS_IN_QUAD);

    int currentVertex = 0;

    for (int w = 0; w < worldWidth; w++)
    {
        for (int h = 0; h < worldHeight; h++)
        {
            float left   = w * TILE_SIZE;
            float right  = left + TILE_SIZE;
            float top    = h * TILE_SIZE;
            float bottom = top + TILE_SIZE;

            // Triangle 1
            rVA[currentVertex + 0].position = Vector2f(left, top);
            rVA[currentVertex + 1].position = Vector2f(right, top);
            rVA[currentVertex + 2].position = Vector2f(right, bottom);

            // Triangle 2
            rVA[currentVertex + 3].position = Vector2f(left, top);
            rVA[currentVertex + 4].position = Vector2f(right, bottom);
            rVA[currentVertex + 5].position = Vector2f(left, bottom);

            // Decide which texture to use
            int verticalOffset;

            if (h == 0 || h == worldHeight - 1 ||
                w == 0 || w == worldWidth - 1)
            {
                // Wall texture
                verticalOffset = TILE_TYPES * TILE_SIZE;
            }
            else
            {
                // Random floor texture
                srand((int)time(0) + h * w - h);

                int mOrG = rand() % TILE_TYPES;
                verticalOffset = mOrG * TILE_SIZE;
            }

            // Texture coordinates for triangle 1
            rVA[currentVertex + 0].texCoords =
                Vector2f(0, verticalOffset);

            rVA[currentVertex + 1].texCoords =
                Vector2f(TILE_SIZE, verticalOffset);

            rVA[currentVertex + 2].texCoords =
                Vector2f(TILE_SIZE, verticalOffset + TILE_SIZE);

            // Texture coordinates for triangle 2
            rVA[currentVertex + 3].texCoords =
                Vector2f(0, verticalOffset);

            rVA[currentVertex + 4].texCoords =
                Vector2f(TILE_SIZE, verticalOffset + TILE_SIZE);

            rVA[currentVertex + 5].texCoords =
                Vector2f(0, verticalOffset + TILE_SIZE);

            // Move to the next tile
            currentVertex += VERTS_IN_QUAD;
        }
    }

    return TILE_SIZE;
}