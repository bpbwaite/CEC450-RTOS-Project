%% Import data from text file

%% Set up the Import Options and import the data
opts = delimitedTextImportOptions("NumVariables", 7);
% Specify range and delimiter
opts.DataLines = [13, Inf];
opts.Delimiter = ",";
% Specify column names and types
opts.VariableNames = ["Source", "CH1", "CH2", "CH3", "CH4", "VarName6", "VarName7"];
opts.VariableTypes = ["double", "double", "double", "double", "double", "double", "double"];
% Specify file level properties
opts.ExtraColumnsRule = "ignore";
opts.EmptyLineRule = "read";
% Specify variable properties
opts = setvaropts(opts, ["VarName6", "VarName7"], "TrimNonNumeric", true);
opts = setvaropts(opts, ["VarName6", "VarName7"], "ThousandsSeparator", ",");
% Import the data
SDS00001 = readtable("C:\Users\bpbra\Desktop\_Working\CEC450-RTOS-Project\SDS00001.csv", opts);
%% Convert to output type
SDS00001 = table2array(SDS00001);
%% Set up the Import Options and import the data
opts = delimitedTextImportOptions("NumVariables", 7);
% Specify range and delimiter
opts.DataLines = [13, Inf];
opts.Delimiter = ",";
% Specify column names and types
opts.VariableNames = ["Source", "CH1", "CH2", "CH3", "CH4", "VarName6", "VarName7"];
opts.VariableTypes = ["double", "double", "double", "double", "double", "double", "double"];
% Specify file level properties
opts.ExtraColumnsRule = "ignore";
opts.EmptyLineRule = "read";
% Specify variable properties
opts = setvaropts(opts, ["VarName6", "VarName7"], "TrimNonNumeric", true);
opts = setvaropts(opts, ["VarName6", "VarName7"], "ThousandsSeparator", ",");
% Import the data
SDS00002 = readtable("C:\Users\bpbra\Desktop\_Working\CEC450-RTOS-Project\SDS00002.csv", opts);
%% Convert to output type
SDS00002 = table2array(SDS00002);
%% Set up the Import Options and import the data
opts = delimitedTextImportOptions("NumVariables", 7);
% Specify range and delimiter
opts.DataLines = [13, Inf];
opts.Delimiter = ",";
% Specify column names and types
opts.VariableNames = ["Source", "CH1", "CH2", "CH3", "CH4", "VarName6", "VarName7"];
opts.VariableTypes = ["double", "double", "double", "double", "double", "double", "double"];
% Specify file level properties
opts.ExtraColumnsRule = "ignore";
opts.EmptyLineRule = "read";
% Specify variable properties
opts = setvaropts(opts, ["VarName6", "VarName7"], "TrimNonNumeric", true);
opts = setvaropts(opts, ["VarName6", "VarName7"], "ThousandsSeparator", ",");
% Import the data
SDS00003 = readtable("C:\Users\bpbra\Desktop\_Working\CEC450-RTOS-Project\SDS00003.csv", opts);
%% Convert to output type
SDS00003 = table2array(SDS00003);
%% Clear temporary variables
clear opts

