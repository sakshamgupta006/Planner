function leg = clickable_legend(varargin)
%CLICKABLELEGEND Display a clickable legend that allows you to turn traces
%on and off.
%
%   CLICKABLELEGEND(...) works exactly like legend(...), except that it
%   allows you to click on your labels in the legend to turn traces on
%   and off. You call CLICKABLELEGEND(...) with the exact same arguments
%   that you would call legend(...) with.
%
%   For more information on how to use legend(...) see the legend help.
%
%   See also LEGEND.
%
%   Author: Ben Straub, bstraub@vt.edu

%   2/11/16 10:08: added a call to the default CB function to allow 
%   dragging of the legend.
%   2/11/16 11:00: I use findobj to find other lines/patches that have all
%   the same color and properties and stuff. Then I treat them the same,
%   i.e. I also make them invisible/visible. This way, you can have
%   multiple identical lines and turn them all off from a single legend
%   entry.

    evalStr = 'l = legend(';
    for idx = 1:length(varargin)
        evalStr = [evalStr sprintf('varargin{%d},',idx)];
    end
    evalStr(end) = ')'; %replace last comma with )
    
    eval([evalStr ';']);
    
%     leg = legend(varargin);
    oldCB = get(l,'ButtonDownFcn');
    set(l,'ButtonDownFcn',{@legendClickCB,gca,oldCB});
    ylim(ylim); xlim(xlim);
    
    if nargout == 1
        leg = l;
    end
end

function legendClickCB(src,evt,ax,oldCB)
    %how many things are in the legend?
    N = length(src.String);
    
    %figure out which thing we clicked on
    region = ceil(N*evt.IntersectionPoint(2));
    if region == 0 %this happens if you click right on the bottom border
        region = 1; %just make it 1 instead
    end
    
    %get the actual plot lines (and other junk) from the plot [aka ax]
    lines = get(ax,'children');
    %go through and remove anything that's not a line or patch
    for idx = length(lines):-1:1
        if ~( strcmp(lines(idx).Type,'line') || strcmp(lines(idx).Type,'patch') )
            lines(idx)=[]; %bad type of object (probably an image) so delete it
        end
    end
    
    %and get the individual plot line corresponding to what we clicked
    line = lines(length(lines)-N+region);
    
%     findobj(ax,'Displayname',src.String{N-region+1});
    
    status = get(line,'visible'); %check if it's visible
    if strcmp(line.Type,'line')
        isline = true;
        oldcolor = get(line,'color'); %and get its color
    else
        isline = false;
        oldcolor = get(line,'faceColor');
    end
    if strcmp(status,'off') %if it's not visible then we previously turned it off
        %so make it visible again
        newVisibility = 'on';
        newcolor = (1-10*(1-oldcolor)); %make it its old (bright) color again
    else %its visible
        %make it invisible
        newVisibility = 'off';
        newcolor = (1-0.1*(1-oldcolor)); %make it a very faded version of its color
    end
   
    if isline
        clones = findobj(lines,'Color',line.Color,'LineStyle',line.LineStyle, ...
            'LineWidth',line.LineWidth,'Marker',line.Marker, ...
            'MarkerSize',line.MarkerSize,'MarkerFaceColor',line.MarkerFaceColor,...
            'MarkerEdgeColor',line.MarkerEdgeColor,'Type','line');
    else
        clones = findobj(lines,'EdgeAlpha',line.EdgeAlpha, ...
            'EdgeColor',line.EdgeColor,'FaceAlpha',line.FaceAlpha, ...
            'Marker',line.Marker,'MarkerSize',line.MarkerSize, ...
            'MarkerEdgeColor',line.MarkerEdgeColor, ...
            'MarkerFaceColor',line.MarkerFaceColor,'Type','patch');
    end
    
    if isline
        for idx = 1:length(clones)
            set(clones(idx),'color',newcolor); %set it to the new color
            set(clones(idx),'visible',newVisibility);
        end
    else
        for idx = 1:length(clones)
            set(clones(idx),'faceColor',newcolor); %set it to the new color
            set(clones(idx),'visible',newVisibility);
        end
    end
    
    
    oldCB(src,evt);
end