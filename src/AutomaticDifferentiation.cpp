

#include <iostream>


#include <eigen/Eigen/Dense>
#include  <eigen/Eigen/Core>


typedef float Number;

using Eigen::Dynamic;


//https://eigen.tuxfamily.org/dox/TopicFunctionTakingEigenTypes.html
//By letting your function take templated parameters of these base types, 
//you can let them play nicely with Eigen's expression templates.
template <typename Derived>
void print_size(const Eigen::EigenBase<Derived>& b)
{
  std::cout << "size (rows, cols): " << b.size() << " (" << b.rows()
            << ", " << b.cols() << ")" << std::endl;
}

class AD {

   public:

   Number value;
   Eigen::Matrix<Number, Dynamic, 1> grad;
   Eigen::Matrix<Number, Dynamic, Dynamic> hess;

   // shape of the hessian (redundant really)
   Eigen::Matrix<int, 2, 1> shape;

   // number of design space dimensions
   int space_dim;

   // location in the gradient space - i.e. where this variable "lives"
   // in relation to the otheres in a design space.
   int index;

   // AD variables can have a name
   std::string name;


   // constructor for base variable initilization
   AD(Number val, int space_size, int grad_index, std::string name="ADvar"){
      value = val;            // AD value
      space_dim = space_size; // size of design space
      index = grad_index;     // which index in the gradient
      name=name;              // variable name 

      //Eigen intrinsic for initialization
      grad.setZero(space_size);
      hess.setZero(space_size, space_size);

      grad(index) = 1.0;
   }


};

int main() {

   return 0;
}