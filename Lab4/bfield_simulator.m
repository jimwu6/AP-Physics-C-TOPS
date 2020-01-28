% B-Field Simulator
% AP Physics C - SPH4U0
% Jim Wu, Angela Pang, Eric Shang, Nelson Lee, Steven Zhang

% Constants
mu_0 = 4*pi*10^-7;

% Initial variables
current = 0.1; % in A
theta = pi/6; % angle of rotation in rad
L = 0.02; % length in m
R1 = 0.07; % radius of first coil in m
R2 = 0.05; % radius of second coil in m
loops = 300; % number of wraps
const = 10^-7*current; % mu_0/4pi * i
steps = 0.020; % for different points on the graph

% Rotation Matrices
rx = [1 0 0; 0 cos(theta) -sin(theta); 0 sin(theta) cos(theta)]; %x
ry = [cos(theta) 0 sin(theta); 0 1 0; -sin(theta) 0 cos(theta)]; %y
rz = [cos(theta) -sin(theta) 0; sin(theta) cos(theta) 0; 0 0 1]; %z

% Parameters for solenoids
t1 = [0:1:2000];
t2 = [0:1:2000];
% shift for solenoid 2
shiftx = 0.05;
shifty = 0.05;
shiftz = 0.05;

% Solenoid 1 - No rotation
k = loops*2*pi;

x1 = R1 * cos(t1);
y1 = L / k * t1;
z1 = R1 * sin(t1);

dx1 = -R1 * sin(t1);
dy1 = L / k * ones(size(t1));
dz1 = R1 * cos(t1);

% consolidate points and derivatives
p1 = [x1; y1; z1;];
dp1 = [dx1; dy1; dz1;];

% Solenoid 2 - Rotation by theta along z axis then shift
x2 = R2 * cos(t2);
y2 = L / k * t2;
z2 = R2 * sin(t2);

dx2 = -R2 * sin(t2);
dy2 = L / k * ones(size(t2));
dz2 = R2 * cos(t2);

% consolidate points and derivatives
p2 = [x2; y2; z2;];
dp2 = [dx2; dy2; dz2;];

% rotate points and derivatives
for i = 1:length(x2)
    p2(:, i) = rx * p2(:, i) + [shiftx; shifty; shiftz;]; % only shift points
    dp2(:, i) = rx * dp2(:, i); % don't shift derivatives
end

% Final vector fields
px = -0.10:steps:0.15;
py = -0.05:steps:0.15;
pz = -0.1:steps:0.15;
% Preset vector field sizes
B = ones(length(px)*length(py)*length(pz), 3);
P = ones(length(px)*length(py)*length(pz), 3);

% counter for indicies of B and P
cnt = 1;

% calculate B-field
for i = 1:length(px)
    for j = 1:length(py)
        %fprintf('%d %d \n', i, j); debugging
        for k = 1:length(pz)
            % set initial dB at a certain point to be 0
            dB = [0 0 0];
            for m = 1:length(p1)
                % solenoid 1
                % generate r
                r = -[px(i)-p1(1, m), py(j)-p1(2, m), pz(k)-p1(3, m)];
                % generate dB = ds x r / r^3
                dB = dB + cross([dp1(1, m), dp1(2, m), dp1(3, m)], r) / norm(r)^3;
                
                % solenoid 2
                % generate r
                r = [px(i)-p2(1, m), py(j)-p2(2, m), pz(k)-p2(3, m)];
                % generate dB = ds x r / r^3
                dB = dB + cross([dp2(1, m), dp2(2, m), dp2(3, m)], r) / norm(r)^3;
                
            end
            % place dB and point into vector fields
            B(cnt, :) = dB;
            P(cnt, :) = [px(i), py(j), pz(k)];
            % increase index
            cnt = cnt + 1;
        end
    end
end

% add the constant and increase field to get bigger quivers
B = B * const * 100;
% have final B field for surface plot
Bf = B / 100;

% Setup Figures
fig = figure('Position', [100, 100, 1200, 450]); 

% plot 1 for quiver plot
subplot(1, 2, 1)

plot3(p1(1,:), p1(2,:), p1(3, :), 'LineWidth', 1.5, 'Color', 'g');
hold on;
plot3(p2(1,:), p2(2,:), p2(3, :), 'LineWidth', 1.5, 'Color', 'b');

% display every 5th vector
sn = 5;
quiver3(P(1:sn:end,1), P(1:sn:end,2), P(1:sn:end,3), B(1:sn:end,1), B(1:sn:end,2), B(1:sn:end,3), 'Color','red','Autoscale','off');

% label axes
xlabel('<= X Position (m) =>', 'FontSize', 10)
ylabel('<= Y Position (m) =>', 'FontSize', 10)
zlabel('<= Z Position (m) =>', 'FontSize', 10)
set(gca,'fontname','times')  % Set it to times

% second plot for surface plot
subplot(1, 2, 2)

% create surface vectors, only taking some points to increase clarity
surfx = P(1:13,3);
surfy = P(1:13,3);
surfz = ones(length(surfx), length(surfy));

% pick threshold to check for surfaceplot
pla = 0.03;

n = 1;
m = 1;
for i = 1:length(P)
    % epsilon error checking for floating point numbers
    if abs(P(i, 2)-pla) < 0.009999999;
        surfz(n, m) = norm(Bf(i, :)) * 1000;
        if (n == 13) %creates new column
            m = m+1;
            n = 0;
        end
        n = n+1;
        if (m > 13) %ends loop when required values are stored
            i = length(P) + 1;
        end
    end
end

% plot
splot = surf(surfx, surfy, surfz);

% label axes
xlabel('<= X Position (m) =>', 'FontSize', 10)
ylabel('<= Z Position (m) =>', 'FontSize', 10)
zlabel('Magnetic Field (mT) =>', 'FontSize', 10)
set(gca,'fontname','times')  % Set it to times
hold off;