
/*****************************************************************************************
*
* class matrix
*
*****************************************************************************************/
class matrix
{
private:
    std::size_t nx, ny;
    std::vector<double> array;
public:
    matrix() = default;                         // constructeur sans paramètre
    matrix(const matrix&) = default;            // constructeur par copie
    matrix& operator=(const matrix&) = default; // assignation par copie
    matrix(matrix&&) = default;                 // constructeur par déplacement
    matrix& operator=(matrix&&) = default;      // assignation par déplacement
    matrix(const std::size_t nx, const std::size_t ny): nx{nx}, ny{ny}
    {
        array.resize(nx*ny);
    }

    // shape of the matrix
    auto shape() const
    {
        return std::make_pair(nx, ny);
    }
    
    // fill the matrix with a constant value
    void fill(const double value)
    {
        std::fill(array.begin(), array.end(), value);
    }
    
    // modify the value: the matrix becomes identity
    void identity()
    {
        std::fill(array.begin(), array.end(), 0);
        for (std::size_t i = 0; i < nx; ++i)
            array[i + ny*i] = 1;
    }
    
    // return the value ith row and jth column: read data
    double operator()(const std::size_t i, const std::size_t j) const
    {
        return array[j + ny*i];
    }
    
    // return the reference ith row and jth column: write data
    double& operator()(const std::size_t i, const std::size_t j)
    {
        return array[j + ny*i];
    }
    
    // operators overload 
    matrix& operator+=(const matrix);
    matrix& operator-=(const matrix);
    matrix& operator*=(const matrix);
    matrix& operator^=(const std::size_t);
    matrix& operator+=(const double);
    matrix& operator-=(const double);
    matrix& operator*=(const double);
    matrix& operator/=(const double);
    
    bool is_zero(double pres = 1.e-14)
    {
        for (auto a: array)
            if (std::fabs(a) > pres)
                return false;
        return true;
    }
};

std::ostream& operator<<(std::ostream &out, const matrix &mat)
{
    out.setf(std::ios_base::scientific);
    out.precision(3);
    auto shape = mat.shape();
    for(std::size_t i=0; i<shape.first; ++i)
    {
        out << "[ ";
        for(std::size_t j=0; j<shape.second; ++j)
            std::cout << std::setw(10) << mat(i, j) << " ";
        out << "]\n";
    }
    out.unsetf(std::ios_base::scientific);
    return out;
}

matrix& matrix::operator+=(const matrix B)
{
    if (shape()!=B.shape())
        throw std::length_error("Dimension mismatch !!");
    for(std::size_t i=0; i<array.size(); ++i)
        array[i] += B.array[i];
    return *this;
}

matrix& matrix::operator-=(const matrix B)
{
    if (shape()!=B.shape())
        throw std::length_error("Dimension mismatch !!");
    for(std::size_t i=0; i<array.size(); ++i)
        array[i] -= B.array[i];
    return *this;
}

matrix& matrix::operator*=(const matrix B)
{
    auto shapeA = shape();
    auto shapeB = B.shape();
    if (shapeA.second != shapeB.first)
        throw std::length_error("Dimension mismatch !!");
    matrix Acopy=*this;
    ny = shapeB.second;
    array.resize(nx*ny);
    for(std::size_t i=0; i<nx; ++i)
        for(std::size_t j=0; j<ny; ++j)
        {
            double som = 0;
            for(std::size_t k=0; k<shapeA.second; ++k)
                som += Acopy(i, k)*B(k, j);
            (*this)(i, j) = som;
        }
    return *this;
}

matrix& matrix::operator^=(const std::size_t p)
{
    if (nx!=ny)
        throw std::length_error("Matrix has to be square !!");
    matrix s = *this; // copy of the matrix
    // modify the matrix into identity
    identity();
    std::size_t m = p;
    while (m>0)
    {
        if (m%2 != 0)
        *this *= s;
        s *= s;
        m /= 2;
    }
    return *this;
}

matrix& matrix::operator+=(const double a)
{
    for(std::size_t i=0; i<array.size(); ++i)
        array[i] += a;
    return *this;
}

matrix& matrix::operator-=(const double a)
{
    for(std::size_t i=0; i<array.size(); ++i)
        array[i] -= a;
    return *this;
}

matrix& matrix::operator*=(const double a)
{
    for(std::size_t i=0; i<array.size(); ++i)
        array[i] *= a;
    return *this;
}

matrix& matrix::operator/=(const double a)
{
    if (a == 0.)
        throw std::invalid_argument("Division by 0.!!");
    for(std::size_t i=0; i<array.size(); ++i)
        array[i] /= a;
    return *this;
}

matrix operator+(const matrix& A, const matrix& B)
{
    matrix C = A;
    return C += B;
}

matrix operator-(const matrix& A, const matrix& B)
{
    matrix C = A;
    return C -= B;
}

matrix operator^(const matrix& A, const std::size_t p)
{
    matrix C = A;
    return C^=p;
}

matrix operator*(const matrix& A, const matrix& B)
{
    matrix C = A;
    return C *= B;
}

matrix identity(const std::size_t nx, const std::size_t ny, const int k = 0)
{
    // BEGIN SOLUTION
    auto A = matrix(nx, ny);
    A.fill(0);
    std::size_t i = std::max(0, -k), j = std::max(0, k);
    while ((i < nx) && (j < ny))
        A(i++,j++) = 1;
    return A;
    // END SOLUTION
}

std::vector<double> operator*(const matrix& A, const std::vector<double>& v)
{
    auto shapeA = A.shape();
    if (shapeA.second != v.size())
        throw std::length_error("Dimension mismatch !!");
    std::vector<double> vout(shapeA.first);
    for(std::size_t i=0; i<shapeA.first; ++i)
    {
        double som = 0;
        for(std::size_t j=0; j<v.size(); ++j)
            som += A(i, j) * v[j];
        vout[i] = som;
    }
    return vout;
}

std::vector<double> operator*(const std::vector<double>& v, const matrix& A)
{
    auto shapeA = A.shape();
    if (shapeA.first != v.size())
        throw std::length_error("Dimension mismatch !!");
    std::vector<double> vout(shapeA.second);
    for(std::size_t j=0; j<shapeA.second; ++j)
    {
        double som = 0;
        for(std::size_t i=0; i<v.size(); ++i)
            som += v[i] * A(i, j);
        vout[j] = som;
    }
    return vout;
}

double operator*(const std::vector<double>& v, const std::vector<double>& w)
{
    if (v.size() != w.size())
        throw std::length_error("Dimension mismatch !!");
    return std::inner_product(v.begin(), v.end(), w.begin(), 0.);
}

matrix operator+(const double a, const matrix& A)
{
    matrix B = A;
    return B += a;
}

matrix operator+(const matrix& A, const double a)
{
    matrix B = A;
    return B += a;
}

matrix operator-(const double a, const matrix& A)
{
    matrix B = A;
    return B -= a;
}

matrix operator-(const matrix& A, const double a)
{
    matrix B = A;
    return B -= a;
}

matrix operator*(const double a, const matrix& A)
{
    matrix B = A;
    return B *= a;
}

matrix operator*(const matrix& A, const double a)
{
    matrix B = A;
    return B *= a;
}

matrix operator/(const matrix& A, const double a)
{
    matrix B = A;
    return B /= a;
}
