function temperature = T(r)
    k = 1.38E-23;
    mu = 0.61;
    m_H = 1.67E-27;
    temperature = 0.1 * P(r) * mu * m_H / k / rho(r);
end