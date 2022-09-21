%% Author Qijun Tang

clear all

%% 1.setting parameters
% manually put the info about your data
t_int=0.667; % INTERVAL (sample interval, or the time between data points, in minutes multiplied by 4) (0.667 is for the default interval: 10s)
t_start='11:00'; % START TIME (24 hour) Always is 2 digits for both hour and minute, as in 04:20
date_start='08-AUG-2022'; % START DATE (dd-mon-yyyy), where the month uses stardard 3-letter abbreviations (lower case)
n_Chn=4; % number of channels of your system, default is 4

%% 2.read and process activity
[filename,path] = uigetfile('*.txt');
cd(path);
data_act = readtable(filename);
%% 3.save the activity data to matlab format
save('act','data_act');
%% 4.export activity data to clocklab format
for n=1:n_Chn
table_act=cell(size(data_act,1)+7,1); %there is a 7-row header
%make header
table_act(1)={['act-Chn',num2str(n)]};
table_act(2)={date_start}; % START DATE (dd-mon-yyyy), where the month uses stardard 3-letter abbreviations (lower case)
table_act(3)={t_start}; % START TIME (24 hour) Always is 2 digits for both hour and minute, as in 04:20
table_act(4)={t_int}; % INTERVAL (sample interval, or the time between data points, in minutes multiplied by 4)
table_act(5)={'C57BL6J'}; % place holder that you can put info
table_act(6)={'TRF'}; % place holder that you can put info
table_act(7)={'male'}; % place holder that you can put info
table_act(8:end)=table2cell(data_act(:,2*n+1));
% export
filename_act=[table_act{1},'.awd'];
writecell(table_act,filename_act,'FileType','text');
end