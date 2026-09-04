#include "ZombieArena.h"
#include "Zombie.h"

const int SPAWN_MARGIN = 75;

Zombie* createHorde(int numZombies, IntRect arena)
{
    Zombie* zombies = new Zombie[numZombies];
    int maxY = arena.position.y + arena.size.y - SPAWN_MARGIN;
    int minY = arena.position.y + SPAWN_MARGIN;

    int maxX = arena.position.x + arena.size.x - SPAWN_MARGIN;
    int minX = arena.position.x + SPAWN_MARGIN;

    for (int i = 0; i < numZombies; i++)
    {
        
        //Which side should the zombie spawn
        srand((int)time(0) * i);
        int side = (rand() % 4);
        float x, y;
        switch (side)
        {

            case 0:
                // Left
                x = minX;
                y = (rand() % (maxY - minY)) + minY;
                break;
            case 1:

                // Right
                x = maxX;
                y = (rand() % (maxY - minY)) + minY;
                break;
            case 2:

                // Top
                x = (rand() % (maxX - minX)) + minX;
                y = minY;
                break;

            case 3:
                // Bottom
                x = (rand() % (maxX - minX)) + minX;
                y = maxY;
                break;

        }

        // Bloater , crawler or runner
        srand((int)time(0) * i * 2);
        int type = (rand() % 3);

        // Spawn the new zombie into the array
        zombies[i].spawn(x, y, type, i);
    }

    return zombies;

}
