import os
import pandas as pd
import sys
import functools as ft
import os
import argparse

# Dir, Short, Long
header_template_list = [["name", "places","transitions","run1","run2","run3","run4","run5"],
                        ["name","places","transitions","runs","time"],
                        ["model", "query","prep","fire","eval","places","transitions","runs","totaltime"]]
index_list = [[3,4,5,6,7],[3,4],[2,3,4,8]]
usecols_list = [[0,1,2,3,4,5,6,7],[0,1,2,3,4],[0,1,3,5,7,8,9,10,12]]
# Full length, numbered length
length_list = [[8],[5],[9]]
settings_int = 3

def mergeDFs(liste):
    df_result = pd.merge(liste[0],liste[1])
    for i in range(2,len(liste)):
        df_result = pd.merge(df_result,liste[i])
    return df_result

def getDirectories(path):
    dir_list = []
    # Get the directory for the results
    for root,dirs, _ in os.walk(path):
        for dir in dirs:
            if "results" in dir:
                dir_list.append(os.path.join(root, dir))
    dir_list.sort()
    return dir_list

def getDFinFolder(path, setting_index, index, results):
    temp_index = index
    data_frames = []
    file_list = []
    headers = []
    for root,_, files in os.walk(path):
        for file in files:
            if "results" in file:
                file_list.append(file) 
                header = header_template_list[setting_index].copy()
                for num in index_list[setting_index]:
                    header[num] = "_".join([str(temp_index),str(header[num])])
                df = pd.read_csv(os.path.join(root,file), usecols=usecols_list[setting_index], names=header, header=None)
                data_frames.append(df)
                temp_index = temp_index + 1
                headers.append(header)
    return data_frames, file_list, headers, temp_index

def main(path, setting, merge, solo):
    index = 1
    data_frames_list = []
    header_list = []
    merged_frames = []
    file_list = []
    dir_list = []
    int_names = ["A", "ALL", "AM"]

    if setting == 0:
        dir_list = getDirectories(path)
        for dir in dir_list:
            df_list, files, headers, index = getDFinFolder(dir, 0, index, False)
            data_frames_list.append(df_list)
            file_list.append(files)
            header_list.append(headers)
    else:
        # Load dataframes for each folder
        df_list, files, headers, index = getDFinFolder(path, setting, index, True)
        data_frames_list.append(df_list)
        file_list.append(files)
        header_list.append(headers)

    # Merge all frames
    for exp in data_frames_list:
        df_merge = mergeDFs(exp)
        merged_frames.append(df_merge)

    if setting == 0:
        df_final = mergeDFs(merged_frames)
    else:
        df_final = merged_frames[0]

    print(df_final)

    if setting == 0:
        if merge:
            for i in range(len(dir_list)):
                header_set = set()
                header_delete = set(df_final.columns.values.tolist())
                df_write = df_final.copy()
                for header in header_list[i]:
                    header_set = header_set.union(set(header))
                header_delete.difference_update(header_set)
                filename = dir_list[i] +"("
                for file in file_list[i]:
                    filename = filename + file + " "
                filename = filename + ").csv"
                df_write = df_write.drop(columns=header_delete)
                print(df_write)
                df_write.to_csv(filename)
    else:
        if merge:
            for i in range(len(file_list[0])):
                header_delete = set(df_final.columns.values.tolist())
                header_delete.difference_update(set(header_list[0][i]))
                filename = int_names[i] + "-merged.csv"
                df_write = df_final.copy()
                df_write = df_write.drop(columns=header_delete)
                print(df_write)
                df_write.to_csv(filename)
        if solo:
            length = len(file_list[0])
            for i in range(length):
                df_write = data_frames_list[0][i].copy() 
                for k in range(1, length):
                    df_write = pd.merge(df_write,data_frames_list[0][(i+k)%length], how="left",indicator=True)
                    df_write = df_write.loc[df_write['_merge'] == 'left_only']
                    header_delete = set(df_write.columns.values.tolist())
                    header_delete.difference_update(set(header_list[0][i]))
                    df_write = df_write.drop(columns=header_delete)
                filename = int_names[i] +"-solo.csv"
                print(df_write)
                df_write.to_csv(filename)

if __name__ == '__main__':
    cwd = os.getcwd()

    parser = argparse.ArgumentParser(description='For intersection or finding solo results')

    parser.add_argument('--f', metavar='N', type=str, nargs=1,
                        help='Path to ')

    parser.add_argument('-format', metavar='N', type=int, nargs=1, required=True,
                    help='An integer for settings')

    parser.add_argument('--m', default=False, action='store_true',
                        help='Get Merging Results')

    parser.add_argument('--s', default=False, action='store_true',
                        help='Get Solo Results')

    args = parser.parse_args()
    if args.format[0] > settings_int:
        print("Not a format")
    elif args.f is None:
        main(cwd,args.format[0] -1,args.m,args.s)
    else:
        print(args.f)
        cwd = os.path.join(cwd,args.f[0])
        main(cwd,args.format[0]-1,args.m,args.s)