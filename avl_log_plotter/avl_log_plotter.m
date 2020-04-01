%==========================================================================
% Autonomous Vehicle Library
%
% Description: Parses and plots the data logs generated by AVL.
%==========================================================================

close all; clear all; clc;
addpath('functions');
addpath('mex');
addpath('plot_me');
disp('Running data plotter...');

%% Setttings

% Plots the log files in /var/avl_logs/current/log if true, otherwise plots
% the first log file in the base filepath
plot_current = false;

% Folder in which to look for log folders to be parsed and plotted
base_filepath = 'plot_me/';

% Time axis limits. All plot x axes will start and stop at these values. To
% use the full timespan of the log files, use -inf and +inf
t_start = -inf;
t_stop = inf;

% Re-parses log files into .dat files if set to true. Setting to false
% allows faster plotting if .dat files are already generated
reparse_logs = false;

% List of files to ignore for parsing and plotting. These logs aren't
% really plottable
ignore_files = [
    "command_node.log"
    "tcp_server_node.log"
    "mission_node.log"
    "safety_node.log"
    "status_node.log"
    ];

%% Parse AVL Log Files

% Get a list of folders in the folder set as the base filepath. Remove the
% . and .. entries that matlab includes, and remove items that are not folders
log_folder_names = dir(base_filepath);
log_folder_names(ismember( {log_folder_names.name}, {'.', '..'})) = [];
log_folder_names([log_folder_names.isdir] == 0) = [];

% Pick the first of the folders found and assemble a filepath to the log
% folder inside it
if plot_current
    log_filepath = '/var/avl_logs/current/log';
else
    log_filepath = fullfile(log_folder_names(1).folder, log_folder_names(1).name, 'log');
end

% Set up the dat folder path
mkdir([log_filepath '/../'], 'dat')
dat_filepath = [log_filepath '/../dat'];

% Check for already existing .dat files. If there are some, we don't need
% to re-parse every log file
if isempty(dir(fullfile(dat_filepath, '*.dat')))

    disp(' ');
    disp('Parsing log files into dat files...');

    % Get a list of all log files in the directory and parse them into dat
    % files
    log_filenames = dir(fullfile(log_filepath, '*.log'));
    for i = 1:size(log_filenames,1)

        % Skip the file if it's on the ignore list
        if ismember(log_filenames(i).name, ignore_files)
            disp(['Ignoring file ' log_filenames(i).name]);
            continue;
        end

        parse_log(fullfile(log_filenames(i).folder, log_filenames(i).name));

    end
    
    % Move all data files to the dat folder
    movefile([log_filepath '/*.dat'], dat_filepath)
    
    clear mex

else
    disp(' ');
    disp('Dat files found, skipping log parsing');
end

% if reparse_logs
%     % Delete any existing parsed dat files from previous runs
%     delete([log_filepath '*.dat']);
% end

%% Determine Start and End Times

disp(' ');
disp('================================================================================');
disp(' ');
disp('Determining log timespan...');
disp(' ');

% We want plots to have an x-axis of seconds since logging started, and not
% epoch time. We also want the x axis to cover the full range of timestamps
% within the log files for every plot. We can find the earliest and latest
% of all timestamps to get the x axis limits, and subtract the start
% timestamp from all timestamps to get time in seconds

% Variable to keep track of the earliest and latest timestamp in the log
% files
t_earliest = realmax;
t_latest = -1.0;

% Get a list of all dat files in the directory and check the first and last
% timestamp in each one by reading in the data file
dat_filenames = dir(fullfile(dat_filepath, '*.dat'));
for i = 1:size(dat_filenames,1)

    try

        % Read the file contents
        content = read_dat_file(fullfile(dat_filenames(i).folder, dat_filenames(i).name));

        % Get the first and last timestamps
        t_first = content.t(1);
        t_last = content.t(end);

        % If this file's first timestamp is earlier than the current
        % earliest timestamp, save it
        if t_first < t_earliest
            t_earliest = t_first;
        end

        % If this file's last timestamp is later than the current latest
        % timestamp, save it
        if t_last > t_latest
            t_latest = t_last;
        end

    catch ex

        % The read_dat_file function will print an error if it fails, no
        % need to do anything here

    end

