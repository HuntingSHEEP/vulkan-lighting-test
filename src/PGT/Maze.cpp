#include "PGT/Maze.h"
#include <ctime>


Maze::Maze(VulkanEngine* vkEngine, int sizeHorizontal, int sizeVertical, float cellSize, MazeBuildAlgorithm mazeBuildAlgorithm, glm::vec3 position){
    //Struktura siatki, umożliwia usuwanie ścianek w każdej z komórek. Ścianki są współdzielone
    gridMesh = new GridMesh(vkEngine, sizeHorizontal, sizeVertical, cellSize, position);

    if(mazeBuildAlgorithm == MAZE_BUILD_ALGORITHM_BINARY_TREE)
        generateMazeBinaryTree(sizeHorizontal, sizeVertical);
    else if(mazeBuildAlgorithm == MAZE_BUILD_ALGORITHM_SIDEWINDER)
       generateMazeSidewinder(sizeHorizontal, sizeVertical);

    //Po wybiciu ścian należy na nowo utworzyć index buffer
    gridMesh->update(vkEngine);
};

void Maze::generateMazeBinaryTree(int sizeHorizontal, int sizeVertical){
    //WYCINANIE POŁNOCNYCH KOMÓREK
    for(int a=0; a<sizeHorizontal-1; a++)
        gridMesh->getEdge(a, sizeVertical-1, EDGE_ORIENTATION_EAST)->isActive = false;

    //WYCINANIE WSCHODNICH KOMÓREK
    for(int a=0; a<sizeVertical-1; a++)
        gridMesh->getEdge(sizeHorizontal-1, a, EDGE_ORIENTATION_NORTH)->isActive = false;

    //SPACER PO POZOSTAŁYCH KOMÓRKACH
    srand(static_cast<unsigned int>(time(nullptr)));

    bool cutNorthWall;
    EdgeOrientation orientation;

    for(int y=0; y<sizeVertical-1; y++)
        for(int x=0; x<sizeHorizontal-1; x++){
            orientation = EDGE_ORIENTATION_EAST;
            cutNorthWall = (static_cast<float>(rand()) / RAND_MAX) < 0.5f;

            if(cutNorthWall)
                orientation = EDGE_ORIENTATION_NORTH;

            gridMesh->getEdge(x, y, orientation)->isActive = false;
        }
};

void Maze::generateMazeSidewinder(int sizeHorizontal, int sizeVertical){
    //WYCINANIE POŁNOCNYCH KOMÓREK
    for(int a=0; a<sizeHorizontal-1; a++)
        gridMesh->getEdge(a, sizeVertical-1, EDGE_ORIENTATION_EAST)->isActive = false;

    //SPACER PO POZOSTAŁYCH KOMÓRKACH
    srand(static_cast<unsigned int>(time(nullptr)));

    bool cutNorthWall;
    EdgeOrientation orientation;

    struct Cell{
        int x;
        int y;
    };

    std::vector<Cell> sciezka{};

    for(int y=0; y<sizeVertical-1; y++)
        for(int x=0; x<sizeHorizontal; x++){
            orientation = EDGE_ORIENTATION_EAST;
            cutNorthWall = (static_cast<float>(rand()) / RAND_MAX) < 0.5f;

            //Komórki w ostatniej kolumnie można wycinać conajwyżej na północ
            if(x==sizeHorizontal-1)
                cutNorthWall = true;

            if(cutNorthWall){
                orientation = EDGE_ORIENTATION_NORTH;
                Cell cell;

                if(0 < sciezka.size()){
                    int cellIndex = static_cast<int>((static_cast<float>(rand()) / RAND_MAX) * sciezka.size());
                    cell = sciezka[cellIndex];
                }else{
                    cell = {x,y};
                }

                gridMesh->getEdge(cell.x, cell.y, orientation)->isActive = false;
                sciezka = {};
            }else{
                //CUT EAST WALL
                sciezka.push_back({x,y});
                gridMesh->getEdge(x, y, orientation)->isActive = false;
            }
        }
};

DrawModelInfo& Maze::getDrawInfo(){
    return gridMesh->getDrawInfo();
};


