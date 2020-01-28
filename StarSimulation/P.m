function pressure = P(r)
    G = 6.67E-11;
    rho_centre = 1.622E5;
    r_sun = 6.38E8;
    pressure = int(-G*m(r)*rho(r)/(r^2), r) + 5*pi*G*rho_centre^2*r_sun^2/36;
    %pressure = int(-G*m(r)*rho(r)/(r^2), r);
end