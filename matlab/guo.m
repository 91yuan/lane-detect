A=imread('f2.png');
% imshow(A);
I=rgb2gray(A);
%imshow(I);

%focal length
fx=309.4362; fy= 344.2161;
% optical center
ox= 317.9034; oy =256.5352;
% height of the camera in mm
h= 2179.8*0.001; 
% pitch of the camera,alfa
pitch= 14.0/180*pi;
c1=cos(pitch); s1=sin(pitch);
% yaw of the camera,beta
yaw= 0.0;
c2=cos(yaw); s2=sin(yaw);
% input imag width and height
iw= 640; ih= 480;
% output imag w and h
ow=160; oh=120;
%ipm setting
% ipmW=160; ipmH=120; ipmL=85; ipmR=550; ipmT=50; ipmB=380; 
% eps=0.2*480=96;

%compute the vanishing point
W=[s2/c1;c2/c1;0];
A=[c2,-s2,0;s2,c2,0;0,0,1];
B=[1,0,0;0,-s1,-c1;0,c1,-s1];
T=[fx,0,ox;0,fy,oy;0,0,1];

Vanish=T*B*A*W;
%the size of the ipm on the input image
x=Vanish(1); y=Vanish(2);
uvLimits=...
[ x,  iw, 0, x; ...
  y,   y, y, ih; ...
  1,   1, 1,  1; ...
  0,   0, 0,  0; ...
  ];

T=...
[ 
-c2/fx, s1*s2/fy,   ox*c2/fx-oy*s1*s2/fy-c1*s2, 0; ...
s2/fx,  s1*c1/fy,  -ox*s2/fx-s1*c2*oy/fy-c1*c2, 0; ...   %s1*c2
   0,      c1/fy,      -oy*c1/fy+s1,            0; ...
   0,    -c1/(h*fy),   oy*c1/(h*fy)-s1/h,       0; ...
 ];
%the size of the input image on the ground
xyLimits=h*T*uvLimits;
for i=1:3
    xyLimits(i,:)=xyLimits(i,:)./xyLimits(4,:);
end
%compute the point on the ground corresponding to the ipm  
 maxX=max(xyLimits(1,:));
 maxY=max(xyLimits(2,:));
 minX=min(xyLimits(1,:));
 minY=min(xyLimits(2,:));
 stepRow=(maxY-minY)/oh;
 stepCol=(maxX-minX)/ow;

xyGrid=ones(3,oh*ow);
y=maxY;
for i=1:oh
   x=minX;
   for j=1:ow
    xyGrid(1,(i-1)*oh+j)=x;
    xyGrid(2,(i-1)*oh+j)=y;
    x=x+stepCol;
   end
   y=y-stepRow;
end
%get the pixel value on the corresponding ground
xyGrid(3,:)=xyGrid(3,:)*(-h);
M=[
 fx*c2+c1*s2*ox,    -fx*s2+c1*c2*ox, -s1*ox; ...
 s2*(oy*c1-fy*s1), c2*(oy*c1-fy*s1), -fy*c1-oy*s1; ...
      c1*s2,               c1*c2,        -s1;
];

uvGrid=M*xyGrid;
for i=1:2
    uvGrid(i,:)=uvGrid(i,:)./uvGrid(3,:);
end