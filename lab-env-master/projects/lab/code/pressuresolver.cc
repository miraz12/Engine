//
// Created by antchr-4 on 2/16/17.
//

#include <limits>
#include "pressuresolver.h"


/********************************************************************************
    VectorXd
********************************************************************************/

VectorXd::VectorXd() {
}

VectorXd::VectorXd(int size) : _vector(size, 0.0) {
}

VectorXd::VectorXd(int size, double fill) : _vector(size, fill) {
}

VectorXd::VectorXd(VectorXd &vector) {
    _vector.reserve(vector.size());
    for (unsigned int i = 0; i < vector.size(); i++) {
        _vector.push_back(vector[i]);
    }
}

VectorXd::~VectorXd() {
}

const double VectorXd::operator[](int i) const {
    return _vector[i];
}

double& VectorXd::operator[](int i) {
    return _vector[i];
}

void VectorXd::fill(double fill) {
    for (unsigned int i = 0; i < _vector.size(); i++) {
        _vector[i] = fill;
    }
}

double VectorXd::dot(VectorXd &vector) {
    double sum = 0.0;
    for (unsigned int i = 0; i < _vector.size(); i++) {
        sum += _vector[i] * vector._vector[i];
    }

    return sum;
}

double VectorXd::absMaxCoeff() {
    double max = -std::numeric_limits<double>::infinity();
    double min = std::numeric_limits<double>::infinity();
    for (unsigned int i = 0; i < _vector.size(); i++) {
        if (fabs(_vector[i]) > max) {
            max = fabs(_vector[i]);
        }
    }

    return max;
}

/********************************************************************************
    MatrixCoefficients
********************************************************************************/

MatrixCoefficients::MatrixCoefficients() {
}

MatrixCoefficients::MatrixCoefficients(int size) : cells(size, MatrixCell()) {
}

MatrixCoefficients::~MatrixCoefficients() {
}

const MatrixCell MatrixCoefficients::operator[](int i) const
{
    return cells[i];
}

MatrixCell& MatrixCoefficients::operator[](int i)
{
    return cells[i];
}

/********************************************************************************
    PressureSolver
********************************************************************************/

PressureSolver::PressureSolver()
{}

void PressureSolver::solve(PressureSolverParameters params, VectorXd& pressure)     // Ap = B
{
    this->deltaTime = params.deltaTime;
    this->cellWidth = params.cellwidth;
    this->grid = params.velocityField;
    this->fluidCells = params.fCells;
    this->density = params.density;

    pressure.fill(0.0);

    IndexKeyMap();      //Give all fluid cells a index by "left, up, out"

    VectorXd b(numFluidCells);
    CalculateNegativeDivergence(b);     //Setup negative divergence vector b

    MatrixCoefficients A(numFluidCells);
    CalculateMatrixCoefficients(A);

    VectorXd precon(numFluidCells);
    CalculatePreconditionerVector(A, precon);

    SolvePressureSystem(A, b, precon, pressure);

    return;
}

void PressureSolver::IndexKeyMap()
{
    numFluidCells = fluidCells.size();
}

void PressureSolver::CalculateNegativeDivergence(VectorXd &b)
{
    double scale = 1.0f / (float)cellWidth;

    for (int i = 0; i < numFluidCells; ++i)
    {
        Cell * itc = this->fluidCells[i];
        b[itc->fluidIndex] = -scale * (double)itc->rightN->velocity[0] -  itc->velocity[0] +
                                              itc->upN->velocity[1] -     itc->velocity[1] +
                                              itc->outN->velocity[2] -    itc->velocity[2];
    }

    double usolid = 0.0;
    double vsolid = 0.0;
    double wsolid = 0.0;
    for (int j = 0; j < numFluidCells; ++j)
    {
        Cell* itc = this->fluidCells[j];
        int fluidInd = itc->fluidIndex;

        if (itc->leftN->type == Cell::SOLID)
        {
            b[fluidInd] -= scale*(itc->velocity[0] - usolid);
        }
        if (itc->rightN->type == Cell::SOLID)
        {
            b[fluidInd] += scale*(itc->rightN->velocity[0] - usolid);
        }
        if (itc->downN->type == Cell::SOLID)
        {
            b[fluidInd] -= scale*(itc->velocity[1] - vsolid);
        }
        if (itc->upN->type == Cell::SOLID)
        {
            b[fluidInd] += scale*(itc->upN->velocity[1] - vsolid);
        }
        if (itc->inN->type == Cell::SOLID)
        {
            b[fluidInd] -= scale*(itc->velocity[2] - wsolid);
        }
        if (itc->outN->type == Cell::SOLID)
        {
            b[fluidInd] += scale*(itc->outN->velocity[2] - wsolid);
        }
    }
}

void PressureSolver::CalculateMatrixCoefficients(MatrixCoefficients &A)
{
    for (int j = 0; j < numFluidCells; ++j)
    {
        Cell *itc = this->fluidCells[j];
        int fluidInd = itc->fluidIndex;


        int n = grid->NumFluidAirNeighbours(itc);
        A.cells[fluidInd].diag = (char)n;

        if (itc->rightN->type == Cell::FLUID)
        {
            A.cells[fluidInd].plusi = 0x01;
        }
        if (itc->upN->type == Cell::FLUID)
        {
            A.cells[fluidInd].plusj = 0x01;
        }
        if (itc->outN->type == Cell::FLUID)
        {
            A.cells[fluidInd].plusk = 0x01;
        }
    }
}

