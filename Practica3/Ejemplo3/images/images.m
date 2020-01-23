file = 'translatingtree_150x150_40frs.bin';
fid = fopen(file);
a = fread(fid,'single');
n = length(a);
xsize=150
ysize=150
nframes = n/xsize/ysize

for n=1:nframes,
	for i=1:ysize,
		iframe = (n-1)*xsize*ysize;
		mov(n,i,:) = a(iframe+(i-1)*xsize+1:iframe+i*xsize);
	end
	img(:,:) = mov(n,:,:);
	name =sprintf('imageTrans%i.png', n);
	imwrite(uint8(img),name);
end
img

