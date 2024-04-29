%% Datos de la prueba posicion Rombo

clear;
clc;

Datos_mov_rombo_1 = readtable('Position_path_test.csv'); %Name of csv file

Indice_mov_rombo_1 = table2array(Datos_mov_rombo_1(:,1));
elementos_mov_rombo_1 = length(Indice_mov_rombo_1);
posicion_X_mov_rombo_1 = table2array(Datos_mov_rombo_1(:,3));
posicion_Y_mov_rombo_1 = table2array(Datos_mov_rombo_1(:,5));
posicion_Z_mov_rombo_1 = table2array(Datos_mov_rombo_1(:,7));
Tiempo_pos_raw_mov_rombo_1 = table2array(Datos_mov_rombo_1(:,2));
Tiempo_pos_mov_rombo_1 = datetime(Tiempo_pos_raw_mov_rombo_1);

Tramo_1_x = linspace(1,4,400)';
Tramo_1_y = linspace(1,1,400)';

Tramo_2_x = linspace(4,4,400)';
Tramo_2_y = linspace(1,2.25,400)';

Tramo_3_x = linspace(4,1,400)';
Tramo_3_y = linspace(2.25,2.25,400)';

Tramo_4_x = linspace(1,1,400)';
Tramo_4_y = linspace(2.25,3.5,400)';

Tramo_5_x = linspace(1,4,400)';
Tramo_5_y = linspace(3.5,3.5,400)';

Trayectoria_x = [Tramo_1_x ; Tramo_2_x ; Tramo_3_x ; Tramo_4_x ; Tramo_5_x];
Trayectoria_y = [Tramo_1_y ; Tramo_2_y ; Tramo_3_y ; Tramo_4_y; Tramo_5_y];
Trayectoria_z = linspace(1.2,1.2,2000)';

Real = [Trayectoria_x Trayectoria_y Trayectoria_z ];
Medido = [posicion_X_mov_rombo_1 posicion_Y_mov_rombo_1 posicion_Z_mov_rombo_1];

D = sqrt((Medido(:,1)-Real(:,1).').^2 + ...
    (Medido(:,2)-Real(:,2).').^2 + ...
    (Medido(:,3)-Real(:,3).').^2);
[Dmin closestpart] = min(D,[],2);
%iscloseenough = Dmin <= Dtolerance;

D_xy = sqrt((Medido(:,1)-Real(:,1).').^2 + ...
    (Medido(:,2)-Real(:,2).').^2);
[Dmin_xy closestpart_xy] = min(D_xy,[],2);

Error_medio_3ejes = mean(Dmin)*100; %Error medio considerando ejes xyz en cm
Error_medio_plano_xy = mean(Dmin_xy)*100; %Error medio considerando ejes xy en cm

figure(1)
plot3(Real(:,1),Real(:,2),Real(:,3)); 
hold on
plot3(Medido(:,1),Medido(:,2),Medido(:,3),'*');
axis equal
grid on

diferencias = [];

for k = 1:numel(closestpart)
    a = Medido(k,:);
    b = Real(closestpart(k),:);
    diferencias = [diferencias; (a-b)]; %Cálculo de las diferencias en cada eje para hallar error medio
    plot3([a(1) b(1)],[a(2) b(2)],[a(3) b(3)],'g')
end

legend('Real path', 'Estimated position', 'Error')

xlim([0 5]);
ylim([0 4.5]);
zlim([0 2.5]);
xlabel('Eje X (m)');
ylabel('Eje Y (m)');
zlabel('Eje Z (m)');

Error_medio_x = mean(diferencias(:,1))*100;
sigma_medio_x = std(diferencias(:,1))*100;

Error_medio_y = mean(diferencias(:,2))*100;
sigma_medio_y = std(diferencias(:,2))*100;

Error_medio_z = mean(diferencias(:,3))*100;
sigma_medio_z = std(diferencias(:,3))*100;

diferencias_abs = abs(diferencias);

[~, idx_max] = max(diferencias_abs(:,1));
[~, idy_max] = max(diferencias_abs(:,2));
[~, idz_max] = max(diferencias_abs(:,3));

[~, idx_min] = min(diferencias_abs(:,1));
[~, idy_min] = min(diferencias_abs(:,2));
[~, idz_min] = min(diferencias_abs(:,3));

Error_max_x = diferencias(idx_max,1)*100;
Error_min_x = diferencias(idx_min,1)*100;

Error_max_y = diferencias(idy_max,2)*100;
Error_min_y = diferencias(idy_min,2)*100;

Error_max_z = diferencias(idz_max,3)*100;
Error_min_z = diferencias(idz_min,3)*100;

f1 = figure(1)
set(f1,'Units', 'centimeters', 'Position', [15, 10, 20, 15]);

%FIGURA PARA PLANO XY
figure(2)
plot(Real(:,1),Real(:,2)); 
hold on
plot(Medido(:,1),Medido(:,2),'*');
axis equal
grid on

for k = 1:numel(closestpart_xy)
    a = Medido(k,:);
    b = Real(closestpart_xy(k),:);
    plot([a(1) b(1)],[a(2) b(2)],'g')
end

xlim([0 5]);
ylim([0 4.5]);
xlabel('X-Axis (m)');
ylabel('Y-Axis (m)');

legend('Real path', 'Estimated position', 'Error')

f2 = figure(2)
set(f2,'Units', 'centimeters', 'Position', [15, 10, 20, 15]);

% Tabla de datos
T = table(Error_medio_x, sigma_medio_x, Error_max_x, Error_min_x, Error_medio_y, sigma_medio_y, Error_max_y, Error_min_y, Error_medio_z,...
    sigma_medio_z, Error_max_z, Error_min_z, 'VariableNames', {'X media error', 'Sigma X', 'Error Max X', 'Error Min X', 'Y media error', 'Sigma Y',...
    'Error Max Y', 'Error Min Y', 'Z media error', 'Sigma Z', 'Error Max Z', 'Error Min Z',});
