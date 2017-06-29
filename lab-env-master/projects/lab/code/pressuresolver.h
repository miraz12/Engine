//
// Created by antchr-4 on 2/16/17.
//

#ifndef GSCEPT_LAB_ENV_PRESSURESOLVER_H
#define GSCEPT_LAB_ENV_PRESSURESOLVER_H


#include <map>
#include "mactable.h"

/********************************************************************************
    VectorXd
********************************************************************************/

class VectorXd
{
public:
    VectorXd();
    VectorXd(int size);
    VectorXd(int size, double fill);
    VectorXd(VectorXd &vector);
    ~VectorXd();

    const double operator [](int i) const;
    double& operator[](int i);

    inline size_t size() {
        return _vector.size();
    }

    void fill(double fill);
    double dot(VectorXd &vector);
    double absMaxCoeff();

    std::vector<double> _vector;

};

/********************************************************************************
    MatrixCoefficients
********************************************************************************/

struct MatrixCell {
    char diag;
    char plusi;
    char plusj;
    char plusk;

    MatrixCell() : diag(0x00), plusi(0x00), plusj(0x00), plusk(0x00) {}
};

class MatrixCoefficients
{
public:
    MatrixCoefficients();
    MatrixCoefficients(int size);
    ~MatrixCoefficients();

    const MatrixCell operator [](int i) const;
    MatrixCell& operator [](int i);

    inline size_t size() {
        return cells.size();
    }

    std::vector<MatrixCell> cells;
};

/********************************************************************************
    PressureSolver
********************************************************************************/

struct PressureSolverParameters
{
    double cellwidth;
    double density = 20.0;
    double deltaTime;
    MacTable* velocityField;
    std::vector<Cell*> fCells;
};


class PressureSolver
{
    double density = 20.0;
    int isize = 0;
    int jsize = 0;
    int ksize = 0;
    double pressureSolveTolerance = 1e-6;
    int maxCGIterations = 200;
    MacTable* grid;
    double deltaTime = 0.0;
    double cellWidth = 0.0;

    void CalculateNegativeDivergence(VectorXd& b);
    void CalculateMatrixCoefficients(MatrixCoefficients& A);
    void CalculatePreconditionerVector(MatrixCoefficients& A, VectorXd& precon);
    void SolvePressureSystem(MatrixCoefficients& A, VectorXd& b, VectorXd& precon, VectorXd& pressure);
    void IndexKeyMap();
    void ApplyPreconditioner(MatrixCoefficients &A, VectorXd &precon, VectorXd &residual, VectorXd &vect);
    void ApplyMatrix(MatrixCoefficients& A, VectorXd& x, VectorXd& result);
    void AddScaledVector(VectorXd &v1, VectorXd &v2, double scale);
    void AddScaledVectors(VectorXd &v1, double s1, VectorXd &v2, double s2, VectorXd &result);

public:
    void solve(PressureSolverParameters params, VectorXd& p);

    PressureSolver();
    ~PressureSolver(){};
    std::map<int, int> fluidVIndex;
    int numFluidCells = 0;
    std::vector<Cell*> fluidCells;

};


#endif //GSCEPT_LAB_ENV_PRESSURESOLVER_H
