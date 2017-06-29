//
// Created by antchr-4 on 2/1/17.
//

#include "macgrid.h"
#include "pressuresolver.h"
#include <cassert>


void MacGrid::removeCell(float x, float y, float z)
{
    int indexX = std::floor(x / cellWidth);
    int indexY = std::floor(y / cellHeight);
    int indexZ = std::floor(z / cellDepth);
    int hash = 541 * indexX + 79 * indexY + 31 * indexZ;
    std::tr1::unordered_map<float, Cell*>::iterator it = macCells.find(hash);
    delete(it->second);
    macCells.erase(it->first);
}

void MacGrid::addCell(float x, float y, float z, Cell* c)
{
    assert(!cellInGrid(x, y, z) && "Cell already in grid damnit");
    int indexX = std::floor(x / cellWidth);
    int indexY = std::floor(y / cellHeight);
    int indexZ = std::floor(z / cellDepth);
	int hash = 541 * indexX + 79 * indexY + 31 * indexZ;
    c->position = vector3D(x, y, z);
    cellPos.push_back(vector3D(x,y,z));
    macCells[hash] = c;
}

Cell *MacGrid::getCell(float x, float y, float z)
{

    assert(cellInGrid(x, y, z) && "CELL NOT IN GRID");
    int indexX = std::floor(x / cellWidth);
        int indexY = std::floor(y / cellHeight);
        int indexZ = std::floor(z / cellDepth);
        int hash = 541 * indexX + 79 * indexY + 31 * indexZ;
        return macCells[hash];
}

std::tr1::unordered_map<float, Cell *>::iterator MacGrid::findCell(float x, float y, float z)
{
    int indexX = std::floor(x / cellWidth);
    int indexY = std::floor(y / cellHeight);
    int indexZ = std::floor(z / cellDepth);
    int hash = 541 * indexX + 79 * indexY + 31 * indexZ;
    return macCells.find(hash);
}

MacGrid::MacGrid(float width, float height, float depth, vector3D pos) : width(width), height(height), depth(depth)
{
    macCells.rehash(rowCount*colCount*depthCount);
    cellWidth = width/rowCount;
    cellHeight = height/colCount;
    cellDepth = depth/depthCount;

    hCellWidth = cellWidth * 0.5f;
    hCellHeight = cellHeight * 0.5f;
    hCellDepth = cellDepth * 0.5f;
    float itx = pos.x() + width*0.5f;
    for (int i = 0; i < colCount; ++i)
    {
        itx = itx - cellWidth;
        float ity = pos.y() + height*0.5f;
        for (int j = 0; j < rowCount; ++j)
        {
            ity = ity - cellHeight;
            float itz = pos.z() + depth*0.5f;
            for (int k = 0; k < colCount; ++k)
            {
                itz = itz - cellDepth;
                //Cell* inCell = new Cell(Cell::AIR, -1);
                //addCell(itx + hCellWidth, ity + hCellHeight, itz + hCellDepth, inCell);
            }
        }
    }
}

vector3D MacGrid::findClosestCell(vector3D pos)
{
    float x = pos.x();
    float y = pos.y();
    float z = pos.z();

    int indexX = (pos.x() / cellWidth);
	int indexY = (pos.y() / cellHeight);
	int indexZ = (pos.z() / cellDepth);

    float xP = x > 0.f ? (indexX) * cellWidth + hCellWidth : (indexX) * cellWidth - hCellWidth;
    float yP = y > 0.f ? (indexY) * cellHeight + hCellHeight : (indexY) * cellHeight - hCellHeight;
    float zP = z > 0.f ? (indexZ) * cellDepth + hCellDepth : (indexZ) * cellDepth - hCellDepth;

    return vector3D(xP, yP, zP);
}

