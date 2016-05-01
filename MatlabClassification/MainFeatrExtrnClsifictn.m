clear all;
close all;
clc;

Label_PATH = '../../Auto mixing Dataset/Mixed Audio/';
LABELfolderInfo = dir(Label_PATH);

DATA_PATH = '../../Auto mixing Dataset/Audio for Dataset/';
DATAfolderInfo = dir(DATA_PATH);

iBlockLength = 2048;
iHopLength = iBlockLength;
afWindow = rectwin(iBlockLength);

load('NumFolds.mat'); % gives variable NUM_FOLDS which comes after running the GenerateGTlabels

features = [];
labels = [];
for i = 4:size(LABELfolderInfo)
    subfolderInfoLabel = dir ([Label_PATH LABELfolderInfo(i).name '/' '*.txt']);
    for j = 1:size(subfolderInfoLabel,1)
        
        % read the labels from Label_PATH
        [pathstr,name,ext]=fileparts([Label_PATH LABELfolderInfo(i).name '/' subfolderInfoLabel(j).name]);
        intermlabel1=dlmread([pathstr '/' name '.txt'],'\t');
        
        % open the curresponding audio file for feature extraction
        folderName = strsplit(pathstr,'/');
        [audio,fs] = audioread ([DATA_PATH folderName{end} '/' name '.wav']);
        
        for k = 1:size(audio,2)
            [vrms, t] = FeatureTimeRms(audio, iBlockLength, iHopLength, fs);
            [X,f,t]     = spectrogram(  audio,...
                                    afWindow,...
                                    iBlockLength-iHopLength,...
                                    iBlockLength,...
                                    fs);
        
            % magnitude spectrum
            X           = abs(X)*2/iBlockLength;
        
            [vsc] = FeatureSpectralCentroid (X, fs);
            reqdvppm(:,k)=vrms(1:length(vsc));
            reqdvsc(:,k)=vsc;
            intermlabel(:,k) = intermlabel1(1:length(vsc),k);
        end
        
        features=[features;[reqdvsc(:),reqdvppm()]];
        labels = [labels;intermlabel(:)];
        clear reqdvppm reqdvsc intermlabel;
    end
end
fclose all;

[Rsq, S, p, r, predictions] = CrossValidation(labels, features, NUM_FOLDS);
 