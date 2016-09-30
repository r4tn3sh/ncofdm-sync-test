function savecomplexvar(var,filename)


rl = real(var);
im = imag(var);
s = length(var);

X = ['Saving variable of size ',num2str(s),' in file ',filename];
disp(X);

exdata = zeros(1,s);
for x=1:s
    exdata(2*x-1) = rl(x);
    exdata(2*x) = im(x);
end

fid = fopen(filename,'wb');
Z = fwrite(fid,exdata,'float');
fclose(fid);