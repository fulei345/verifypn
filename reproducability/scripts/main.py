import os
import numpy as np
from numpy import int64
import pandas as pd
import argparse

exp3_list["Our ", "BFS ", "DSF ","BestFS "]

# Exp 1, Exp 2, Exp 3, Exp 3.1 Tables
header_template_list = [["model", "places","transitions","run1","run2","run3","run4","run5"],
                        ["model", "query","prep","fire","eval","eval_count", "steps", "places","transitions","runs","totaltime"],
                        ["model", "query", "time"],
                        ["model", "query", "time"]]

# Columns to index
index_list = [[3,4,5,6,7],[2,3,4,5,6,9,10],[2],[2]]
# Columns to import to pands
usecols_list = [[0,1,2,3,4,5,6,7],[0,1,4,6,8,10,12,13,14,16,18],[0,1,3],[0,1,3]]
# In folders
in_folder = [True,False,False,True]
settings_int = 4

def mergeDFs(liste, how):
    df_result = pd.merge(liste[0],liste[1],how=how)
    for i in range(2,len(liste)):
        df_result = pd.merge(df_result,liste[i],how=how)
    return df_result

def getDirectories(path):
    dir_list = []
    # Get the directory for the results
    for root,dirs, _ in os.walk(path):
        for dir in dirs:
            dir_list.append(os.path.join(root, dir))
    dir_list.sort()
    return dir_list

def getSolos(df_list,index,header_list):
    length = len(df_list)
    df_write = df_list[index].copy() 
    for k in range(1, length):
        df_write = pd.merge(df_write,df_list[(index+k)%length], how="left",indicator=True)
        df_write = df_write.loc[df_write['_merge'] == 'left_only']
        header_delete = set(df_write.columns.values.tolist())
        header_delete.difference_update(set(header_list[index]))
        df_write = df_write.drop(columns=header_delete)
    return df_write


def getFiles(dir, results):
    file_list = []
    path_file_list = []
    for root,_, files in os.walk(dir):
        for file in files:
            if "results" in file or not results:
                file_list.append(file)
                path_file_list.append(os.path.join(dir,file))
    file_list.sort()
    path_file_list.sort()
    return file_list, path_file_list

def getDataFrame(file_list, setting_index, index):
    temp_index = index
    data_frames = []
    headers = []
    for file in file_list:
        header = header_template_list[setting_index].copy()
        for num in index_list[setting_index]:
            header[num] = "_".join([str(temp_index),str(header[num])])
        type_dict = dict.fromkeys(header, int64)
        type_dict["model"] = str
        df = pd.read_csv(file, usecols=usecols_list[setting_index], names=header, header=None, dtype=type_dict)
        data_frames.append(df)
        temp_index = temp_index + 1
        headers.append(header)
    return data_frames, headers, temp_index

