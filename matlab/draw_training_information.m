%% Parameters
% Name of input log file
name = '../training.log';
% Define axis limits (if empty, retrieves max from log file)
iterations_lim = [];
loss_lim = [0 10];

% Name of temp file
name_tmp = 'temp.log';

%% Processing

% First preprocess file outside of matlab (faster)
result = unix(['cat ' name ' | grep images > ' name_tmp ]);

if(result)
    error('Unable to preprocess training file. Does it exist?');
end

% Open temp file
fid = fopen(name_tmp);

% Read line by line
tline = fgetl(fid);

% Iteration versus loss
iter_vs_loss = [];

while ischar(tline)
    % Extract specific information from line
    % First get iteration
    pos1 = strfind(tline, ':');
    iteration = str2double(tline(1:pos1-1));
    
    % Next get loss
    pos2 = strfind(tline, ',');
    loss = str2double(tline(pos1+2:pos2-1));
    
    % Next get average loss
    pos3 = strfind(tline, 'avg');
    avg_loss = str2double(tline(pos2+2:pos3-2));
    
    % Store information
    iter_vs_loss = [iter_vs_loss; iteration loss avg_loss];

    % Get next line
    tline = fgetl(fid);

end

fclose(fid);

%% Draw figure

% Plot loss rate
figure(1),clf;
plot(iter_vs_loss(:,1), iter_vs_loss(:,2), 'r', 'linewidth', 2);
hold on;
plot(iter_vs_loss(:,1), iter_vs_loss(:,3), 'b', 'linewidth', 2);

% Cleanup figure
grid on;
xlabel('# Iterations');
ylabel('Training loss');
legend('Loss rate','Averaged loss rate','location', 'ne');
set(gca,'LooseInset',get(gca,'TightInset'));

if(isempty(iterations_lim))
    iterations_lim = [1 iter_vs_loss(end,1)];
end

if(isempty(loss_lim))
    loss_lim = [0 max(iter_vs_loss(:,2))];
end

axis([iterations_lim loss_lim]);



