
using parameters = std::map<std::string, double>;

/*****************************************************************************************
*
* class mesh1D to define the mesh of the simulation in 1D
*
*****************************************************************************************/
class mesh1D
{
public:
    double xmin, xmax, dx; // left, right bounds + spatial step
    std::size_t Nx;        // number of points
    std::vector<double> x; // the points of the mesh
    std::string name;      // the name of the class
    
    mesh1D() = default;                    // empty constructor
    mesh1D(const mesh1D&) = default;         // copy constructor
    mesh1D& operator=(mesh1D&& m) = default; // move constructor
    mesh1D(parameters);
};

mesh1D::mesh1D(parameters param)
{
    name = "generic";
    xmin = param["xmin"];
    xmax = param["xmax"];
    Nx = param["Nx"];
    if (Nx <= 0)
    {
        std::cout << "Error in mesh generation: the parameter 'Nx' has to be positive" << std::endl;
        Nx = 1;
    }
    dx = 0.;
    x.resize(Nx);
}

// print
std::ostream& operator<<(std::ostream& out, mesh1D& m)
{
    out << std::string(80, '-') << std::endl;
    out << "mesh informations (" << m.name << ")" << std::endl;
    out << std::string(80, '-') << std::endl;
    // BEGIN SOLUTION
    out << "    dimension:         " << 1      << std::endl;
    out << "    xmin:              " << m.xmin << std::endl;
    out << "    xmax:              " << m.xmax << std::endl;
    out << "    number of points:  " << m.Nx   << std::endl;
    out << "    spatial step:      " << m.dx   << std::endl;
    out << "[" << std::endl;
    if (m.Nx <= 6)
        for (auto &xk : m.x)
            out << "  " << xk << std::endl;
    else
    {
        for (auto it = m.x.begin(); it != std::next(m.x.begin(),3); ++it)
            out << "  " << *it << std::endl;
        out << "   ... " << std::endl;
        for (auto it = std::prev(m.x.end(), 3); it != m.x.end(); ++it)
            out << "  " << *it << std::endl;
    }
    out << "]" << std::endl;
    // END SOLUTION
    out << std::string(80, '-') << std::endl;
    return out;
}

class mesh1D_cc: public mesh1D
{
public:
    mesh1D_cc() = default;
    mesh1D_cc(parameters);
};

// constructor
mesh1D_cc::mesh1D_cc(parameters param): mesh1D(param)
{
    name = "cell-centered";
    // BEGIN SOLUTION
    dx = (xmax - xmin) / Nx;
    x[0] = xmin + 0.5*dx;
    for (auto k = 1; k < x.size(); ++k)
        x[k] = x[k-1] + dx;
    // END SOLUTION
}

class mesh1D_nc: public mesh1D
{
public:
    mesh1D_nc() = default;
    mesh1D_nc(parameters);
};

// constructor
mesh1D_nc::mesh1D_nc(parameters param): mesh1D(param)
{
    name = "node-centered";
    // BEGIN SOLUTION
    dx = (xmax - xmin) / (Nx - 1);
    x[0] = xmin;
    for (auto k = 1; k < x.size(); ++k)
        x[k] = x[k-1] + dx;
    // END SOLUTION
}


