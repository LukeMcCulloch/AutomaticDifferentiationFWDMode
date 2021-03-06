

#include "../include/GetEigen.h"


typedef float Number;


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
   // unary operations
   AD operator-();

   //-------------------------
   // binary operations
   AD operator+(const AD& other);
   AD operator-(const AD& other);
   AD operator*(const AD& other);
   AD operator/(const AD& other);

   AD operator+(Number other);
   AD operator-(Number other);
   AD operator*(Number other);
   AD operator/(Number other);

   //-------------------------
   // printing
   void print_value();
   void print_grad();
   void print_hess();
   void print_size();
   void print();

   //-------------------------
   //AD One = 
};



//-------------------------
// unary operations
AD AD::operator-() {

   Number new_value = -value;
   int space_size = space_dim;
   AD result(new_value, space_size);
   result.grad = -grad;
   result.hess = -hess;
   return result;
}

//-------------------------
// binary operations

AD AD::operator+(const AD& other) {

   Number new_value = value + other.value;
   //std::cout << " adding " << value << " to " << other.value << std::endl;
   int space_size = space_dim;

   AD result(new_value, space_size);
   result.grad = grad + other.grad;
   result.hess = hess + other.hess;
   return result;

}

AD AD::operator-(const AD& other) {

   Number new_value = value - other.value;
   int space_size = space_dim;
   AD result(new_value, space_size);
   result.grad = grad - other.grad;
   result.hess = hess - other.hess;
   return result;
}

AD AD::operator*(const AD& other) {

   Number new_value = value * other.value;
   int space_size = space_dim;
   AD result(new_value, space_size);
   result.grad = grad*other.value + other.grad*value;
   result.hess =  other.value*hess + \
                  other.grad * grad.transpose() + \
                  grad * other.grad.transpose() + \
                  other.hess * value;

   return result;

}


AD AD::operator/(const AD& other) {

   Number new_value = value / other.value;
   int space_size = space_dim;

   AD result(new_value, space_size);

   // store components of the gradient:
   // scalar:
   Number bottom = other.value*other.value;
   /*
   // vector:
   Eigen::Matrix<Number, Dynamic, 1> top;
   top.setZero(space_size);
   top = other.value*grad - value*other.grad;


   // compute the gradient:
   result.grad = (top)/(bottom);


   // store components of the Hessian:
   Eigen::Matrix<Number, Dynamic, 1> dbottom;
   dbottom.setZero(space_size);
   dbottom = other.value*other.grad - other.value*other.grad;

   // store components of the Hessian:
   Eigen::Matrix<Number, Dynamic, Dynamic>  dtop;
   dtop.setZero(space_size, space_size);
   dtop =   (
               other.grad * grad.transpose() + \
               other.value * hess
            ) - \
            (
               grad * other.grad.transpose() + \
               value*other.hess
            );

   // compute the Hessian:
   result.hess = ( (bottom * dtop) - (top * dbottom.transpose()) )  / (bottom*bottom);
   */

   // more efficient (?) (drops the temporaries... 
   // which makes for extra computation but less memory action)
   /**/
   // compute the gradient (memory efficient):
   result.grad = (other.value*grad - value*other.grad)/(bottom);

   // compute the Hessian (without temporaries this time):
   result.hess =  (  bottom * \
                              (
                                 (
                                    other.grad * grad.transpose() + \
                                    other.value * hess
                                 ) - \
                                 (
                                    grad * other.grad.transpose() + \
                                    value*other.hess
                                 )
                              )
                              - 
                     (
                        (other.value*grad - value*other.grad) *\
                        (other.value*other.grad - other.value*other.grad).transpose()
                     )
                  ) / (bottom*bottom);
   /**/


   return result;

}


//----------------------------------------------------------------------
// left var is AD, right var is Number


AD AD::operator+(Number other) {

   Number new_value = value + other;
   //std::cout << " adding " << value << " to " << other.value << std::endl;
   int space_size = space_dim;

   AD result(new_value, space_size);
   result.grad = grad;
   result.hess = hess;
   return result;

}

AD AD::operator-(Number other) {

   Number new_value = value - other;
   int space_size = space_dim;
   AD result(new_value, space_size);
   result.grad = grad;
   result.hess = hess;
   return result;
}

AD AD::operator*(Number other) {

   Number new_value = value * other;
   int space_size = space_dim;

   AD result(new_value, space_size);

   result.grad = grad*other;

   result.hess =  other*hess;

   return result;

}

AD AD::operator/(Number other) {

   Number new_value = value / other;
   int space_size = space_dim;

   AD result(new_value, space_size);

   // save:
   Number bottom = other*other;

   // compute the gradient 
   result.grad = ( other * grad ) / bottom;

   // compute the Hessian 
   result.hess =  (  bottom * other * hess ) / (bottom*bottom);

   return result;

}



//-------------------------
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







//-------------------------
// r-operations
AD operator+( Number self , AD& other) {
   return other + self;
}
AD operator-( Number self , AD& other) {
   return -other + self;
}
AD operator*( Number self , AD& other) {
   return other * self;
}
AD operator/( Number self , AD& other) {
   int sizeofOthergrad = other.grad.size();
   AD One(1.0f, sizeofOthergrad, 0, "one");
   return self * One / other;//fix to handle float/AD
}





int main() {
   std::cout  << "Eigen version: " << EIGEN_MAJOR_VERSION  << "."<< EIGEN_MINOR_VERSION  << std::endl;
   AD a(2.0f, 2, 0, "a");
   AD b(3.0f, 2, 1, "b");
   a.print();
   b.print();

   // struct cell_data{
   //      float xc;  // Cell-center coordinate
   //      Array2D<float>   u = Array2D<float>(3,1);  // Conservative variables = [rho, rho*u, rho*E]
   //      Array2D<float>  u0 = Array2D<float>(3,1);  // Conservative variables at the previous time step
   //      Array2D<float>  ww = Array2D<float>(3,1);  // Primitive variables = [rho, u, p]
   //      Array2D<float>  dw = Array2D<float>(3,1);  // Slope (difference) of primitive variables
   //      Array2D<float> res = Array2D<float>(3,1);  // Residual = f_{j+1/2) - f_{j-1/2)
   //  };

   AD c = a + b;
   c.print();

   AD d = a - b;
   d.print();

   AD e = a * b;
   e.print();

   AD f = a/b;
   f.print();

   AD s1 = a+1.0f;
   AD s2 = a-1.0f;
   AD s3 = a*1.0f;
   AD s4 = a/1.0f;
   s1.print();
   s2.print();
   s3.print();
   s4.print();

   std::cout << "-------------------------" << std::endl;
   std::cout << "a = " << std::endl;
   a.print();

   AD r1 = 1.0f+a;
   AD r2 = 1.0f-a;
   r1.print();
   r2.print();

   AD r11 = 1.0+a;
   AD r21 = 1.0-a;
   r11.print();
   r21.print();


   AD r3 = a+1.0f;
   AD r4 = a-1.0f;
   r3.print();
   r4.print();


   std::cout << "-------------------------" << std::endl;
   std::cout << "a = " << std::endl;
   a.print();
   AD r5 = 1.0/a;
   AD r6 = 1.0*a;
   std::cout << "r5 = " << std::endl;
   r5.print();
   std::cout << "r6 = " << std::endl;
   r6.print();

   //just testing Eigen a bit
   // std::cout << r2.grad << std::endl;
   // r2.grad(0,0) = 1.;
   // r2.grad(1,0) = 3.1459;
   // std::cout << r2.grad << std::endl;


   //std::cout << r1.hess.inverse() << std::endl;

   return 0;
}