MacGrid::~MacGrid()
{
    std::tr1::unordered_map<float, Cell*>::iterator it = macCells.begin();
    for (it; it != macCells.end() ; it++)
    {
        delete(it->second);
        macCells.erase(it->first);
    }
}
std::vector<vector3D> MacGrid::getNeighbourPos(float x, float y, float z)
{

    vector3D p = findClosestCell(vector3D(x, y, z));
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

Cell *MacGrid::getCellIndex(int x, int y, int z)
{
    assert(cellInGrid(x, y, z) && "CELL NOT IN GRID");
    int hash = 541 * x + 79 * y + 31 * z;
        return macCells[hash];
}

bool MacGrid::cellInGrid(float x, float y, float z)
{
    std::tr1::unordered_map<float, Cell*>::iterator got = findCell(x, y, z);
    if (got == macCells.end())
    {
        return false;
    }
    else
    {
        return true;
    }
}

bool MacGrid::cellInBounds(float x, float y, float z)
{
    if ((x <= (width/2.f)-cellWidth && x >= (-width/2.f+cellWidth)) && (y <= (height/2.f)-cellWidth && y >= (-height/2.f)+cellWidth) && (z <= (depth/2.f)-cellWidth && z >= (-depth/2.f)+cellWidth))
    {
        return true;
    }
    else
    {
        return false;
    }
}


float MacGrid::getInterpolatedValue(float x, float y, float z, int index)
{
    //int i = std::floor(x / cellWidth);    //Why divide by cell dim??
    int i = std::floor(x);
    //int j = std::floor(y / cellHeight);
    int j = std::floor(y);
    //int k = std::floor(z / cellDepth);
    int k = std::floor(z);

    int size = macCells.size();
    float out = (i-1-x) * (j+1-y) * (k+1-z) * getCellIndex(i, j, k)->velocity[index];
    float enddiv = (i-1-x) * (j+1-y) * (k+1-z);
    test.clear();
    if ( cellInGrid(i+1, j, k))
    {
        test.push_back(getCellIndex(i+1, j, k)->position);
        out += (x-i) * (j+1-y) * (k+1-z) *  getCellIndex(i+1, j, k)->velocity[index];
        enddiv += (x-i) * (j+1-y) * (k+1-z);
    }
    if ( cellInGrid(i, j+1, k))
    {
        test.push_back(getCellIndex(i, j+1, k)->position);
        out += (i+1-x) * (y-j) * (k+1-z) *  getCellIndex(i, j+1, k)->velocity[index];
        enddiv += (i+1-x) * (y-j) * (k+1-z);
    }
    if ( cellInGrid(i+1, j+1, k))
    {
        test.push_back(getCellIndex(i+1, j+1, k)->position);
        out += (x-1) * (y-j) * (k+1-z) * getCellIndex(i+1, j+1, k)->velocity[index];
        enddiv += (x-1) * (y-j) * (k+1-z);
    }
    if ( cellInGrid(i, j, k+1))
    {
        test.push_back(getCellIndex(i, j, k+1)->position);
        out += (i+1-x) * (j+1-y) * (z-k) * getCellIndex(i, j, k+1)->velocity[index];
        enddiv += (i+1-x) * (j+1-y) * (z-k);
    }
    if ( cellInGrid(i+1, j, k+1))
    {
        test.push_back(getCellIndex(i+1, j, k+1)->position);
        out += (x-i) * (j+1-y) * (z-k) * getCellIndex(i+1, j, k+1)->velocity[index];
        enddiv += (x-i) * (j+1-y) * (z-k);
    }
    if ( cellInGrid(i, j+1, k+1))
    {
        test.push_back(getCellIndex(i, j+1, k+1)->position);
        out += (i+1-x) * (y-j) * (z-k) * getCellIndex(i, j+1, k+1)->velocity[index];
        enddiv += (i+1-x) * (y-j) * (z-k);
    }
    if ( cellInGrid(i+1, j+1, k+1))
    {
        test.push_back(getCellIndex(i+1, j+1, k+1)->position);
        out += (x-i) * (y-j) * (z-k) * getCellIndex(i+1, j+1, k+1)->velocity[index];
        enddiv += (x-i) * (y-j) * (z-k);
    }

    return out ; //== 0 ? out : out/enddiv;
}

float MacGrid::getLaplacianTimes(float x, float y, float z, int index)
{

    //int i = std::floor(x / cellWidth);    //Why divide by cell dim??
    int i = std::floor(x);
    //int j = std::floor(y / cellHeight);
    int j = std::floor(y);
    //int k = std::floor(z / cellDepth);
    int k = std::floor(z);

    return getCellIndex(i + 1, j, k)->velocity[index] + getCellIndex(i - 1 , j, k)->velocity[index] +
           getCellIndex(i, j + 1, k)->velocity[index] + getCellIndex(i, j -1 , k)->velocity[index] +
           getCellIndex(i, j, k + 1)->velocity[index] + getCellIndex(i, j, k - 1 )->velocity[index] - 6 * getCellIndex(i, j, k)->velocity[index];


}

bool MacGrid::cellInGrid(int x, int y, int z)
{
    std::tr1::unordered_map<float, Cell*>::iterator got = findCell(x, y, z);
    if (got == macCells.end())
    {
        return false;
    }
    else
    {
        return true;
    }
}

std::tr1::unordered_map<float, Cell *>::iterator MacGrid::findCell(int x, int y, int z)
{
    int indexX = std::floor(x);
    int indexY = std::floor(y);
    int indexZ = std::floor(z);
    int hash = 541 * indexX + 79 * indexY + 31 * indexZ;
    return macCells.find(hash);
}

vector3D MacGrid::findIndex(vector3D pos)
{
    int indexX = std::floor(pos.x() / cellWidth);
    int indexY = std::floor(pos.y() / cellHeight);
    int indexZ = std::floor(pos.z() / cellDepth);

    return vector3D(indexX, indexY, indexZ);
}

std::vector<vector3D> MacGrid::CellsToIndexVector()
{
    std::vector<vector3D> out;
    for (int z = -depthCount/2; z < depthCount/2; ++z)
    {
        for (int y = -colCount/2; y < colCount/2; ++y)
        {
            for (int x = -rowCount/2; x < rowCount/2; ++x)
            {
                out.push_back(vector3D(x,y,z));
                this->fluidVIndex[541 * x + 79 * y + 31 * z] = (int)out.size()-1;
            }
        }
    }
    return out;
}

void MacGrid::findNeighbours(vector3D pos, Cell* c)
{
    float x = pos.x();
    float y = pos.y();
    float z = pos.z();

    vector3D test = findIndex(pos);

    int i = test.x();
    int j = test.y();
    int k = test.z();

    if ( cellInGrid(i+1, j, k))
    {
        c->leftN = getCellIndex(i+1, j, k);
    }
    if ( cellInGrid(i-1, j, k))
    {
        c->rightN = getCellIndex(i-1, j, k);
    }
    if ( cellInGrid(i, j+1, k))
    {
        c->upN = getCellIndex(i, j+1, k);
    }
    if ( cellInGrid(i, j-1, k))
    {
        c->downN = getCellIndex(i, j-1, k);
    }
    if ( cellInGrid(i, j, k+1))
    {
        c->outN = getCellIndex(i, j, k+1);
    }
    if ( cellInGrid(i, j, k-1))
    {
        c->inN = getCellIndex(i, j, k-1);
    }
}

double MacGrid::getDivergence(vector3D pos)
{
    vector3D ind = findIndex(pos);
    return (getCellIndex(ind.x() + 1, ind.y(), ind.z())->velocity.x() - getCellIndex(ind.x(), ind.y(), ind.z())->velocity.x()+
            getCellIndex(ind.x(), ind.y() + 1, ind.z())->velocity.y() - getCellIndex(ind.x(), ind.y(), ind.z())->velocity.y()+
            getCellIndex(ind.x(), ind.y(), ind.z() + 1)->velocity.z() - getCellIndex(ind.x(), ind.y(), ind.z())->velocity.z());
}

double MacGrid::getDivergenceInd(vector3D ind)
{
    return (getCellIndex(ind.x() + 1, ind.y(), ind.z())->velocity.x() - getCellIndex(ind.x(), ind.y(), ind.z())->velocity.x()+
            getCellIndex(ind.x(), ind.y() + 1, ind.z())->velocity.y() - getCellIndex(ind.x(), ind.y(), ind.z())->velocity.y()+
            getCellIndex(ind.x(), ind.y(), ind.z() + 1)->velocity.z() - getCellIndex(ind.x(), ind.y(), ind.z())->velocity.z());
}

void MacGrid::setPressureFromIndex(VectorXd v )
{
    //float a = x + WIDTH * (y + DEPTH * z);
    for (int i = 0; i < v.size(); ++i)
    {

    }
}

int MacGrid::FlatIndex(vector3D p)
{
    return p.x() + rowCount * ( p.y() + depthCount * p.z());
}

int MacGrid::NumFluidAirNeighbours(vector3D ind)
{
    int n = 0;
    Cell* c = getCellIndex(ind.x(), ind.y(), ind.z());
    if (c->rightN->type != Cell::SOLID) { n++; }
    if (c->leftN->type != Cell::SOLID) { n++; }
    if (c->upN->type != Cell::SOLID) { n++; }
    if (c->downN->type != Cell::SOLID) { n++; }
    if (c->inN->type != Cell::SOLID) { n++; }
    if (c->outN->type != Cell::SOLID) { n++; }
    return n;
}

std::vector<vector3D> MacGrid::getNeighbourIndex(int x, int y, int z)
{
    int indexX = std::floor(x / cellWidth);
    int indexY = std::floor(y / cellHeight);
    int indexZ = std::floor(z / cellDepth);
    std::vector<vector3D> neighbours;
    neighbours.push_back(vector3D(indexX+1, indexY, indexZ));
    neighbours.push_back(vector3D(indexX-1, indexY, indexZ));
    neighbours.push_back(vector3D(indexX, indexY+1, indexZ));
    neighbours.push_back(vector3D(indexX, indexY-1, indexZ));
    neighbours.push_back(vector3D(indexX, indexY, indexZ+1));
    neighbours.push_back(vector3D(indexX, indexY, indexZ-1));
    return neighbours;
}



