import os
import glob 
import networkx as nx
import matplotlib
matplotlib.use("Agg")
import matplotlib.pyplot as plt
import json
import webbrowser
from collections import OrderedDict

def gen_subgraph(subg_name, file):
    file.write('''\n        <div class=subgraph>''')
    file.write('''\n          <img src=''' + '''../subgraphs_found/subgraph''' + str(subg_name) + '''.png >''')
    file.write('''\n          <figcaption>Subgraph ''' + str(subg_name) + '''</figcaption>''')
    file.write('''\n        </div>''')


quad_to_clr = {1: "#D9534F", 2: "#FFAD60", 3: "#FFEEAD", 4: "#96CEB4"}
oct_to_clr = {1: "#FFD6AF", 2: "#E0BBE4", 3: "#ECA2A6", 4: "#DCDBD9", 5: "#C3B8AA", 6: "#F4CFDF", 7: "#FFF6D6", 8: "#B6D8F2", 9: "#CAE1D9"}
file = open("new_templatetest.json", "r")
graph = json.load(file)

if not os.path.exists('../datasets/' + str(graph["name"]) + '/subgraphs_found'):
  os.mkdir('../datasets/' + str(graph["name"]) + '/subgraphs_found')

subgfolder = glob.glob('../datasets/' + str(graph["name"]) + '/subgraphs_found/*')

if not os.path.exists('../datasets/' + str(graph["name"]) + '/html/'):
  os.mkdir('../datasets/' + str(graph["name"]) + '/html/')

style = open('../datasets/' + str(graph["name"]) + '/html/style.css', "w")
style.write('''.subgraphs {
    display: grid;
    grid-template-columns: 800px 800px;
}

.subgraph {
    border-style: solid;
    margin-top: 50px;
    margin-bottom: 50px;
    margin-right: 50px;
    margin-left: 50px;
}

figcaption {
    text-align: center;
    font-size: xx-large;
}
''')

label_sort = open('../datasets/' + str(graph["name"]) + '/html/index.html', 'w')
label_sort.write('''<!DOCTYPE html>
<html>
  <link rel="stylesheet" href="style.css">
  <head>
    <title>Sorted by Label</title>
  </head>
  <body>
    <a href=\"occ_sort.html\">Sort By Occurrences</a>
    <div class=subgraphs>''')
for f in subgfolder:
    os.remove(f)

occs = {}
subs = []

for subgraph in graph["subgraphs"]:
    G = nx.Graph()
    clr = []
    
    for node in subgraph["nodes"]:
        G.add_node(node["id"], pos=(node["pos"][0], node["pos"][1]))
        clr.append(oct_to_clr[node["color"]])

    for edge in subgraph["edges"]:
        G.add_edge(edge[0], edge[1])

    pos = nx.get_node_attributes(G,'pos')
    plt.title("Occurrences: " + str(subgraph["occurrences"]) + "\nRelative Frequency: " + str(subgraph["percentage"]))
    nx.draw(G, pos, node_color=clr, node_size=3000, width=3, edgecolors="black")
    ax = plt.gca()
    ax.margins(0.20)
    plt.axis("off")
    
    mapper = open("../resources/subgraphmapper2.json")
    mappings = json.load(mapper, object_pairs_hook=OrderedDict)
    mapper.close()

    subg_name = ""
    if str(subgraph["label"]) in mappings:
        subg_name = mappings[str(subgraph["label"])]
    else:
        subg_name = mappings[list(mappings)[-1]] + 1
        mappings[str(subgraph["label"])] = subg_name
        mapper = open("../resources/subgraphmapper2.json", 'w')
        json.dump(OrderedDict(mappings), mapper, indent = 1, separators=(',', ': '))
        mapper.close()

    plt.savefig('../datasets/' + str(graph["name"]) + "/subgraphs_found/subgraph" + str(subg_name) + ".png")
    plt.clf()

    subs.append(subg_name)
    occs[subg_name] = subgraph["occurrences"]

subs.sort()

for subg_name in subs:
  gen_subgraph(subg_name, label_sort)

label_sort.write('''\n    </div>
  </body>
</html>''')

occs_sorted = sorted(occs.items(), key = lambda kv:(kv[1], kv[0]), reverse=True)

occ_sort = open('../datasets/' + str(graph["name"]) + '/html/occ_sort.html', 'w')
occ_sort.write('''<!DOCTYPE html>
<html>
  <link rel="stylesheet" href="style.css">
  <head>
    <title>Sorted by Label</title>
  </head>
  <body>
    <a href=\"index.html\">Sort By Label</a>
    <div class=subgraphs>''')

for elem in occs_sorted:
    gen_subgraph(elem[0], occ_sort)

occ_sort.write('''\n    </div>
  </body>
</html>''')