A=imread('f2.png');
imshow(A);
I=rgb2gray(A);
%imshow(I);

%focal length
fx=309.4362; fy= 344.2161;
% optical center
ox= 317.9034; oy =256.5352;
% height of the camera in mm
h= 2179.8; 
% pitch of the camera,alfa
pitch= 14.0/180*pi;
c1=cos(pitch); s1=sin(pitch);
% yaw of the camera,beta
yaw= 0.0;
c2=cos(yaw); s2=sin(yaw);
% imag width and height
iw= 640; ih= 480;
%ipm setting
% ipmW=160; ipmH=120; ipmL=85; ipmR=550; ipmT=50; ipmB=380; 
% eps=0.2*480=96;

%compute the vanishing point
W=[s2/c1;c2/c1;0];
A=[c2,-s2,0;s2,c2,0;0,0,1];
B=[1,0,0;0,-s1,-c1;0,c1,-s1];
T=[fx,0,ox;0,fy,oy;0,0,1];

Vanish=T*B*A*W;
%%%
x=Vanish(1); y=Vanish(2);
uvLimits=...
[ x,  iw, 0, x; ...
  y,   y, y,ih];

T=...
[ 
-1/fx*c2, 1/fy*s1*s2, 1/fx*ox*c2-1/fy*oy*s1*s2-c1*s2, 0; ...
1/fx*s2,  1/fy*s1*c1, -1/fx*ox*s2-1/fy*s1*c2-c1*c2,   0; ...   
   0,        1/fy*c1,      -1/fy*oy*c1+s1,            0; ...
   0,    -1/(h*fy)*c1,   1/(h*fy)*oy*c1-1/h*s1,       0; ...
 ];

xyLimits=h*T*uvLimits;

maxX=max(xyLimits(1));
maxY=max(xyLimits(2));
x=0;y=maxY;
xyGrid=size(2,rows*cols);

for i=1:cols
   for j=1:rows
    xyGrid(1,(i-1)*cols+j)=x;
    xyGrid(2,(i-1)*cols+j)=y;
    x=x+maxX/rows;
   end
   y=y-maxY/cols;
end

