
std::size_t ij2k(std::size_t i, std::size_t j, std::size_t Nx, std::size_t Ny)
{
    //return i*Ny + j;
    return j*Nx + i;
}

std::size_t k2i(std::size_t k, std::size_t Nx, std::size_t Ny)
{
    //return k / Ny;
    return k % Nx;
}
    
std::size_t k2j(std::size_t k, std::size_t Nx, std::size_t Ny)
{
    //return k % Ny;
    return k / Nx;
}
