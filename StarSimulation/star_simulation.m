
syms r;
r_sun = 6.38E8;
fplot(T(r), [0 r_sun], 'k');
grid on;

title("Temperature Gradient using Radius")
set(gca, 'YScale', 'log')
ax = gca;
ax.XTick = 0:0.1*r_sun:r_sun;
ax.XTickLabel = {'0','0.1','0.2','0.3','0.4','0.5','0.6','0.7','0.8','0.9','1'};

xlabel('r (\times6.38E8 m)') 
ylabel('T(r) (K)')
