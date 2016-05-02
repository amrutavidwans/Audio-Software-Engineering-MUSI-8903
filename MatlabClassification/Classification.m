close all;
fclose all;
clear all;
clc;

load('FinalFeaturesNZ.mat');

NUM_FOLDS=10000;
% [Rsq, S, p, r, predictions] = CrossValidation(labelsNZ, featuresNZ, NUM_FOLDS);

features=featuresNZ;
labels=labelsNZ;
n_fold=NUM_FOLDS;
clear featuresNZ labelsNZ;

% Preallocate memory.
num_data = size(labels, 1);
predictions = zeros(num_data, 1);
sorted_labels = zeros(num_data, 1);

% Proportional distributions of classes among folds.
folds = cvpartition(labels, 'KFold', n_fold);

% Evaluate one fold at a time.
% data_start_idx = 1;
for (fold = 1:1)
  % Grab the test data.
  test_indices = folds.test(fold);
  test_labels = labels(test_indices, :);
  test_features = features(test_indices, :);
  data_idx = find(test_indices == 1);
  % Get training data.
  train_indices = folds.training(fold);
  train_labels = labels(train_indices, :);
  train_features = features(train_indices, :);
  
  % Zero-cross whitening.
%   [train_features, test_features] = whiten(train_features, test_features);
  [train_features, test_features] = NormalizeFeatures(train_features, test_features);
  
  % Train the classifier and get predictions for the current fold.
  svm = svmtrain(train_labels, train_features, '-s 3 -q');
  cur_predictions = svmpredict(test_labels, test_features, svm, '-q');
  
  % Store current predictions and their corresponding labels.
%   num_test_data = size(test_labels, 1);
%   data_stop_idx = data_start_idx + num_test_data - 1;
  predictions(data_idx) = cur_predictions;
  sorted_labels(data_idx) = test_labels;
  
%   data_start_idx = data_start_idx + num_test_data;
end
% [Rsq, S, p, r] = myRegEvaluation(test_labels, cur_predictions);%(sorted_labels, predictions);

save([pwd '/MyModel.mat'],'svm');

y=test_labels;
f=cur_predictions;

[r, p] = corr(y, f);
S = rms( (f-y));

y_mean = mean(y);

SS_tot = sum( (y - y_mean).^2);
SS_res = sum( (y - f).^2);

Rsq = 1 - SS_res / SS_tot;