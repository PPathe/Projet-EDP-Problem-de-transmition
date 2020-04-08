
using parameters = std::map<std::string, double>;

/*****************************************************************************************
*
* class solution1D to define the numerical solution in 1D
*
*****************************************************************************************/
class solution1D
{    
public:
    std::vector<double> u; // the numerical solution
    double t;              // the time t 
    mesh1D m;              // the mesh on which the solution is computed
    parameters param;      // the parameters
    
    solution1D(parameters);  // the constructor
    void init(double (*ptr_f)(double)); // initialization of the solution
    
    double max(); // return the maximum of the solution
    double min(); // return the minimum of the solution
};

solution1D::solution1D(parameters p)
{
    param = p;
    // mesh choice (node-centered or cell-centered)
    bool toto = false;
    auto it_nc = param.find("node-centered");
    if (it_nc != param.end())
    {
        if (it_nc->second == 1.)
            m = mesh1D_nc(param);
        else
            m = mesh1D_cc(param);
        toto = true;
    }
    auto it_cc = param.find("cell-centered");
    if (it_cc != param.end())
    {
        if (it_cc->second == 1.)
            m = mesh1D_cc(param);
        else
            m = mesh1D_nc(param);
        toto = true;
    }
    if (!toto)
        throw std::invalid_argument("Missing argument: cell-centered or node-centered mesh!");
    u.resize(m.Nx);
}

void solution1D::init(double (*ptr_f)(double))
{
    // BEGIN SOLUTION
    t = 0.;
    std::transform(m.x.begin(), m.x.end(), u.begin(), ptr_f);
    //for (auto i=0; i<m.Nx; ++i)
    //    u[i] = ptr_f(m.x[i]);
    // END SOLUTION
}

double solution1D::min()
{
    // BEGIN SOLUTION
    return *std::min_element(u.begin(), u.end());
    // END SOLUTION
}

double solution1D::max()
{
    // BEGIN SOLUTION
    return *std::max_element(u.begin(), u.end());
    // END SOLUTION
}

std::ostream& operator<<(std::ostream& out, solution1D& S)
{
    out << std::string(80, '-')    << std::endl;
    out << "solution informations" << std::endl;
    out << std::string(80, '-')    << std::endl;
    // BEGIN SOLUTION
    out << "    dimension:         " << 1      << std::endl;
    out << "    xmin:              " << S.m.xmin << std::endl;
    out << "    xmax:              " << S.m.xmax << std::endl;
    out << "    number of points:  " << S.m.Nx   << std::endl;
    out << "    spatial step:      " << S.m.dx   << std::endl;
    out << "[" << std::endl;
    if (S.m.Nx <= 6)
        for (auto k = 0; k < S.m.Nx; ++k)
            out << "  " << S.m.x[k] << " \t " << S.u[k] << std::endl;
    else
    {
        for (auto k = 0; k < 3; ++k)
            out << "  " << S.m.x[k] << " \t " << S.u[k] << std::endl;
        out << "   ... " << std::endl;
        for (auto k = S.m.Nx - 3; k < S.m.Nx; ++k)
            out << "  " << S.m.x[k] << " \t " << S.u[k] << std::endl;

    }
    out << "]" << std::endl;    
    // END SOLUTION
    out << std::string(80, '-') << std::endl;
    return out;
}
