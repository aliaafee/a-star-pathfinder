import os.path
from importlib.machinery import ExtensionFileLoader
pathfinder = ExtensionFileLoader("pathfinder", os.path.join(
    os.path.dirname(os.path.realpath(__file__)), "..", "build", "pathfinder.cpython-310-x86_64-linux-gnu.so")).load_module()
Pathfinder = pathfinder.Pathfinder

p = pathfinder.Pathfinder(10, 10)

p.clear()

grid = [1] * 100

if not p.set_grid(grid):
    print("Failed to copy a grid to pathfinder")
else:
    print("Copied grid to pathfider")

found = p.search_astar(1, 1, 9, 9, True)

if found:
    print("Found a path which contains {} steps.".format(len(p.get_path())))
else:
    print("Path not found")


