

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


   // constructor for operations
   AD(Number val, int space_size, std::string name="ADvar"){
      value = val;            // AD value
      space_dim = space_size; // size of design space
      //index = index;     // which index in the gradient
      name=name;

      grad.setZero(space_size);
      hess.setZero(space_size, space_size);

   }

   //-------------------------
   // operations
   AD operator+(AD other);
   AD operator-(AD other);

   //-------------------------
   // printing
   void print_value();
   void print_grad();
   void print_hess();
   void print_size();
   void print();


};



//-------------------------
// implementation
// operations

AD AD::operator+(AD other) {

   Number new_value = value + other.value;
   std::cout << " adding " << value << " to " << other.value << std::endl;
   int space_size = space_dim;

   AD result(new_value, space_size);
   result.grad = grad + other.grad;
   result.hess = hess + other.hess;
   return result;

}

AD AD::operator-(AD other) {

   Number new_value = value - other.value;
   int space_size = space_dim;
   AD result(new_value, space_size);
   result.grad = grad - other.grad;
   result.hess = hess - other.hess;
   return result;
}



//-------------------------
// implementation
// printing
void AD::print()
{
   std::cout << "AD(" << name << std::endl;
   print_size();
   print_value();
   print_grad();
   print_hess();
   std::cout << "    )\n\n" << std::endl;
}

void AD::print_value()
{
  std::cout << " value: " << value << "" << std::endl;
}
void AD::print_grad()
{
  std::cout << " grad: \n" << grad << "" << std::endl;
}
void AD::print_hess()
{
  std::cout << " hess: \n" << hess << "" << std::endl;
}

void AD::print_size()
{
  std::cout << " design space size: (" << space_dim << ")" << std::endl;
}

int main() {
   std::cout  << "Eigen version: " << EIGEN_MAJOR_VERSION  << "."<< EIGEN_MINOR_VERSION  << std::endl;
   AD a(2.0, 2, 0, "a");
   AD b(3.0, 2, 1, "b");
   a.print();
   b.print();

   AD c = a + b;
   c.print();

   AD d = c - a;
   d.print();

   return 0;
}