%--------------------------------------------------------------------------
% Name:        gps_to_ned
% Description: Converts a lat, lon, and alt curvilinear position to an x, 
%              y, and z NED position given lat, lon, and alt of the origin.
% Arguments:   - p:  lat, lon, and alt in deg/deg/m to be converted
%              - p0: lat, lon, and alt in deg/deg/m of the origin
% Returns:     Measurement converted to NED position as x, y, and z NED 
%              position in meters.
%--------------------------------------------------------------------------
function r = gps_to_ned(p, p0)

    % Convert the lat and lon to radians
    p(:,1:2) = deg2rad(p(:,1:2));
    p0(:,1:2) = deg2rad(p0(:,1:2));
    
    % Calculate the difference from the origin for each measurement
    for i = 1:size(p, 1)
        p(i,:) = p(i,:) - p0;
    end
    
    % Multiply the difference in radians by the radius of the earth to get 
    % meters from the origin
    p(:,1:2) = p(:,1:2) * 6371000;

    r = p;

end