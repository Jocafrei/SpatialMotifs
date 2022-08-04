import overpy
import sys
import os

api = overpy.Overpass()

name = sys.argv[1]

# fetch from region
'''result = api.query("""
    [out:xml]; 
    area[name = "Manhattan"]; 
    (way(area)[highway]; ); 
    (._;>;); out;
    """)'''
    
# fetch from bounding box
result = api.query("""
    [out:xml];
    (way(51.7511,-1.2659,51.7585,-1.2464)[highway]; ); 
    (._;>;); out;
    """)

counter = {}
valid_nodes = set()
total = 0

for way in result.ways:
    for id, node in enumerate(way.nodes):
        total += 1
        if id == 0 or id == len(way.nodes):
            valid_nodes.add((node.lat, node.lon))
        if (node.lat, node.lon) not in counter:
            counter[(node.lat, node.lon)] = 1
        else:
            counter[(node.lat, node.lon)] += 1
        if counter[(node.lat, node.lon)] > 1:
            valid_nodes.add((node.lat, node.lon))

os.mkdir("../" + name)
output = open("../" + name + "/" + name + ".txt", "a")
output.write(str(0) + "\n")
output.write(str(len(valid_nodes)) + "\n")

nodes = {}
count = 1

for way in result.ways:
    for node in way.nodes:
        if (node.lat, node.lon) in valid_nodes and (node.lat, node.lon) not in nodes:
            output.write(str(node.lat) + " " + str(node.lon) + "\n")
            nodes[(node.lat, node.lon)] = count
            count += 1

edges = 0

for way in result.ways:
    for id, node in enumerate(way.nodes):
        if (node.lat, node.lon) in valid_nodes:
            for i in range(id+1, len(way.nodes)):
                if (way.nodes[i].lat, way.nodes[i].lon) in valid_nodes:
                    edges += 1
                    break

output.write(str(edges) + "\n")

for way in result.ways:
    for id, node in enumerate(way.nodes):
        if (node.lat, node.lon) in valid_nodes:
            for i in range(id+1, len(way.nodes)):
                if (way.nodes[i].lat, way.nodes[i].lon) in valid_nodes:
                    output.write(str(nodes[(node.lat, node.lon)]) + " " + str(nodes[(way.nodes[i].lat, way.nodes[i].lon)]) + " 1\n")
                    break
