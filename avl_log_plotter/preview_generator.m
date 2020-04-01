%==========================================================================
% Autonomous Vehicle Library
%
% Description: Parses and generates a preview map for every log folder in
%              the specified folder.
%==========================================================================

close all; clear all; clc;
addpath('functions');
addpath('mex');
addpath('plot_me');
disp('Running data plotter...');

%% Setttings

% Path to the folder containing the log folders
input_folder_path = '/home/eagle/Desktop/Deckbox Logs';

% Name of the image file to generate
output_file_name = 'path_preview.png';

%% Generate path preview images

% Get a list of folders in the given folder. Remove the . and .. entries 
% that matlab includes, and remove items that are not folders
log_folder_names = dir(input_folder_path);
log_folder_names(ismember( {log_folder_names.name}, {'.', '..'})) = [];
log_folder_names([log_folder_names.isdir] == 0) = [];

% Loop through all folders and generate the path preview image
for i = 1:length(log_folder_names)
    
    % Generate the file paths for the input and output
    folder_path = log_folder_names(i).folder;
    folder_name = log_folder_names(i).name;
    log_folder_path = fullfile(folder_path, folder_name, 'log');
    status_log_filepath = fullfile(log_folder_path, 'status_node.log');
    status_dat_filepath = fullfile(log_folder_path, 'status_node.radio.dat');
    output_filepath = fullfile(folder_path, folder_name, output_file_name);
    
    % Parse the log into a dat file
    parse_log(status_log_filepath);
    
    % Read the dat file into a matrix and extract the latitude and
    % longitude
    log_data = readmatrix(status_dat_filepath);
    lat = log_data(:,12);
    lon = log_data(:,13);
    
    % Remove the generated dat files
    delete(fullfile(log_folder_path, '*.dat'));
    
    % Plot the path preview
    figure('Color', [0.1 0.1 0.1])
    set(gcf, 'name', folder_name, 'numbertitle', 'off')
    set(gca, 'LooseInset', get(gca,'TightInset'))
    hold on
    axis equal
    
    plot(lon, lat, 'b.');

    ylabel('Range Aided EKF Estimate');
    xlabel('Longitude (deg)');
    ylabel('Latitude (deg)');

    set(gca,'Color', [0.1 0.1 0.1]);
    
    % Save the path preview image
    saveas(gcf, output_filepath);

end
