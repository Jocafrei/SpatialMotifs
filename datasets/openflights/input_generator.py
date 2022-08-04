import csv


output = open("open_flights.txt", "w")

num_nodes = sum(1 for line in open("airports.txt"))
num_edges = sum(1 for line in open("routes.txt"))

output.write("0\n")
output.write(str(num_nodes) + "\n")

nodes = open("airports.txt", "r")

airport_mapping = {}
cur = 0
for line in csv.reader(nodes, quotechar='"', delimiter=',', quoting=csv.QUOTE_ALL, skipinitialspace=True):
    output.write(line[6] + " " + line[7] + "\n")
    airport_mapping[line[4]] = cur
    cur += 1
nodes.close()


output.write("\n")
output.write(str(num_edges-728) + "\n")

edges = open("routes.txt", "r")
next(edges)
for line in edges:
    fields = line.split(',')
    if fields[2] in airport_mapping and fields[4] in airport_mapping:
        output.write(str(airport_mapping[fields[2]]+1) + " " + str(airport_mapping[fields[4]]+1) + " 1\n")
edges.close()