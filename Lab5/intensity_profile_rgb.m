I = imread('test6.png');%read file, change path directory as necessary
Ig = rgb2gray(I);
x = [1 size(Ig,2)];%don't know what these do
X_new = linspace(-0.0729565, 0.0729565, 1057);
%X_final = [X_new(:, 1:480) X_new(:,578:1057)];

%X_new = transpose(X_new);
y = [size(Ig,1)*0.5 size(Ig, 1)*0.5];
c = improfile(I,x,y);%takes intensity profile, presumably
%c_new = transpose(c);
%c_final = [c_new(:, 1:480) c_new(:,578:1057)];
%

% coeffs
r = 0.2989;
g = 0.5870;
b = 0.1140;
Gr = zeros(size(Ig, 2), 1);
for i = 1:size(Gr)
    Gr(i) = r*c(i,1,1) + g*c(i,1,2)/2.5 + b*c(i,1,3);
end

figure
subplot(2,1,1)
imshow(Ig)
hold on;
plot(x,y,'r')%plot r
subplot(2,1,2)
hold on
xyz = c(:,1,3);
plot(c(:,1,1),'r')%plot r again?
plot(c(:,1,2)/2.5,'g')%plot g
plot(c(:,1,3),'b')%plot b
plot(Gr, 'Color', [0.5 0.5 0.5]);
%}