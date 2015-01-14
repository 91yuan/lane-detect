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
% input imag width and height
ih= 480; iw= 640;
% output imag w and h
oh=120; ow=160;
% x=317.9034;
% y=170.7125;
% image=[317.9034; 170.7125;1;1];

W=[s2/c1;c2/c1;0];
A=[c2,-s2,0;s2,c2,0;0,0,1];
B=[1,0,0;0,-s1,-c1;0,c1,-s1];
T=[fx,0,ox;0,fy,oy;0,0,1];

Vanish=T*B*A*W;
x=Vanish(1,1)+0.001; y=Vanish(2,1)+0.001;

image=[x; y;1;1];
T=...
[ 
-c2/fx, s1*s2/fy,   ox*c2/fx-oy*s1*s2/fy-c1*s2, 0; ...
s2/fx,  s1*c2/fy,  -ox*s2/fx-s1*c2*oy/fy-c1*c2, 0; ...   %s1*c2
   0,      c1/fy,      -oy*c1/fy+s1,            0; ...
   0,   -c1/(h*fy),    oy*c1/(h*fy)-s1/h,       0; ...
 ];

world=h*T*image;

for i=1:4
    world(i,:)=world(i,:)./world(4,:);
end
world

M=[
 fx*c2+c1*s2*ox,    -fx*s2+c1*c2*ox, -s1*ox; ...
 s2*(oy*c1-fy*s1), c2*(oy*c1-fy*s1), -fy*c1-oy*s1; ...
      c1*s2,               c1*c2,        -s1;
];
%world(3,:)=h;
X=M*world(1:3,1);

for i=1:3
    X(i,:)=X(i,:)./X(3,:);
end

X