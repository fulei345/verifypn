import os
import pandas as pd
import sys

if __name__ == '__main__':
    result_path = ""
    file_list = []
    dir_list = []
    cwd = os.getcwd()
    for root,dirs, files in os.walk(cwd):
        for dir in dirs:
            if "results" in dir:
                dir_list.append(os.path.join(root, dir))

    data_frames = []
    merged_frames = []
    split_frame = []

    index = 0
    header = ["name", "places","transitions"]
    time_template = ["1_", "2_", "3_", "4_", "5_"]
    df = None
    dir_list.sort()

    for dir in dir_list:
        file_list.append([])
        for root,_, files in os.walk(dir):
            for file in files:
                file_list[-1].append(file) 
                time = map(lambda x: x + str(index),time_template)
                temp_header = header.copy()
                temp_header.extend(list(time))
                df = pd.read_csv(os.path.join(root,file), usecols=[0,1,2,3,4,5,6,7], names=temp_header, header=None)
                data_frames.append(df)
                if index % 2 != 0:
                    df_merge = pd.merge(data_frames[index-1], data_frames[index])
                    merged_frames.append(df_merge)
                index = index + 1
    
    df_final = pd.merge(merged_frames[0],merged_frames[1])
    df_final = pd.merge(df_final,merged_frames[2])
    df_final = pd.merge(df_final,merged_frames[3])

    print(df_final)

    for i in range(len(merged_frames)):
        df_split = pd.merge(merged_frames[i],df_final, how='inner')
        header_delete = df_split.columns.values.tolist()
        del header_delete[0:12]
        df_split = df_split.drop(columns=header_delete)
        filename = dir_list[i] + "("
        for file in file_list[0]:
            filename += file + " "
        filename += ").csv"
        print(df_split)
        df_split.to_csv(filename)