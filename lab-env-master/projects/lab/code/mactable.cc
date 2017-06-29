//
// Created by antchr-4 on 2/23/17.
//

#include "mactable.h"

MacTable::MacTable()
{
    maxNumHashValues = 10000;
}

long MacTable::computeHash(int i, int j, int k)
{
    return (abs(541*(long)i + 79*(long)j + 31*(long)k) % maxNumHashValues);
}

bool MacTable::isCellInGrid(int i, int j, int k)
{
    long h = computeHash(i, j, k);

    if (cellMap.find(h) == cellMap.end()) {
        return false;
    }

    Cell *c = NULL;
    std::vector<Cell*> chain = cellMap[h];
    for (int idx=0; idx<(int)chain.size(); idx++) {
        c = chain[idx];
		if (c->i == i && c->j == j && c->k == k) {
            return true;
        }
    }

    return false;
}

void MacTable::insertCell(Cell *cell)
{
    long h = computeHash(cell->i, cell->j, cell->k);

    if (cellMap.find(h) == cellMap.end()) {
        std::vector<Cell*> newChain;
        std::pair<long,std::vector<Cell*>> pair(h, newChain);
        cellMap.insert(pair);
    }

    cellMap[h].push_back(cell);
}

void MacTable::deleteCell(Cell *cell)
{
    int i = cell->i;
    int j = cell->j;
    int k = cell->k;
    long h = computeHash(i, j, k);

    if (cellMap.find(h) == cellMap.end()) {
        return;
    }

    // remove from hash chain
    bool isRemoved = false;
    std::vector<Cell*> chain = cellMap[h];
    for (int idx=0; idx<(int)chain.size(); idx++) {
        Cell *c = (cellMap[h])[idx];
        if (c->i == i && c->j == j && c->k == k) {
            cellMap[h].erase(cellMap[h].begin() + idx);
            isRemoved = true;
            break;
        }
    }

    if (!isRemoved) {
        //qDebug() << "Could not find/remove gridcell" << i << j << k;
    }

    // remove chain from map if empty
    if (chain.size() == 0) {
        cellMap.erase(h);
    }

}

Cell *MacTable::getGridCell(int i, int j, int k)
{
    long h = computeHash(i, j, k);

    Cell *c = NULL;
    std::vector<Cell*> chain = cellMap[h];
    for (int idx=0; idx<(int)chain.size(); idx++) {
        c = chain[idx];
        if (c->i == i && c->j == j && c->k == k) {
            return c;
        }
    }

    return c;
}

Cell *MacTable::findGridCell(int i, int j, int k, bool *isGridCellFound)
{
    long h = computeHash(i, j, k);

    Cell *c = NULL;
    std::vector<Cell*> chain = cellMap[h];
    for (int idx=0; idx<(int)chain.size(); idx++) {
        c = chain[idx];
        if (c->i == i && c->j == j && c->k == k) {
            *isGridCellFound = true;
            return c;
        }
    }

    *isGridCellFound = false;
    return c;
}

void MacTable::getGridCells(std::vector<Cell *> *cells)
{
    for (std::pair<int, std::vector<Cell*>> pair: cellMap)
    {
        for (int i=0; i < (int)pair.second.size(); i++)
        {
            cells->push_back(pair.second[i]);
        }
    }
}

std::unordered_map<long, std::vector<Cell *>> MacTable::getIterator()
{
    return this->cellMap;
}

vector3D MacTable::FindClosestCell(vector3D pos)
{
    float x = pos.x();
    float y = pos.y();
    float z = pos.z();

    int indexX = (pos.x() * invCellWidht);
    int indexY = (pos.y() * invCellHeight);
    int indexZ = (pos.z() * invCellDepth);

    float xP = x > 0.f ? (indexX) * cellWidth + hCellWidth : (indexX) * cellWidth - hCellWidth;
    float yP = y > 0.f ? (indexY) * cellHeight + hCellHeight : (indexY) * cellHeight - hCellHeight;
    float zP = z > 0.f ? (indexZ) * cellDepth + hCellDepth : (indexZ) * cellDepth - hCellDepth;

    return vector3D(xP, yP, zP);
}