def main(path, setting, inner, outer, solo, plots,winners):
    index = 1
    data_frames_list = []
    header_list = []
    merged_frames = []
    filename_list = []
    dir_list = []
    path_file_list = []
    unique = 0
    unique_df = None

    # Import dataframes
    dir_list = getDirectories(path)
    print(path)
    if len(dir_list) == 0:
        dir_list.append(path)
        files, path_file = getFiles(path,True)
        filename_list.append(files)
        path_file_list.append(path_file)
    else:
        for dir in dir_list:
            files, path_file = getFiles(dir,False)
            filename_list.append(files)
            path_file_list.append(path_file)

    for file_list in path_file_list:
        df_list, headers, index = getDataFrame(file_list, setting, index)
        data_frames_list.append(df_list)
        header_list.append(headers)
    
    # Merge all frames
    if setting < 2 or inner:
        for exp in data_frames_list:
            df_merge = mergeDFs(exp,"inner")
            merged_frames.append(df_merge)

        if in_folder[setting]:
            df_final = mergeDFs(merged_frames,"inner")
        else:
            df_final = merged_frames[0]
        print(df_final)

    print(filename_list)

    if inner:
        for i in range(len(dir_list)):
            if in_folder[setting]:
                header_set = set()
                header_delete = set(df_final.columns.values.tolist())
                for header in header_list[i]:
                    header_set = header_set.union(set(header))
                header_delete.difference_update(header_set)
                df_write = df_final.copy()
                dirs = os.path.split(dir_list[i])[-1]
                filename = dirs +"("
                for file in filename_list[i]:
                    filename = filename + file + " "
                filename = filename + ").csv"
                df_write = df_write.drop(columns=header_delete)
                print(df_write)
                filepath = os.path.join(path, filename)
                df_write.to_csv(filepath)
            else:
                for k in range(len(filename_list[0])):
                    header_delete = set(df_final.columns.values.tolist())
                    df_write = df_final.copy()
                    header_delete.difference_update(header_list[i][k])
                    filename = filename_list[0][i].replace("results","")
                    filename = filename + "-merged.csv"
                    df_write = df_write.drop(columns=header_delete)
                    print(df_write)
                    filepath = os.path.join(path, filename)
                    df_write.to_csv(filepath)

    if outer:
            for exp in data_frames_list:
                df_merge = mergeDFs(exp,"outer")
            df_write = df_merge["model"]
            print(df_write)
            filename = "outer.csv"
            filepath = os.path.join(path, filename)
            df_write.to_csv(filepath)

    if solo:
        for h in range(len(dir_list)):
            length = len(filename_list[h])
            for i in range(length):
                df_write = data_frames_list[h][i].copy() 
                for k in range(1, length):
                    df_write = pd.merge(df_write,data_frames_list[h][(i+k)%length], how="left",indicator=True)
                    df_write = df_write.loc[df_write['_merge'] == 'left_only']
                    header_delete = set(df_write.columns.values.tolist())
                    header_delete.difference_update(set(header_list[h][i]))
                    df_write = df_write.drop(columns=header_delete)
                temp = filename_list[h][i].replace("results","")
                filename = temp +"-solo.csv"
                print(df_write)
                filepath = os.path.join(path, filename)
                df_write.to_csv(filepath)
    if plots:
        if setting == 0:
            for i in range(len(dir_list)):
                ratio_list = []
                series_list = []
                for k in range(len(data_frames_list[i])):
                    header_delete = set(df_final.columns.values.tolist())
                    df_write = df_final.copy()
                    header_delete.difference_update(header_list[i][k])
                    header_delete.add("places")
                    header_delete.add("transitions")
                    df_write = df_write.drop(columns=header_delete)
                    result_series = df_write.median(axis='columns',numeric_only=True)
                    result_series = result_series.apply(lambda x: np.true_divide(x, 1000000))
                    ratio_series = result_series.copy(deep=True)
                    ratio_list.append(ratio_series)
                    result_series = result_series.sort_values(ignore_index=True)
                    series_list.append(result_series)
                df_write = pd.concat(series_list,ignore_index=True, axis=1)
                df_write.columns = ["Naive","Online"]
                print(df_write)
                "plot.csv"
                filepath = os.path.join(path, filename)
                df_write.to_csv(filepath)

                print(ratio_list)
                series_write = (ratio_list[1]/ ratio_list[0])
                series_write = series_write.sort_values(ignore_index=True)
                frame = {'Ratio': series_write}
                df_write = pd.DataFrame(frame)
                print(df_write)
                filename = "ratio-plot.csv"
                filepath = os.path.join(path, filename)
                df_write.to_csv(filepath)

        if setting == 1:
            # pps
            a = pd.Series([0.0001])
            a = a.repeat(len(df_final)).reset_index(drop=True)
            series_list = []
            series_list.append((df_final['1_prep']/ df_final['1_steps']))
            series_list.append(a)
            series_list.append((df_final['3_prep']/ df_final['3_steps']))
            series_list[0] = series_list[0].sort_values(ignore_index=True)
            series_list[2] = series_list[2].sort_values(ignore_index=True)
            df_write = pd.concat(series_list,ignore_index=True, axis=1)
            df_write.columns = ["A0","AT","AM"]
            print(df_write)
            filename = "pps-plot.csv"
            filepath = os.path.join(path, filename)
            df_write.to_csv(filepath)

            # eval count
            series_list = []
            series_list.append((df_final['1_eval_count']))
            series_list.append((df_final['2_eval_count']))
            series_list.append((df_final['3_eval_count']))
            series_list[0] = series_list[0].sort_values(ignore_index=True)
            series_list[1] = series_list[1].sort_values(ignore_index=True)
            series_list[2] = series_list[2].sort_values(ignore_index=True)
            df_write = pd.concat(series_list,ignore_index=True, axis=1)
            df_write.columns = ["A0","AT","AM"]
            print(df_write)
            filename = "count-plot.csv"
            filepath = os.path.join(path, filename)
            df_write.to_csv(filepath)

            # total
            series_list = []
            series_list.append((df_final['1_eval'] + df_final['1_prep']))
            series_list.append((df_final['2_eval'] + df_final['2_prep']))
            series_list.append((df_final['3_eval'] + df_final['3_prep']))
            series_list[0] = series_list[0].sort_values(ignore_index=True)
            series_list[1] = series_list[1].sort_values(ignore_index=True)
            series_list[2] = series_list[2].sort_values(ignore_index=True)
            df_write = pd.concat(series_list,ignore_index=True, axis=1)
            df_write.columns = ["A0","AT","AM"]
            print(df_write)
            filename = "total-plot.csv"
            filepath = os.path.join(path, filename)
            df_write.to_csv(filepath)

        if setting == 2:
            for i in range(len(dir_list)):
                series_list = []
                for k in range(len(data_frames_list[i])):
                    series = data_frames_list[i][k][str(k+1) + "_time"]
                    series = series.sort_values(ignore_index=True)
                    series_list.append(series)
                df_write = pd.concat(series_list,ignore_index=True, axis=1)
                df_write.columns = ["Staic","Distance","Closest","Static + Distance","BFS","DFS","BestFS"]
                print(df_write)
                filename = "big-plot.csv"
                filepath = os.path.join(path, filename)
                df_write.to_csv(filepath)
    if winners:
        for i in range(len(dir_list)):
            df_final = df_final.drop(columns=["model","query"])
            winners = df_final.idxmin(axis=1)
            winners = winners.value_counts()
            f = open(os.path.join(dir_list,"winners.txt"), "w")
            for k in range(len(dir_list[i])):
                f.write(exp3_list[k] + str(winners[str(k+1) + "_time"]) + "\n")
            f.close()

