function log_data = read_dat_file(filepath)

    fid = fopen(filepath);
    
    % Read the first line and split by spaces to get a vector of labels.
    % Remove the first entry because it is a timestamp
    line = fgetl(fid);
    log_data.labels = regexp(line,' ','split');
    log_data.labels(1) = [];
    
    % Read the second line and split by spaces to get a vector of units.
    % Remove the first entry because it is a timestamp
    line = fgetl(fid);
    log_data.units = regexp(line,' ','split');
    log_data.units(1) = [];
    
    fclose(fid);

    try
        
        % Read the space separated date, starting after the first two
        % lines with labels and units
        dlm_data = dlmread(filepath, ' ', 2, 0);
        
        % Separate the timestamps from the rest of the data
        log_data.t = dlm_data(:,1);
        log_data.data = dlm_data(:,2:end);

    catch ex
        
        disp(['Failed to load dat file ' filepath ' (' ex.message ')']);
        log_data = [];
        
    end

end