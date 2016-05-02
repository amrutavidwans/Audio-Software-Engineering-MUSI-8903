clear all;
close all;
clc;

load('ClassificationDATA.mat'); % opens the mat file with svm struct of model, variable name of struct is 'svm'
fid=fopen('Mymodel','w');

fprintf(fid,'%s %s\n','svm_type','epsilon_svr');
fprintf(fid,'%s %s\n','kernel_type','rbf');    % rbf
fprintf(fid,'%s %s\n','nr_class','2'); 
% fprintf(fid,'%s %s\n','degree', '3');

% total_sv 7
% rho -0.0259673
% SV
% -0.8018564420672978 0:0.1 1:0.2 
% -0.02543064825426999 0:0.2 1:0.3 
% 1 0:0.8 1:0.9 
% 0.2674193697345183 0:0.6 1:0.7 
% -0.9862237335345639 0:0.55 1:0.65 
% -0.07743909865596341 0:0.45 1:0.55 
% 0.6235305527775767 0:0.75 1:2.1220255e-314 


fprintf(fid,'%s %s\n','total_sv',num2str(svm.totalSV));
fprintf(fid,'%s %s\n','rho',num2str(svm.rho));

fprintf(fid,'%s\n','SV');

FullSVs=full(svm.SVs);

for i=1:length(svm.SVs)

    fprintf(fid, '%s', num2str(svm.sv_coef(i)));
    fprintf(fid, '%s %s\n', [' 0:' num2str(FullSVs(i,1))],[' 1:' num2str(FullSVs(i,2))]);
    
    
end
fclose(fid);