output = open("chileanPG.txt", "a")

num_nodes = sum(1 for line in open("WithTap_node.csv"))
num_edges = sum(1 for line in open("WithTap_edge.csv"))

output.write("0\n")
output.write(str(num_nodes-1) + "\n")

nodes = open("WithTap_node.csv", "r")

next(nodes)
for line in nodes:
    fields = line.split(',')
    output.write(fields[4] + " " + fields[5] + "\n")
nodes.close()

output.write("\n")
output.write(str(num_edges-1) + "\n")

edges = open("WithTap_edge.csv", "r")
next(edges)
for line in edges:
    fields = line.split(',')
    output.write(fields[0] + " " + fields[1] + " " + fields[5])
edges.close()