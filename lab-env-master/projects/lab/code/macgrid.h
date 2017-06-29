//
// Created by antchr-4 on 2/1/17.
//

#ifndef GSCEPT_LAB_ENV_MACGRID_H
#define GSCEPT_LAB_ENV_MACGRID_H

#ifdef __linux__ 
#include <tr1/unordered_map>
#include <vector>
#include <map>
#include <eigen3/Eigen/Sparse>

#elif _WIN32
#include <unordered_map>
#endif 


#include "cell.h"
#include <map>

class VectorXd;
typedef std::tr1::unordered_map<float, Cell*> MacTable1;

//Hashtable holding cells. Keyed by coordinates. hash=541x+79y+31z
class MacGrid
{
public:

    MacGrid(float width, float height, float depth, vector3D pos);

    virtual ~MacGrid();
    std::vector<vector3D> getNeighbourPos(float x, float y, float z);
    std::vector<vector3D> getNeighbourIndex(int x, int y, int z);
    void removeCell(float x, float y, float z);
    void addCell(float x, float y, float z, Cell* c);
    Cell* getCell(float x, float y, float z);
    bool cellInGrid(float x, float y, float z);
    bool cellInGrid(int x, int y, int z);
    bool cellInBounds(float x, float y, float z);
    Cell* getCellIndex(int x, int y, int z);
    std::tr1::unordered_map<float, Cell*>::iterator findCell(float x, float y, float z);
    std::tr1::unordered_map<float, Cell*>::iterator findCell(int x, int y, int z);
    std::vector<vector3D> cellPos;
    vector3D findClosestCell(vector3D pos);
    vector3D findIndex(vector3D pos);
    float getInterpolatedValue(float x, float y, float z, int index);
    float getLaplacianTimes(float x, float y, float z, int index);
    std::vector<vector3D> CellsToIndexVector();
    void setPressureFromIndex(VectorXd);
    void findNeighbours(vector3D pos, Cell* c);
    double getDivergence(vector3D pos);
    double getDivergenceInd(vector3D pos);
    int FlatIndex(vector3D p );
    int NumFluidAirNeighbours(vector3D);



    float rowCount = 20.f;
    float colCount = 20.f;
    float depthCount = 20.f;
    MacTable1 macCells;

    float cellWidth;
    float cellHeight;
    float cellDepth;
    float hCellWidth;
    float hCellHeight;
    float hCellDepth;

    std::vector<vector3D> indexVector;
    std::vector<vector3D> test;
    std::map<int, int> fluidVIndex;

private:

    float width;
    float height;
    float depth;

};




#endif //GSCEPT_LAB_ENV_MACGRID_H
