import sys
filename = sys.argv[1]
print('filename:'+filename)
with open(filename, 'r') as f:
        l1 = []
        for ele in f:
            line = ele.split('\n')
            cell = line[0].split(',')
            modelquery1 = cell[0].split('/')[2]
            modelquery2 = modelquery1.rsplit('-',1)
            model = modelquery2[0]
            query = modelquery2[1].split(':')[0]
            cell[0] = model
            cell[1] = query
            l1.append(cell)
file=open(filename, 'w')
for li in l1:
    for cel in li[:-1]:
        file.writelines(cel)
        file.writelines(',')
    file.writelines(li[-1])
    file.writelines('\n')
file.close()