void MacTable::SetLayers()
{
    for (auto it : this->cellMap)
    {
        Cell *c;
        std::vector<Cell*> chain = it.second;
        for (int idx=0; idx<(int)chain.size(); idx++)
        {
            c = chain[idx];
            c->layer = -1;
            c->fluidIndex = -1;
        }
    }
}

MacTable::MacTable(float width, float height, float depth, vector3D pos) : width(width),  height(height),  depth(depth)
{
    cellMap.rehash(rowCount*colCount*depthCount);
    cellWidth = width/rowCount;
    cellHeight = height/colCount;
    cellDepth = depth/depthCount;

    hCellWidth = cellWidth * 0.5f;
    hCellHeight = cellHeight * 0.5f;
    hCellDepth = cellDepth * 0.5f;

    hHeight = height * 0.5f;
    hWidth = width * 0.5f;
    hDepth = depth * 0.5f;

    invCellWidht = 1.f/cellWidth;
    invCellHeight = 1.f/cellHeight;
    invCellDepth = 1.f/cellDepth;

    maxNumHashValues = 10000;

}

std::vector<vector3D> MacTable::getNeighbourPos(vector3D pos)
{
    vector3D p = FindClosestCell(pos);
    float xb = p.x();
    float yb = p.y();
    float zb = p.z();


    std::vector<vector3D> neighbours;

    neighbours.push_back(vector3D(xb+cellWidth,   yb,            zb));
    neighbours.push_back(vector3D(xb-cellWidth,   yb,            zb));
    neighbours.push_back(vector3D(xb,             yb+cellWidth, zb));
    neighbours.push_back(vector3D(xb,             yb-cellWidth, zb));
    neighbours.push_back(vector3D(xb,             yb,            zb+cellWidth));
    neighbours.push_back(vector3D(xb,             yb,            zb-cellWidth));
    return neighbours;
}

bool MacTable::cellInBounds(vector3D pos)
{
    float x = pos.x();
    float y = pos.y();
    float z = pos.z();

    if ((x <= (width/2.f)-cellWidth && x >= (-width/2.f+cellWidth)) && (y <= (height/2.f)-cellWidth && y >= (-height/2.f)+cellWidth) && (z <= (depth/2.f)-cellWidth && z >= (-depth/2.f)+cellWidth))
    {
        return true;
    }
    else
    {
        return false;
    }
}

float MacTable::getInterpolatedValue(float x, float y, float z, int index)
{
    int i = std::floor(x);
    int j = std::floor(y);
    int k = std::floor(z);

    float out = (i+1-x) * (j+1-y) * (k+1-z) * getGridCell(i, j, k)->velocity[index];
    bool inGrid = false;

    Cell* c = findGridCell(i, j, k, &inGrid);
    if (inGrid)
    {
        out = (i+1-x) * (j+1-y) * (k+1-z) * getGridCell(i, j, k)->velocity[index];
    }
    c = findGridCell(i+1, j, k, &inGrid);
    if (inGrid)
    {
        out += (x-i) * (j+1-y) * (k+1-z) *  c->velocity[index];
    }

    c = findGridCell(i, j+1, k, &inGrid);
    if (inGrid)
    {
        out += (i+1-x) * (y-j) * (k+1-z) *  c->velocity[index];
    }

    c = findGridCell(i+1, j+1, k, &inGrid);
    if (inGrid)
    {
        out += (x-1) * (y-j) * (k+1-z) * c->velocity[index];
    }

    c = findGridCell(i, j, k+1, &inGrid);
    if (inGrid)
    {
        out += (i+1-x) * (j+1-y) * (z-k) * c->velocity[index];
    }

    c = findGridCell(i+1, j, k+1, &inGrid);
    if (inGrid)
    {
        out += (x-i) * (j+1-y) * (z-k) * c->velocity[index];
    }

    c = findGridCell(i, j+1, k+1, &inGrid);
    if (inGrid)
    {
        out += (i+1-x) * (y-j) * (z-k) * c->velocity[index];
    }

    c = findGridCell(i+1, j+1, k+1, &inGrid);
    if (inGrid)
    {
        out += (x-i) * (y-j) * (z-k) * c->velocity[index];
    }

    return out ;
}

