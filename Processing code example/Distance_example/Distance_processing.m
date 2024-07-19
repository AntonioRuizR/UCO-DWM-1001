clear;
clc;

%% Compare estimated data and real data in LOS scenario (1 meter example)

Real_distance = 1; %Real distance to compare in meters

Data_1m_LOS = readtable('Test_1m_LOS.csv'); % CSV file with distance data
save('Test_1m_LOS.mat','Data_1m_LOS');

Distance_1m_LOS = table2array(Data_1m_LOS(:,3));
Time_1m_raw = table2array(Data_1m_LOS(:,2));
Index_1m = table2array(Data_1m_LOS(:,1));
Time_1m = datetime(Time_1m_raw);
Seconds_1m = seconds(Time_1m(end) - Time_1m(1));
Seconds_1m_plot = transpose(linspace(1,Seconds_1m,Index_1m(end-270)));

%Calc

media_1m = mean(Distance_1m_LOS(270:end));
sigma_1m = std(Distance_1m_LOS(270:end));
offset_1m = media_1m - Real_distance;
median_1m = median(Distance_1m_LOS(270:end));
mode_1m = mode(Distance_1m_LOS(270:end));
max_1m = max(Distance_1m_LOS(270:end));
min_1m = min(Distance_1m_LOS(270:end));

figure('Name','X meters distance: LOS scenario','NumberTitle','off')

plot(Seconds_1m_plot,Distance_1m_LOS(271:end))
ylim([0 1.5]) % Set y-axis range
xlim([0 Seconds_1m_plot(end)]) % Set x-axis range
annotation('textbox',[.15 .1 .1 .1],'String',['Media: ' sprintf('%.4f',media_1m) ' m'],'FitBoxToText','on');
annotation('textbox',[.4 .1 .1 .1],'String',['sigma: ' sprintf('%.4f',sigma_1m) ' m'],'FitBoxToText','on');
annotation('textbox',[.65 .1 .1 .1],'String',['offset: ' sprintf('%.4f',offset_1m) ' m'],'FitBoxToText','on');
annotation('textbox',[.15 .20 .1 .1],'String',['Median: ' sprintf('%.4f',median_1m) ' m'],'FitBoxToText','on');
annotation('textbox',[.40 .20 .1 .1],'String',['Mode: ' sprintf('%.4f',mode_1m) ' m'],'FitBoxToText','on');
xlabel('Time (Seconds)')
ylabel('Measured distance')

%Figure Setup (optional)
f = figure(1); % Selecciona la figura
set(f,'Units', 'Centimeters', 'Position', [10, 5, 15, 10]);

% Ejes
xlabel('Time (s)')
ylabel('Distance (cm)')
set(gca,'Fontsize', 10)

% Axis limit
ylim([50, 150]);
%xlim([0, 150]);

% Graph properties
% One graph considered. More graphs can be added: curve(2)...
graphs = get(gca,'children');
graph_1 = graphs(1);
set(graph_1, 'YData', graph_1.YData*100); % Paso a cm
set(graph_1, 'LineWidth', 1); % Grosor curva
%set(graph_1, 'LineStyle', '-'); % C
%set(graph_1, 'MarkerSize', 8);
%set(graph_1, 'Marker', '*');% Dot size: 8

% Leyendas
legend('Distance')

% Optional figure save as png
%print('Distance_1m_LOS', '-dpng', '-r300') % Resolution: 300 dpi