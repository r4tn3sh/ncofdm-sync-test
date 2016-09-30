clear all;
close all;

% Parameters
M = 4;                      % Size of signal constellation
k = log2(M);                % Number of bits per symbol
F = 64;                     % FFT size
C = F/4;                    % CP length

%fid = fopen('C:\Users\Ratnesh\cap_data.dat','rb');
fid = fopen('cap_data_async.dat','rb');
tl = 2000000;
a = fread(fid,[2,tl],'float');
fclose(fid);

offset =  1000000;
l=500000;
b = a(:,offset:offset+l);
rxdata = b(1,:) + b(2,:)*1i;

% l=20000;
% load('file.mat');
% data = z;

fid = fopen('pnseq.dat','r');
totpnseq = fread(fid,[1,l]);
fclose(fid);
totpnseq = 2*totpnseq - 1;

p_os = 1;
N = 160;
pnseq1 = totpnseq(p_os:p_os+N-1);
for loop=1:l-N-1
    ldata = rxdata(loop : loop+N-1);
    data_mean = mean(ldata);
    numr = abs(sum(ldata.*pnseq1)- N*data_mean*mean(pnseq1));
    denm = sqrt(sum(abs(ldata).^2) - N*data_mean*conj(data_mean)) * sqrt(N);
    cor1(loop) = numr/denm;
end
% stem(abs(cor1))
% ylim([0 1]);
% hold on;
index1 = find(abs(cor1)>0.5);

p_os = 200;
N = 160;
pnseq2 = totpnseq(p_os:p_os+N-1);
for loop=1:l-N-1
    data_mean = mean(rxdata(loop : loop+N-1));
    numr = sum(rxdata(loop : loop+N-1).*pnseq2) - N*data_mean*mean(pnseq2);
    denm = sqrt(sum(abs(rxdata(loop : loop+N-1)).^2) - N*data_mean*conj(data_mean)) * sqrt(N);
    cor2(loop) = numr/denm;
end
% stem(abs(cor2))
index2 = find(abs(cor2)>0.5);

if isempty([index1  index2])
    error('No correlation found for PN-sequence 1 or 2');
end
% if isempty(index1)
%     error('No correlation found for PN-sequence 1');
% end
% if isempty(index2)
%     error('No correlation found for PN-sequence 2');
% end

% Load baseline data
load('basedata.mat');
basedata = basedata';
basebits = basebits';
modbasedata = qammod(basedata, M);
plt_ind = 8; % every 8th symbol is pilot
    
    
if length(index1) > length(index2)
    rel_index = index1;
else
    rel_index = index2;
end

total_error = 0;
total_bits = 0;
for loop = 1:length(rel_index)-1
    start_index = rel_index(loop);
    pndata = rxdata(start_index: start_index+N-1);
    m_pndata = mean(pndata);
    temp_data = pndata - m_pndata;

    decbits_temp = zeros(k*F,1);

    phase_diff1 = angle(pnseq1) - angle(temp_data);
    ph1 = meanangle(phase_diff1);
    mul_sym1 = exp(1i*(-ph1));
    %mul_sym1 = cos(mean(phase_diff1)) + 1i*sin(mean(0-phase_diff1));
    for x=1:8
        rxblock1 = (rxdata(start_index+1 + N + (x-1)*(C+F):start_index+1 + N + (x-1)*(C+F) + F -1))'*mul_sym1;
        store_rxblock1(:,x) = (rxblock1);
        fftrxblock1 = fft(rxblock1);
        %%%%%% Phase correction after fft %%%%%
        yc = 0;
        angcorr = 0;
        for y=1:F
            if (A1(y)>0)
                if rem(yc,plt_ind) == 0
                    angcorr = angle(modbasedata(y))-angle(fftrxblock1(y));
                end
                fftrxblock1(y) = fftrxblock1(y)*exp(1i*angcorr);
                yc = yc+1;
            end
        end
        %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
        store_fftblock1(:,x) = fftrxblock1;
        decdata_temp = qamdemod(fftrxblock1,M).*A1';
        decdata1(:,x) = decdata_temp;
    end

    phase_diff2 = angle(pnseq2) - angle(temp_data);
    ph2 = meanangle(phase_diff2);
    mul_sym2 = exp(1i*(-ph2));
    %mul_sym2 = cos(mean(phase_diff2)) + 1i*sin(mean(0-phase_diff2));
    for x=1:8
        rxblock2 = (rxdata(start_index + N + (x-1)*(C+F):start_index + N + (x-1)*(C+F) + F -1))'*mul_sym2;
        store_rxblock2(:,x) = (rxblock2);
        fftrxblock2 = fft(rxblock2);
        %%%%%% Phase correction after fft %%%%%
        yc = 0;
        angcorr = 0;
        for y=1:F
            if (A2(y)>0)
                if rem(yc,plt_ind) == 0
                    angcorr = angle(modbasedata(y))-angle(fftrxblock2(y));
                end
                fftrxblock2(y) = fftrxblock2(y)*exp(1i*angcorr);
                yc = yc+1;
            end
        end
        %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
        store_fftblock2(:,x) = fftrxblock2;
        decdata_temp = qamdemod(fft(rxblock2),M).*A2';
        decdata2(:,x) = decdata_temp;
    end

    total_decdata = decdata1+decdata2;
    %figure; plot(phase_diff);
    %figure; plot(temp_data);
    %close all;


    for x=1:8
        decdata_temp = total_decdata(:,x);
        for y=1:F
            decbits_temp((y-1)*k+1:y*k) = de2bi(decdata_temp(y),k,'left-msb');
        end
        total_decbits(:,x) = decbits_temp;
    end

    for x=1:8
        total_data_diff(:,x) = total_decdata(:,x)-basedata;
        total_bits_diff(:,x) = total_decbits(:,x)-basebits;
    end
    
    total_error = total_error + sum(sum(abs(total_bits_diff)));
    total_bits = total_bits + numel(total_bits_diff);
end

BER = total_error/total_bits
% figure;
% for x=1:160
%     if pnseq1(x)>0
%         plot(temp_data(x),'bo');
%     else
%         plot(temp_data(x),'ro');
%     end
%     hold on;
% end
% figure;
% for x=1:160
%     if pnseq2(x)>0
%         plot(temp_data(x),'bo');
%     else
%         plot(temp_data(x),'ro');
%     end
%     hold on;
% end