float MacTable::getLaplacianTimes(float x, float y, float z, int index)
{
    //int i = std::floor(x / cellWidth);    //Why divide by cell dim??
    int i = std::floor(x);
    //int j = std::floor(y / cellHeight);
    int j = std::floor(y);
    //int k = std::floor(z / cellDepth);
    int k = std::floor(z);

    return getGridCell(i + 1, j, k)->velocity[index] + getGridCell(i - 1 , j, k)->velocity[index] +
            getGridCell(i, j + 1, k)->velocity[index] + getGridCell(i, j -1 , k)->velocity[index] +
            getGridCell(i, j, k + 1)->velocity[index] + getGridCell(i, j, k - 1 )->velocity[index] - 6 * getGridCell(i, j, k)->velocity[index];

}

double MacTable::getDivergence(vector3D ind)
{
    return (getGridCell(ind.x() + 1, ind.y(), ind.z())->velocity.x() - getGridCell(ind.x(), ind.y(), ind.z())->velocity.x()+
            getGridCell(ind.x(), ind.y() + 1, ind.z())->velocity.y() - getGridCell(ind.x(), ind.y(), ind.z())->velocity.y()+
            getGridCell(ind.x(), ind.y(), ind.z() + 1)->velocity.z() - getGridCell(ind.x(), ind.y(), ind.z())->velocity.z());

}

int MacTable::NumFluidAirNeighbours(Cell* c)
{
    int n = 0;
    if (c->rightN->type != Cell::SOLID) { n++; }
    if (c->leftN->type != Cell::SOLID) { n++; }
    if (c->upN->type != Cell::SOLID) { n++; }
    if (c->downN->type != Cell::SOLID) { n++; }
    if (c->inN->type != Cell::SOLID) { n++; }
    if (c->outN->type != Cell::SOLID) { n++; }
    return n;
}

void MacTable::setNeighbours()
{
    for (auto it : cellMap)
    {
        Cell *itc;
        std::vector<Cell *> chain = it.second;
        for (int idx = 0; idx < (int) chain.size(); idx++)
        {
            itc = chain[idx];
            int i = itc->i;
            int j = itc->j;
            int k = itc->k;

            bool inGrid = false;
            Cell* c;

            c = findGridCell(i-1,j,k, &inGrid);
            if (inGrid)
            {
                itc->leftN = c;
            }
            c = findGridCell(i+1,j,k, &inGrid);
            if ( inGrid)
            {
                itc->rightN = c;
            }
            c = findGridCell(i,j+1,k, &inGrid);
            if ( inGrid)
            {
                itc->upN = c;
            }
            c = findGridCell(i,j-1,k, &inGrid);
            if ( inGrid)
            {
                itc->downN = c;
            }
            c = findGridCell(i,j,k+1, &inGrid);
            if ( inGrid)
            {
                itc->outN = c;
            }
            c = findGridCell(i,j,k-1, &inGrid);
            if ( inGrid)
            {
                itc->inN = c;
            }
        }
    }
}

double MacTable::getExtrapolatedVelocity(int i, int j, int k, int idx, int vIndx)
{
    std::vector<Cell*> neigh = getNeighbourCells(vector3D(i, j, k));

    double sum = 0.0;
    double weightsum = 0.0;


    for (int l = 0; l < 6; ++l)
    {
        Cell* c = neigh[l];
        if (isCellInGrid(c->i, c->j, c->k) && c->layer - 1 == idx)
        {
            sum += c->velocity[vIndx];
            weightsum++;
        }
    }
    if (sum == 0.0)
        return 0.0;

    return sum/weightsum;
}

