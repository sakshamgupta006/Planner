function plot_log_data(log_data, log_name, t_first, t_last, t_start, t_stop)
        
        % Light Theme
%         bg_color = [1.0 1.0 1.0];
%         fg_color = [0.0 0.0 0.0];
        
        % Dark Theme
        bg_color = [0.1 0.1 0.1];
        fg_color = [1.0 1.0 1.0];
        
        line_colormap = lines;
        line_width = 1.5;
        marker_size = 10;

        % Convert timestamps in epoch time to seconds since the start time t0
        log_data.t(:,1) = log_data.t(:,1) - t_first;
        t_last = t_last - t_first;
        t_first = 0.0;
        
        if t_start == -inf
            t_start = t_first;
        end
        
        if t_stop == inf
            t_stop = t_last;
        end        

        % Figure setup
        figure('Color', bg_color, 'InvertHardcopy', 'off')
        set(gcf,'name',log_name,'numbertitle','off')
        set(gca, 'LooseInset', get(gca,'TightInset'))
        hold on
        
        % Plots
        try
            plot(log_data.t, log_data.data, '.-', 'LineWidth', line_width, 'MarkerSize', marker_size);
        catch ex
            disp(['Failed to plot ' log_name ' (' ex.message ')'])
        end
        
        % Line colors and styles
%         set(groot, 'defaultAxesColorOrder', line_colormap, 'defaultAxesLineStyleOrder', '-|--|:')

        % Labels
        title(log_name, 'Interpreter', 'none', 'color', fg_color);
        xlabel('t (sec)', 'color', fg_color);
        ylabel(log_data.units(1), 'color', fg_color);
        
        % Legend
        leg = clickable_legend(log_data.labels, 'color', fg_color);
        leg.Color = 'none';
        leg.Box = 'off';
        leg.TextColor = fg_color;

        % Font
        set(gca, 'FontName', 'Times New Roman')
        set(gca,'FontSize',29)
        
        % Grid
        xlim([t_start t_stop]);
        grid on
        box on
        set(gca,'GridLineStyle','--')
        
        % Background and axis colors
        set(gca,'Color', bg_color);
        set(gca,'XColor', fg_color);
        set(gca,'YColor', fg_color);

end