end

disp(' ');
disp(['Earliest timestamp: ' num2str(t_earliest)]);
disp(['Latest timestamp:   ' num2str(t_latest)]);

%% Plot Data

disp(' ');
disp('================================================================================');
disp(' ');
disp('Plotting dat files...');
disp(' ');

% Set all plots to docked
set(0,'DefaultFigureWindowStyle','docked');

% Get a list of all dat files in the directory and plot them
dat_filenames = dir(fullfile(dat_filepath, '*.dat'));
for i = 1:size(dat_filenames,1)

    try

        % Read in the log data from the .dat file
        log_data = read_dat_file(fullfile(dat_filenames(i).folder, dat_filenames(i).name));

        % If the dat file has data to be plotted, plot it
        if ~isempty(log_data)
            plot_log_data(log_data, dat_filenames(i).name(1:end-4), t_earliest, t_latest, t_start, t_stop);
        end

        % ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        % Save variables for custom plots

        if dat_filenames(i).name == "inertial_nav_ekf_parser_node.pos_cov.dat" || ...
           dat_filenames(i).name == "inertial_nav_ukf_parser_node.pos_cov.dat" || ...
           dat_filenames(i).name == "inertial_nav_ekf_node.pos_cov.dat"

            for j = 1:size(log_data.data,1)
                C(:,:,j) = [log_data.data(j,4) log_data.data(j,3); log_data.data(j,2) log_data.data(j,1)];
            end

            % Convert covariance in m^2 to deg^2
            C = (rad2deg(sqrt(C) ./ 6371000)).^2;

        end

        if dat_filenames(i).name == "straight_lines_guidance_node.path.dat"
            cur_lat = log_data.data(:,1);
            cur_lon = log_data.data(:,2);
            start_lat = log_data.data(:,3);
            start_lon = log_data.data(:,4);
            goal_lat = log_data.data(:,5);
            goal_lon = log_data.data(:,6);