std::vector<Cell*> MacTable::getNeighbourCells(vector3D ind)
{
    int i = ind.x();
    int j = ind.y();
    int k = ind.z();

    std::vector<Cell*> neighbours;
    neighbours.push_back(getGridCell(i+1,   j,    k));
    neighbours.push_back(getGridCell(i-1,   j,    k));
    neighbours.push_back(getGridCell(i,     j+1,  k));
    neighbours.push_back(getGridCell(i,     j-1,  k));
    neighbours.push_back(getGridCell(i,     j,    k+1));
    neighbours.push_back(getGridCell(i,     j,    k-1));
    return neighbours;
}

void MacTable::UpdateLayers(int maxLayers)
{
    for (int k = 0; k < depthCount; k++){
        for (int j = 0; j < rowCount; j++){
            for (int i = 0; i < colCount; i++){
                bool inGrid = false;
                Cell* c = findGridCell(i, j, k, &inGrid);
                if (!inGrid)
                    continue;

                if (c->type == Cell::FLUID)
                {
                    c->layer = 0;
                }
            }
        }
    }
    for (int layer = 1; layer <= maxLayers; layer++)
    {
        for (int k = 0; k < depthCount+1; k++){
            for (int j = 0; j < rowCount; j++){
                for (int i = 0; i < colCount; i++){

                    bool inGrid = false;
                    Cell* c = findGridCell(i, j, k, &inGrid);
                    if (!inGrid)
                        continue;
                    if (c->layer == layer - 1 && c->type != Cell::SOLID)
                    {
                        std::vector<Cell*> neigh = getNeighbourCells(vector3D(i, j, k));
                        for (int l = 0; l < 6; ++l)
                        {
                            Cell* n = neigh[l];
                            if (isCellInGrid(n->i, n->j, n->k) && n->type != Cell::SOLID && n->layer == -1)
                            {
                                n->layer = layer;
                            }
                        }
                    }
                }
            }
        }
    }
}

void MacTable::ResetStagedVelocity()
{
    for (auto it : cellMap)
    {
        Cell *itc;
        std::vector<Cell *> chain = it.second;
        for (int idx = 0; idx < (int) chain.size(); idx++)
        {
            itc = chain[idx];
            itc->stagedVel = vector3D(0.0f,0.0f,0.0f);
        }
    }

}

bool MacTable::IsFaceBorderingTypeU(int i, int j, int k, Cell::cellType type)
{
    Cell* c = getGridCell(i, j, k);
    if (c->leftN)
    {
        return c->type == type || c->leftN->type == type;
    }
    else
    {
        return c->type == type;
    }
}

bool MacTable::IsFaceBorderingTypeV(int i, int j, int k, Cell::cellType type)
{
    Cell* c = getGridCell(i, j, k);
    if (c->downN)
    {
        return c->type == type || c->downN->type == type;
    }
    else
    {
        return c->type == type;
    }
}

bool MacTable::IsFaceBorderingTypeW(int i, int j, int k, Cell::cellType type)
{
    Cell* c = getGridCell(i, j, k);
    if (c->inN)
    {
        return c->type == type || c->inN->type == type;
    }
    else
    {
        return c->type == type;
    }
}

bool MacTable::IsBorderingIndexU(int i, int j, int k, int index)
{
    Cell* c = getGridCell(i, j, k);
    if (c->leftN)
    {
        return c->layer == index || c->leftN->layer == index;
    }
    else
    {
        return c->layer == index;
    }
}

bool MacTable::IsBorderingIndexV(int i, int j, int k, int index)
{
    Cell* c = getGridCell(i, j, k);
    if (c->downN)
    {
        return c->layer == index || c->downN->layer == index;
    }
    else
    {
        return c->layer == index;
    }
}

bool MacTable::IsBorderingIndexW(int i, int j, int k, int index)
{
    Cell* c = getGridCell(i, j, k);
    if (c->inN)
    {
        return c->layer == index || c->inN->layer == index;
    }
    else
    {
        return c->layer == index;
    }
}




