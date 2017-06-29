//
// Created by antchr-4 on 2/23/17.
//

#ifndef GSCEPT_LAB_ENV_GRIDHASH_H
#define GSCEPT_LAB_ENV_GRIDHASH_H


#include <vector>
#include <unordered_map>
#include "cell.h"

class MacTable
{
public:
    MacTable();
    MacTable(float width, float height, float depth, vector3D pos);
    bool isCellInGrid(int i, int j, int k);
    void insertCell(Cell *cell);
    void deleteCell(Cell *cell);
    Cell* getGridCell(int i, int j, int k);
    Cell* findGridCell(int i, int j, int k, bool *isGridCellFound);
    void getGridCells(std::vector<Cell*> *cells);
    std::unordered_map<long, std::vector<Cell*>> getIterator();
    vector3D FindClosestCell(vector3D pos);
    void SetLayers();
    void UpdateLayers(int maxLayers);
    std::vector<vector3D> getNeighbourPos(vector3D pos);
    std::vector<Cell*> getNeighbourCells(vector3D ind);
    bool cellInBounds(vector3D);
    float getInterpolatedValue(float x, float y, float z, int index);
    float getLaplacianTimes(float x, float y, float z, int index);
    double getDivergence(vector3D ind);
    int NumFluidAirNeighbours(Cell* c);
    void setNeighbours();
    double getExtrapolatedVelocity(int i, int j, int k, int idx, int vIndx);
    void ResetStagedVelocity();
    bool IsFaceBorderingTypeU(int i, int j, int k, Cell::cellType);
    bool IsFaceBorderingTypeV(int i, int j, int k, Cell::cellType);
    bool IsFaceBorderingTypeW(int i, int j, int k, Cell::cellType);
    bool IsBorderingIndexU(int i, int j, int k, int index);
    bool IsBorderingIndexV(int i, int j, int k, int index);
    bool IsBorderingIndexW(int i, int j, int k, int index);


    float rowCount = 20.f;
    float colCount = 20.f;
    float depthCount = 20.f;
    float halfRowC = rowCount*0.5f;
    float halfColC = colCount*0.5f;
    float halfdepthC = depthCount*0.5f;

    float cellWidth;
    float cellHeight;
    float cellDepth;
    float hCellWidth;
    float hCellHeight;
    float hCellDepth;

    float invCellWidht;
    float invCellHeight;
    float invCellDepth;

    float width;
    float height;
    float depth;
    float hWidth;
    float hHeight;
    float hDepth;

    std::unordered_map<long, std::vector<Cell*>> cellMap;


private:
    inline long computeHash(int i, int j, int k);

    long maxNumHashValues;
};


#endif //GSCEPT_LAB_ENV_GRIDHASH_H