%             goal_dist = ;
%             goal_yaw = ;
%             path_yaw = ;
%             relative_yaw = ;
        end

        if dat_filenames(i).name == "inertial_nav_ekf_parser_node.position.dat" || ...
           dat_filenames(i).name == "inertial_nav_ukf_parser_node.position.dat" || ...
           dat_filenames(i).name == "inertial_nav_ukf_node.position.dat" || ...
           dat_filenames(i).name == "inertial_nav_ekf_node.position.dat"
            lat_ekf = log_data.data(:,1);
            lon_ekf = log_data.data(:,2);
        end
        
        if dat_filenames(i).name == "inertial_nav_ekf_parser_node.range.dat" || ...
           dat_filenames(i).name == "inertial_nav_ekf_node.range.dat" || ...
           dat_filenames(i).name == "inertial_nav_ukf_node.range.dat" || ...
           dat_filenames(i).name == "inertial_nav_ukf_parser_node.range.dat"
            t_range = log_data.t;
            r_meas = log_data.data(:,1);
            r_est = log_data.data(:,2);
            lat_vehicle_range = log_data.data(:,3);
            lon_vehicle_range = log_data.data(:,4);
            alt_vehicle_range = log_data.data(:,5);
            lat_beacon_range = log_data.data(:,6);
            lon_beacon_range = log_data.data(:,7);
            alt_beacon_range = log_data.data(:,8);
            range_accepted = log_data.data(:,9);
        end

        if dat_filenames(i).name == "inertial_nav_ekf_parser_node.velocity.dat" || ...
           dat_filenames(i).name == "inertial_nav_ekf_node.v_eb_b.dat"
            t_ekf = log_data.t;
            v_eb_b_ekf = log_data.data(:,1:3);
        end
                
        if dat_filenames(i).name == "inertial_nav_ekf_parser_node.attitude.dat" || ...
           dat_filenames(i).name == "inertial_nav_ekf_node.theta_n_b.dat"
            t_ekf = log_data.t;
            theta_n_b_ekf = log_data.data(:,1:3);
        end

        if dat_filenames(i).name == "gps_node.gps.dat" || ...
           dat_filenames(i).name == "gps_sim_node.gps.dat" || ...
           dat_filenames(i).name == "inertial_nav_ekf_parser_node.gps.dat" || ...
           dat_filenames(i).name == "inertial_nav_ukf_parser_node.gps.dat"
            t_gps = log_data.t;
            lat_gps = log_data.data(:,1);
            lon_gps = log_data.data(:,2);
        end
        
        if dat_filenames(i).name == "dvl_sim_node.velocity.dat"
            t_dvl = log_data.t;
            v_dvl = log_data.data(:,1:3);
        end
        
        if dat_filenames(i).name == "ahrs_sim_node.attitude.dat"
            t_ahrs = log_data.t;
            theta_n_b_ahrs = log_data.data(:,1:3);
        end
        
        if dat_filenames(i).name == "ned_kinematics_node.theta_n_b.dat"
            t_true = log_data.t;
            theta_n_b_true = log_data.data(:,1:3);
        end
        
        if dat_filenames(i).name == "ned_kinematics_node.v_eb_b.dat"
            t_true = log_data.t;
            v_eb_b_true = log_data.data(:,1:3);
        end

        if dat_filenames(i).name == "waypoint_control_node.waypoint.dat"
            t_wp = log_data.t;
            lat_wp = log_data.data(:,3);
            lon_wp = log_data.data(:,4);
        end
        
        if dat_filenames(i).name == "rogue_beacon_sim_node.position.dat"
            t_rbeacon = log_data.t;
            lat0_rbeacon = log_data.data(:,1);
            lon0_rbeacon = log_data.data(:,2);
            alt0_rbeacon = log_data.data(:,3);
            lat_rbeacon = log_data.data(:,4);
            lon_rbeacon = log_data.data(:,5);
            alt_rbeacon = log_data.data(:,6);
        end
        
        if dat_filenames(i).name == "sonar_logging_node.sonar.dat"
            t_sonar = log_data.t;
            sonar_data = log_data.data(:,1:end-1);
            num_scans = length(t_sonar);
            num_points = size(sonar_data,2)/3;
            for i = 1:num_scans
                scan_data(:,:,i) = reshape(sonar_data(i,:), 3, num_points);
                scan_data(1,:,i) = repmat(t_sonar(i),num_points,1);
            end
            sonar_data = [];
            for i = 1:num_scans
                sonar_data = [sonar_data; scan_data(:,:,i)'];
            end
        end

    catch ex

        % The read_dat_file function will print an error if it fails, no
        % need to do anything here

    end

end

% Close all open files, if there are any
fclose('all');

disp(' ');
disp('================================================================================');
disp(' ');
disp('Log plotter finished');
disp(' ');

%% Custom map plot
figure('Color', [0.1 0.1 0.1])
set(gcf, 'name', 'Map', 'numbertitle', 'off')
set(gca, 'LooseInset', get(gca,'TightInset'))
hold on

% Plot straight line guidance
% h_waypoint = plot(start_lon, start_lat, 'gd', 'MarkerSize', 15, 'MarkerFaceColor', 'g');
% plot(goal_lon(end), goal_lat(end), 'gd', 'MarkerSize', 15, 'MarkerFaceColor', 'g');
% plot(start_lon, start_lat, 'g--');
% plot(goal_lon(end), goal_lat(end), 'g--');

% Plot GPS
h_gps = plot(lon_gps, lat_gps, 'y+');


% Plot EKF estimate
h_ekf = plot(lon_ekf, lat_ekf, 'b.');

% % Plot OWTT origin locations
% plot(p_origin_owtt(:,2), p_origin_owtt(:,1), 'rx');


% Plot EKF estimate
% plot(cur_lon, cur_lat, 'rx');

% Plot rogue beacon
% plot(lon_rbeacon, lat_rbeacon, 'r.');

% Plot error elipses
% for i = 1:1000:size(lat_ekf,1)
%     hold on
%     error_ellipse('mu',[lon_ekf(i); lat_ekf(i)], ...
%                   'C', C(:,:,i), ...
%                   'style', 'r--');
% end

% Plot range measurements
% for i = 1:size(t_range,1)
%     hold on
%     if range_accepted(i)
%         h_accepted = plot([lon_vehicle_range(i); lon_beacon_range(i)], [lat_vehicle_range(i); lat_beacon_range(i)], 'g--');
%         plot(lon_vehicle_range(i), lat_vehicle_range(i), 'gx');
%         h_boat = plot(lon_beacon_range(i), lat_beacon_range(i), 'cx');
%     else 
%         h_rejected = plot([lon_vehicle_range(i); lon_beacon_range(i)], [lat_vehicle_range(i); lat_beacon_range(i)], 'r--');
%         plot(lon_vehicle_range(i), lat_vehicle_range(i), 'rx');
%         plot(lon_beacon_range(i), lat_beacon_range(i), 'cx');
%     end
% end

% Plot the underlying Google map
% plot_google_map('APIKey','AIzaSyDlpY42E7cy7ChKi7RkSc3A2ol0XID5huE', ...
%                 'MapType', 'satellite', ...
%                 'MapScale', 0);

axis equal

% lgd = legend([h_gps, h_ekf, h_accepted, h_rejected, h_boat], 'GPS', 'EKF', 'Accepted Range', 'Rejected Range', 'Boat')
% lgd = legend([h_gps, h_ekf, h_waypoint], 'GPS', 'EKF', 'Waypoint');
% lgd.FontSize = 16;

ylabel('Range Aided EKF Estimate');
xlabel('Longitude (deg)');
ylabel('Latitude (deg)');

set(gca,'Color', [0.1 0.1 0.1]);

%% EKF Attitude Comparison

% figure
% set(gcf, 'name', 'EKF Attitude', 'numbertitle', 'off')
% set(gca, 'LooseInset', get(gca,'TightInset'))
% hold on
% 
% % Plot DVL
% plot(t_true, theta_n_b_true(:,1), 'r-');
% plot(t_true, theta_n_b_true(:,2), 'g-');
% plot(t_true, theta_n_b_true(:,3), 'b-');
% 
% % Plot EKF estimate
% plot(t_ekf, theta_n_b_ekf(:,1), 'r.');
% plot(t_ekf, theta_n_b_ekf(:,2), 'g.');
% plot(t_ekf, theta_n_b_ekf(:,3), 'b.');
% 
% clickable_legend('\theta_x_{true}', '\theta_y_{true}', '\theta_z_{true}', ...
%                  '\theta_x_{ekf}', '\theta_y_{ekf}', '\theta_z_{ekf}')

%% EKF Velocity Comparison

% figure
% set(gcf, 'name', 'EKF Velocity', 'numbertitle', 'off')
% set(gca, 'LooseInset', get(gca,'TightInset'))
% hold on
% 
% % Plot DVL
% plot(t_true, v_eb_b_true(:,1), 'r-');
% plot(t_true, v_eb_b_true(:,2), 'g-');
% plot(t_true, v_eb_b_true(:,3), 'b-');
% 
% % Plot EKF estimate
% plot(t_ekf, v_eb_b_ekf(:,1), 'r.');
% plot(t_ekf, v_eb_b_ekf(:,2), 'g.');
% plot(t_ekf, v_eb_b_ekf(:,3), 'b.');

%% Plot Sonar Data

% figure
% set(gcf, 'name', 'Sonar', 'numbertitle', 'off')
% set(gca, 'LooseInset', get(gca,'TightInset'))
% hold on
% 
% scatter3(sonar_data(:,1), sonar_data(:,2), sonar_data(:,3), 5, sonar_data(:,3))
% axis equal
% colormap(jet)
% colorbar