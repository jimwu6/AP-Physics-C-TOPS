function flux = L(r)
    epsilon = 2;
    flux = int(4*pi*r*r*rho(r)*epsilon, r);
end