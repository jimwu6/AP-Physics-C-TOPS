function density = rho(r)
    rho_centre = 1.622E5;
    r_sun = 6.38E8;
    density = rho_centre * (1 - r/r_sun);
end