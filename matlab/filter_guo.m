%  i=9:0.5:25;
%  y=gaussmf(i,[1 17]);
%  y=y/sum(y);
[oh,ow,OutputImage]=ipm;
lengthH=9;
lengthV=33;
halfH=4;
halfV=16;

 HFilter=[-1.000000e-03, -2.200000e-02, -1.480000e-01, -1.940000e-01, 7.300000e-01, -1.940000e-01, -1.480000e-01, -2.200000e-02, -1.000000e-03];
 VFilter=[1.000000e-16, 1.280000e-14, 7.696000e-13, 2.886400e-11, 7.562360e-10, 1.468714e-08, 2.189405e-07, 2.558828e-06, 2.374101e-05, 1.759328e-04, 1.042202e-03, 4.915650e-03, 1.829620e-02, 5.297748e-02, 1.169560e-01, ...
 1.918578e-01, 2.275044e-01, 1.918578e-01, 1.169560e-01, 5.297748e-02, 1.829620e-02, 4.915650e-03, 1.042202e-03, 1.759328e-04, 2.374101e-05, 2.558828e-06, 2.189405e-07, 1.468714e-08, 7.562360e-10, 2.886400e-11, 7.696000e-13, 1.280000e-14, 1.000000e-16];

% j=0:0.1:2;
% sigma=1;
% S=sigma*sigma;
% x=1/S*j.*exp(-1/(2*S)*j.*j).*(1-j.*j/sigma);
OutputImage=im2double(OutputImage);
Image=ones(oh+2*halfV,ow+2*halfH);
Image(1+halfV:oh+halfV,1+halfH:ow+halfH)=OutputImage;
for i=1:halfV
    Image(i,:)=Image(1+halfV,:);
end
for i=oh+halfV+1:oh+halfV*2
    Image(i,:)=Image(oh+halfV,:);
end
for j=1:halfH
    Image(:,j)=Image(:,1+halfH);
end
for j=ow+halfH+1:ow+halfH*2
    Image(:,j)=Image(:,ow+halfH);
end
% figure;
% imshow(Image);
FilterImage=Image;
temp=Image;

% vertical filter
for u=halfV+1:oh+halfV
%     for v=1:ow
    temp(u,:)=VFilter*Image(u-halfV:u+halfV,:);
%     end
end
% figure;
% imshow(temp);

% horizonal filter
for v=halfH+1:ow+halfH
%     for u=1:oh
    FilterImage(:,v)=temp(:,v-halfH:v+halfH)*HFilter';
%     end
end
FilterImage=FilterImage(1+halfV:oh+halfV,1+halfH:ow+halfH);
% figure;
% imshow(FliterImage);

FilterImage(FilterImage<0)=0;
temp=FilterImage(:);
threshold=quantile(temp,0.975);
FilterImage=im2bw(FilterImage,threshold);
imshow(FilterImage);