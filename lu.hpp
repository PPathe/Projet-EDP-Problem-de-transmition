
/*****************************************************************************************
*
* lu decomposition
*
*****************************************************************************************/

matrix lu(const matrix& A)
{
    auto n = A.shape().first, m = A.shape().second;
    if (n != m)
        throw std::length_error("Error in lu decomposition: dimension mismatch !!");
    matrix LU = matrix(n,n);
    for (auto j=0; j<n; ++j)
    {
        for (auto i=0; i<=j; ++i)
        {
            auto sum = 0.;
            for (auto k=0; k<i; ++k)
                sum += LU(i,k) * LU(k,j);
            LU(i,j) = A(i,j) - sum;
        }
        if (LU(j,j) == 0)
            throw std::invalid_argument("Error in lu decomposition: the decomposition failed !!");
        for (auto i=j+1; i<n; ++i)
        {
            auto sum = 0.;
            for (auto k=0; k<j; ++k)
                sum += LU(i,k) * LU(k,j);
            LU(i,j) = (A(i,j) - sum) / LU(j,j);
        }
    }
    return LU;
}

bool lu_verification(const matrix& A, const matrix& LU)
{
    auto shapeA = A.shape(), shapeLU = LU.shape();
    auto n = shapeA.first;
    if ((n != shapeA.second) || (n != shapeLU.first) || (n != shapeLU.second))
        throw std::length_error("Error in lu_verification: dimension mismatch !!");
    auto L = identity(n,n);
    for (auto i=0; i<n; ++i)
        for (auto j=0; j<i; ++j)
            L(i,j) = LU(i,j);
    auto U = matrix(n,n);
    for (auto i=0; i<n; ++i)
        for (auto j=i; j<n; ++j)
            U(i,j) = LU(i,j);
    auto test = A - L*U;
    return test.is_zero();
}

std::vector<double> solve_lu(const matrix& LU, const std::vector<double>& b)
{
    auto n = LU.shape().first;
    if ((n != LU.shape().second) || (n != b.size()))
        throw std::length_error("Error in solve_lu: dimension mismatch !!");
    std::vector<double> x(n);
    // solve Ly=b
    for (auto i=0; i<n; ++i)
    {
        auto sum = b[i];
        for (auto j=0; j<i; ++j)
            sum -= LU(i,j) * x[j];
        x[i] = sum;
    }
    // solve Ux=y
    for (int i=n-1; i>=0; --i)
    {
        auto sum = x[i];
        for (auto j=i+1; j<n; ++j)
            sum -= LU(i,j) * x[j];
        x[i] = sum / LU(i,i);
    }
    return x;
}