void PressureSolver::CalculatePreconditionerVector(MatrixCoefficients &A, VectorXd &precon)
{
    double scale = deltaTime / (density * cellWidth * cellWidth);
    double negscale = -scale;

    double tau = 0.97; //Tuning constant?
    double sigma = 0.25;    //Safety constant?

    for (int i = 0; i < numFluidCells; ++i)
    {
        Cell *itc = this->fluidCells[i];
        int fluidInd = itc->fluidIndex;

        int vidx_im1 = itc->leftN->fluidIndex;
        int vidx_jm1 = itc->downN->fluidIndex;
        int vidx_km1 = itc->inN->fluidIndex;

        double diag = (double)A[fluidInd].diag*scale;

        double plusi_im1 = vidx_im1 != -1 ? (double)A[vidx_im1].plusi * negscale : 0.0;
        double plusi_jm1 = vidx_jm1 != -1 ? (double)A[vidx_jm1].plusi * negscale : 0.0;
        double plusi_km1 = vidx_km1 != -1 ? (double)A[vidx_km1].plusi * negscale : 0.0;

        double plusj_im1 = vidx_im1 != -1 ? (double)A[vidx_im1].plusj * negscale : 0.0;
        double plusj_jm1 = vidx_jm1 != -1 ? (double)A[vidx_jm1].plusj * negscale : 0.0;
        double plusj_km1 = vidx_km1 != -1 ? (double)A[vidx_km1].plusj * negscale : 0.0;

        double plusk_im1 = vidx_im1 != -1 ? (double)A[vidx_im1].plusk * negscale : 0.0;
        double plusk_jm1 = vidx_jm1 != -1 ? (double)A[vidx_jm1].plusk * negscale : 0.0;
        double plusk_km1 = vidx_km1 != -1 ? (double)A[vidx_km1].plusk * negscale : 0.0;

        double precon_im1 = vidx_im1 != -1 ? precon[vidx_im1] : 0.0;
        double precon_jm1 = vidx_jm1 != -1 ? precon[vidx_jm1] : 0.0;
        double precon_km1 = vidx_km1 != -1 ? precon[vidx_km1] : 0.0;

        double v1 = plusi_im1 * precon_im1;
        double v2 = plusj_jm1 * precon_jm1;
        double v3 = plusk_km1 * precon_km1;
        double v4 = precon_im1 * precon_im1;
        double v5 = precon_jm1 * precon_jm1;
        double v6 = precon_km1 * precon_km1;

        double e = diag - v1*v1 - v2*v2 - v3*v3 -
                   tau*(plusi_im1*(plusj_im1 + plusk_im1)*v4 +
                        plusj_jm1*(plusi_jm1 + plusk_jm1)*v5 +
                        plusk_km1*(plusi_km1 + plusj_km1)*v6);

        if (e < sigma*diag) {
            e = diag;
        }

        if (fabs(e) > 10e-9) {
            precon[fluidInd] = 1.0 / sqrt(e);
        }
    }
}

void PressureSolver::SolvePressureSystem(MatrixCoefficients &A, VectorXd &b, VectorXd &precon, VectorXd &pressure)
{
    double tol = pressureSolveTolerance;
    if (b.absMaxCoeff() < tol) {
        return;
    }

    VectorXd residual(b);
    VectorXd auxillary(numFluidCells);
    ApplyPreconditioner(A, precon, residual, auxillary);

    VectorXd search(auxillary);
    double alpha = 0.0;
    double beta = 0.0;
    double sigma = auxillary.dot(residual);
    double sigmaNew = 0.0;
    int iterationNumber = 0;

    while (iterationNumber < maxCGIterations) {
        ApplyMatrix(A, search, auxillary);
        double step= (auxillary.dot(search) - 10e-9);
        alpha = sigma / step;
        AddScaledVector(pressure, search, alpha);
        AddScaledVector(residual, auxillary, -alpha);

        //std::cout << "sigma: "<< sigma << std::endl;
        //std::cout << "alpha: "<< alpha << std::endl;
        //std::cout << "beta: "<< beta << std::endl;


        if (std::isinf(residual.absMaxCoeff()))
        {
            return;
        }


        if (residual.absMaxCoeff() < tol) {
            std::cout << "\tIteration #: " << iterationNumber << "\tEstimated Error: " << ((residual.absMaxCoeff())) << std::endl;
            return;
        }

        ApplyPreconditioner(A, precon, residual, auxillary);
        sigmaNew = auxillary.dot(residual);
        beta = sigmaNew / (sigma + 10e-9);
        AddScaledVectors(auxillary, 1.0, search, beta, search);
        sigma = sigmaNew;

        iterationNumber++;


        if (iterationNumber % 10 == 0)
        {
           //std::cout << "\tIteration #: " << iterationNumber << "\tEstimated Error: " << ((residual.absMaxCoeff())) << std::endl;
        }
    }
    std::cout << "\tIteration #: " << iterationNumber << "\tEstimated Error: " << ((residual.absMaxCoeff())) << std::endl;
}