if __name__ == '__main__':
    cwd = os.getcwd()

    parser = argparse.ArgumentParser(description='For cleaning and sorting')

    parser.add_argument('--f', metavar='N', type=str, nargs=1,
                        help='Path to ')

    parser.add_argument('-format', metavar='N', type=int, nargs=1, required=True,
                    help='An integer for settings')

    # Options:
    #   Inner merge all
    #   Outer merge all
    #   Solo for all


    parser.add_argument('--i', default=False, action='store_true',
                        help='Get Inner Merging Results')

    parser.add_argument('--o', default=False, action='store_true',
                        help='Get Outer Merging Results')

    parser.add_argument('--s', default=False, action='store_true',
                        help='Get Solo Results')

    parser.add_argument('--p', default=False, action='store_true',
                        help='Get Files for Plots')

    parser.add_argument('--w', default=False, action='store_true',
                        help='Get Winners')

    # Tables
    #   Count things

    # Formats
    #   Experiment 1
    #   Experiment 2
    #   Experiment 3
    #       Expirement 3.1 Tables
    #
    args = parser.parse_args()
    if args.format[0] > settings_int:
        print("Not a format")
    elif args.f is None:
        main(cwd,args.format[0]-1,args.i,args.o,args.s,args.p,args.w)
    else:
        cwd = os.path.join(cwd,args.f[0])
        main(cwd,args.format[0]-1,args.i,args.o,args.s,args.p,args.w)