#ifndef _KRETENJA_H_
#define _KRETENJA_H_

#include <iostream>
#include <vector>
#include <cmath>
#include <utility>
#include <cstdlib>

#include "eigen/Eigen/Eigen"

using namespace Eigen;

MatrixXd Euler2A(double alfa, double beta, double gamma);
std::pair<Vector3d, double> AxisAngle(Matrix3d A);
Matrix3d Rodrigez(Vector3d& p, double angle);
void A2Euler(Matrix3d& RodMat);
Vector4d Kvaternioni(Vector3d p, double angle);
std::pair<Vector3d, double> Q2AxisAngle(Vector4d q);


#endif