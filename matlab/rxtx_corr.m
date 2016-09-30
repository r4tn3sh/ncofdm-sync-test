clear all;
%fid = fopen('C:\Users\Ratnesh\cap_data.dat','rb');
fid = fopen('~/cap_data.dat','rb');
tl = 2000000;
a = fread(fid,[2,tl],'float');
fclose(fid);

offset = 200000;
l=5000;
b = a(:,offset:offset+l);
data = b(1,:) + b(2,:)*1i;

% l=20000;
% load('file.mat');
% data = z;

fid = fopen('pnseq.dat','r');
totpnseq = fread(fid,[1,l]);
fclose(fid);
totpnseq = 2*totpnseq - 1;

p_os = 1;
N = 160;
pnseq = totpnseq(p_os:p_os+N-1);
for loop=1:l-N-1
    %x1 = sum(data(1:16).*pnseq(1:16))
    ldata = data(loop : loop+N-1);
    data_mean = mean(ldata);
    %numr = abs(sum(data(loop : loop+N-1).*pnseq)) - abs(N*data_mean*mean(pnseq));
    numr = abs(sum(ldata.*pnseq)- N*data_mean*mean(pnseq));
    denm = sqrt(sum(abs(ldata).^2) - N*data_mean*conj(data_mean)) * sqrt(N);
    cor1(loop) = numr/denm;
end
stem(abs(cor1))
ylim([0 1]);
find(abs(cor1)>0.3)

hold on;
p_os = 200;
N = 160;
pnseq = totpnseq(p_os:p_os+N-1);
for loop=1:l-N-1
    %x1 = sum(data(1:16).*pnseq(1:16))
    data_mean = mean(data(loop : loop+N-1));
    numr = sum(data(loop : loop+N-1).*pnseq) - N*data_mean*mean(pnseq);
    denm = sqrt(sum(abs(data(loop : loop+N-1)).^2) - N*data_mean*conj(data_mean)) * sqrt(N);
    cor2(loop) = numr/denm;
end
stem(abs(cor2))
find(abs(cor2)>0.3)