void PressureSolver::ApplyPreconditioner(MatrixCoefficients &A, VectorXd &precon, VectorXd &residual, VectorXd &vect)
{
    double scale = deltaTime / (density*cellWidth*cellWidth);
    double negscale = -scale;

    // Solve A*q = residual
    VectorXd q(numFluidCells);
    for (int i = 0; i < numFluidCells; ++i)
    {
        Cell *itc = this->fluidCells[i];
        int fluidInd = itc->fluidIndex;

        int vidx_im1 = itc->leftN->fluidIndex;
        int vidx_jm1 = itc->downN->fluidIndex;
        int vidx_km1 = itc->inN->fluidIndex;

        double plusi_im1 = 0.0;
        double precon_im1 = 0.0;
        double q_im1 = 0.0;
        if (vidx_im1 != -1) {
            plusi_im1  = (double)A[vidx_im1].plusi * negscale;
            precon_im1 = precon[vidx_im1];
            q_im1      = q[vidx_im1];
        }

        double plusj_jm1 = 0.0;
        double precon_jm1 = 0.0;
        double q_jm1 = 0.0;
        if (vidx_jm1 != -1) {
            plusj_jm1  = (double)A[vidx_jm1].plusj * negscale;
            precon_jm1 = precon[vidx_jm1];
            q_jm1      = q[vidx_jm1];
        }

        double plusk_km1 = 0.0;
        double precon_km1 = 0.0;
        double q_km1 = 0.0;
        if (vidx_km1 != -1) {
            plusk_km1  = (double)A[vidx_km1].plusk * negscale;
            precon_km1 = precon[vidx_km1];
            q_km1      = q[vidx_km1];
        }

        double t = residual[fluidInd] - plusi_im1 * precon_im1 * q_im1 -
                   plusj_jm1 * precon_jm1 * q_jm1 -
                   plusk_km1 * precon_km1 * q_km1;

        t = t*precon[fluidInd];
        q[fluidInd] = t;

    }

    for (int i = 0; i < numFluidCells; ++i)
    {
        Cell *itc = this->fluidCells[i];
        int fluidInd = itc->fluidIndex;

        int vidx_ip1 = itc->rightN->fluidIndex;
        int vidx_jp1 = itc->upN->fluidIndex;
        int vidx_kp1 = itc->outN->fluidIndex;

        double vect_ip1 = vidx_ip1 != -1 ? vect[vidx_ip1] : 0.0;
        double vect_jp1 = vidx_jp1 != -1 ? vect[vidx_jp1] : 0.0;
        double vect_kp1 = vidx_kp1 != -1 ? vect[vidx_kp1] : 0.0;

        double plusi = (double)A[fluidInd].plusi * negscale;
        double plusj = (double)A[fluidInd].plusj * negscale;
        double plusk = (double)A[fluidInd].plusk * negscale;

        double preconval = precon[fluidInd];
        double t = q[fluidInd] - plusi * preconval * vect_ip1 -
                                 plusj * preconval * vect_jp1 -
                                 plusk * preconval * vect_kp1;

        t = t*preconval;
        vect[fluidInd] = t;
    }
}

void PressureSolver::ApplyMatrix(MatrixCoefficients &A, VectorXd &x, VectorXd &result)
{
    double scale = deltaTime/(density * cellWidth * cellWidth);
    double negscale = -scale;

    for (int i = 0; i < numFluidCells; ++i)
    {
        Cell *itc = this->fluidCells[i];

        // val = dot product of column vector x and idxth row of matrix A
        double val = 0.0;
        int vidx = itc->leftN->fluidIndex;
        if (vidx != -1) { val += x[vidx]; }

        vidx = itc->rightN->fluidIndex;
        if (vidx != -1) { val += x[vidx]; }

        vidx = itc->downN->fluidIndex;
        if (vidx != -1) { val += x[vidx]; }

        vidx = itc->upN->fluidIndex;
        if (vidx != -1) { val += x[vidx]; }

        vidx = itc->inN->fluidIndex;
        if (vidx != -1) { val += x[vidx]; }

        vidx = itc->outN->fluidIndex;
        if (vidx != -1) { val += x[vidx]; }

        val *= negscale;

        vidx = itc->fluidIndex;
        val += (double)A.cells[vidx].diag * scale * x._vector[vidx];

        result._vector[vidx] = val;

    }
}

void PressureSolver::AddScaledVector(VectorXd &v1, VectorXd &v2, double scale)
{
    for (unsigned int idx = 0; idx < v1.size(); idx++) {
        v1._vector[idx] += v2._vector[idx]*scale;
    }
}

void PressureSolver::AddScaledVectors(VectorXd &v1, double s1, VectorXd &v2, double s2, VectorXd &result)
{
    for (unsigned int idx = 0; idx < v1.size(); idx++) {
        result[idx] = v1[idx]*s1 + v2[idx]*s2;
    }
}
