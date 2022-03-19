import os.path
from importlib.machinery import ExtensionFileLoader
pathfinder = ExtensionFileLoader("pathfinder", os.path.join(
    os.path.dirname(os.path.realpath(__file__)), "..", "build", "pathfinder.cpython-310-x86_64-linux-gnu.so")).load_module()
Pathfinder = pathfinder.Pathfinder

import pygame
import sys
from timeit import default_timer as timer

CELL_SIZE = 10

cols = 50
rows = 50

grid = [1 for x in range(cols * rows)]

start = None
end = (0, 0)

pygame.init()
screen = pygame.display.set_mode((cols * CELL_SIZE, rows * CELL_SIZE))
pygame.display.set_caption("Pathfinder test")
clock = pygame.time.Clock()
font = pygame.font.Font(None, 12)


def spos(x, y):
    return x*CELL_SIZE, y*CELL_SIZE


def cpos(x, y):
    return int(x / CELL_SIZE), int(y / CELL_SIZE)


def sposc(x, y):
    return x*CELL_SIZE + CELL_SIZE/2, y*CELL_SIZE + CELL_SIZE/2


def index(x, y):
    return x + cols * y


def update_grid(x, y, val):
    global grid
    i = index(x, y)
    try:
        grid[i] = val
    except:
        pass


def draw_grid(grid, start, end):
    for y in range(rows):
        for x in range(cols):
            cell = grid[index(x, y)]
            if cell == 0:
                pygame.draw.rect(
                    screen, 'black', (*spos(x, y), CELL_SIZE, CELL_SIZE))
            else:
                pass
                pygame.draw.rect(
                    screen, 'grey', (*spos(x, y), CELL_SIZE, CELL_SIZE))
    if start:
        pygame.draw.circle(screen, 'purple', sposc(*start), CELL_SIZE/2.5)
    if end:
        pygame.draw.circle(screen, 'orange', sposc(*end), CELL_SIZE/2.5)


def draw_path(path):
    points = [sposc(*p) for p in path]
    pygame.draw.lines(screen, 'red', False,
                      points, 1
                      )
    for p in points:
        pygame.draw.circle(screen, 'purple', p, CELL_SIZE/10)


def highlight_nodes(nodes):
    for pos in nodes:
        pygame.draw.circle(screen, 'green', sposc(*pos), CELL_SIZE/3)


def onMouseUp(event):
    global start, end

    if event.button == pygame.BUTTON_LEFT:
        end = cpos(*event.pos)
    # if event.button == pygame.BUTTON_RIGHT:
    #     end = cpos(*event.pos)

    screen.fill('white')
    draw_grid(grid, start, end)

    # if start and end:
    #     pfinder = Pathfinder(grid=grid)
    #     path, runs = pfinder.astar_findpath(start, end, diagonal=True)

    #     if 'closed_list' in pfinder.debug.keys():
    #         highlight_nodes(pfinder.debug['closed_list'])

    #     if path:
    #         print("Path found in {} runs".format(runs))
    #         draw_path(path)
    #     else:
    #         print("Path not found after {} runs".format(runs))


def main():
    global start, end

    pfinder = Pathfinder(cols, rows)
    screen.fill('white')
    draw_grid(grid, start, end)
    while True:
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                pygame.quit()
                sys.exit()
            if event.type == pygame.MOUSEBUTTONUP:
                onMouseUp(event)

        keys = pygame.key.get_pressed()
        mouse_position = pygame.mouse.get_pos()

        if keys[pygame.K_a]:
            x, y = cpos(*mouse_position)
            update_grid(x, y, 0)
            screen.fill('white')
            draw_grid(grid, start, end)
        if keys[pygame.K_d]:
            x, y = cpos(*mouse_position)
            update_grid(x, y, 1)
            screen.fill('white')
            draw_grid(grid, start, end)
        if keys[pygame.K_s]:
            start = cpos(*mouse_position)
            screen.fill('white')
            draw_grid(grid, start, end)
            if start and end:
                if pfinder.set_grid(grid):
                    print("Copied Grid")
                t_s = timer()
                found = pfinder.search_astar(*start, *end, True)
                t_e = timer()
                time1 = t_e - t_s

                if found:
                    print("path found in {} runs, {} s".format(
                        pfinder.get_runs(), (time1)))
                    draw_path(pfinder.get_path())
                else:
                    print("astar_findpath not found after {} runs, {} s".format(
                        pfinder.get_runs(), (time1)))

        pygame.display.update()
        clock.tick(60)


if __name__ == "__main__":
    main()
