clear all;
close all;
% Random number seed
rng(100)

% Parameters
M = 4;                      % Size of signal constellation
k = log2(M);                % Number of bits per symbol
ene = 0.01;                 % To keep the absolute value below 1 for USRP transmission
F = 64;                     % FFT size
C = F/4;                    % CP length
P = 500;                    % number of OFDM symbols
datalen = F*P;              % number of samples

% Set two disjoint allocation vector
A = ones(1,F);                    % Allocation vector with all subcarriers being used
A1 = [A(1:F/2) zeros(1,F/2)];
A2 = A - A1;

% Generate data
%basebits = randi(2,k,F)-1;
basedata = randi(M,1,F)-1;
basebits = zeros(1,k*F);
for x=1:F
    basebits((x-1)*k+1:x*k) = de2bi(basedata(x),k,'left-msb');
end
data = zeros(1,F*P);
for x=1:P
    data((x-1)*F+1 : x*F) = basedata;
end
mdata = qammod(data, M);
norm_mdata = mdata/sqrt(mean(abs(mdata).^2));

block_data = reshape(norm_mdata,[F P]);

pre_data_1 = zeros(F,P);
pre_data_2 = zeros(F,P);

for x=1:F
    if A1(x)>0
        pre_data_1(x,:) = block_data(x,:);
    end
    if A2(x)>0
        pre_data_2(x,:) = block_data(x,:);
    end
end


fft_data_1 = sqrt(ene)*ifft(pre_data_1,F)*sqrt(F);
fft_cp_data_1 = [fft_data_1; fft_data_1(1:C,:)];
tx_data_1 = reshape(fft_cp_data_1,[1 (F+C)*P]);    % Data 1 ready

fft_data_2 = sqrt(ene)*ifft(pre_data_2,F)*sqrt(F);
fft_cp_data_2 = [fft_data_2; fft_data_2(1:C,:)];
tx_data_2 = reshape(fft_cp_data_2,[1 (F+C)*P]);    % Data 2 ready 


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% Sanity check %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% zx = 4;
% zx_data = fft_cp_data_1(1+zx:F+zx,:);
% ch_data_1 = fft(circshift(zx_data,0),F);
ch_data_1 = fft(circshift(fft_data_1,0));
for x=1:F
    if A1(x)==0
        ch_data_1(x,:) = 0*ch_data_1(x,:);
    end
end
chsr_data_1 = reshape(ch_data_1,[1 F*P]);

ch_data_2 = fft(fft_data_2);
for x=1:F
    if A2(x)==0
        ch_data_2(x,:) = 0*ch_data_2(x,:);
    end
end
chsr_data_2 = reshape(ch_data_2,[1 F*P]);
comb_data = chsr_data_1+chsr_data_2;

dec_data = qamdemod(comb_data, M);
%plot(data-dec_data);

%%%%%%%%%%%%%%%%%%%%%%%%% Underlay operation %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
seqlen = 10000;
pnseq = randi(2,1,seqlen)-1;
fid = fopen('pnseq.dat','wb');
fwrite(fid,pnseq);
fclose(fid);

ratiodb = 10;
ratio = 10^(-ratiodb/10);
Lpn = 160;
OS1 = 1;
OS2 = 200;
Rep = 800;
% Add underlay
temp_pnseq = [pnseq(OS1:OS1+Lpn-1) zeros(1,Rep-Lpn)];
temp_pnseq = 2*temp_pnseq-1;
for x = 1:(F+C)*P
    y = rem((x-1),Rep)+1;
    pnseq1(x) = temp_pnseq(y);
end
tx_data_ul_1 = tx_data_1 + sqrt(ene*ratio)*pnseq1;

temp_pnseq = [pnseq(OS2:OS2+Lpn-1) zeros(1,Rep-Lpn)];
temp_pnseq = 2*temp_pnseq-1;
for x = 1:(F+C)*P
    y = rem((x-1),Rep)+1;
    pnseq2(x) = temp_pnseq(y);
end
tx_data_ul_2 = tx_data_2 + sqrt(ene*ratio)*pnseq2;

% savecomplexvar(tx_data_ul_1,'txdataul1.dat');
% savecomplexvar(tx_data_ul_2,'txdataul2.dat');

% Add interleaved pn sequence
for x = 1:(F+C)*P
    y = rem((x-1),Rep)+1;
    if y<=Lpn
        tx_data_int_1(x) = sqrt(ene*ratio)*pnseq1(x);
        tx_data_int_2(x) = sqrt(ene*ratio)*pnseq2(x);
    else
        tx_data_int_1(x) = tx_data_1(x);
        tx_data_int_2(x) = tx_data_2(x);
    end
end

savecomplexvar(tx_data_int_1,'int_data1.dat');
savecomplexvar(tx_data_int_2,'int_data2.dat');

z=tx_data_int_1; % **************
save('file.mat','z');
save('basedata.mat','basedata','basebits','A1','A2');