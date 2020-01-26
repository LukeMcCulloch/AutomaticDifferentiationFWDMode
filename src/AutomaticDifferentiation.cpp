

#include <iostream>


#include <eigen/Eigen/Dense>
#include  <eigen/Eigen/Core>


typedef float Number;

using Eigen::Dynamic;
//using Eigen::MatrixXd; // equivalent to:  typedef Matrix<double, Dynamic, Dynamic> MatrixXd;
//using Eigen::VectorXd;
//typedef Eigen::Matrix<Number, Dynamic, 1> Vector;


class AD {

   public:

   Number value;
   Eigen::Matrix<Number, Dynamic, 1> grad;
   Eigen::Matrix<Number, Dynamic, Dynamic> hess;

};

int main() {

   return 0;
}