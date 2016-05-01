clear all;
close all
clc;

DATA_PATH = '../../Auto mixing Dataset/Mixed Audio/';
folderInfo = dir(DATA_PATH);

iBlockLength = 2048;
iHopLength = iBlockLength;

NUM_FOLDS = 0;

for i = 4:size(folderInfo)
    subfolderInfo = dir ([DATA_PATH folderInfo(i).name '/' '*.wav']);
    for j = 1:size(subfolderInfo,1)
        [audio,fs] = audioread([DATA_PATH folderInfo(i).name '/' subfolderInfo(j).name]);
        [pathstr,name,ext]=fileparts([DATA_PATH folderInfo(i).name '/' subfolderInfo(j).name]);
        for k = 1:size(audio,2)
            [vppm, t] = FeatureTimePeakEnvelope(audio(:,k), iBlockLength, iHopLength, fs);
            reqdvppm(k,:)=vppm(2,:);
            NUM_FOLDS = NUM_FOLDS+1;
        end
        % write the vppm in a txt file
        dlmwrite([pathstr '/' name '.txt'],reqdvppm','delimiter','\t');
        clear reqdvppm;
    end
end
fclose all;
save('NumFolds.mat','NUM_F0LDS');