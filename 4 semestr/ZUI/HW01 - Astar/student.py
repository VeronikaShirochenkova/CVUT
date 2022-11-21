from blockworld import BlockWorld


class BlockWorldHeuristic(BlockWorld):
    def __init__(self, num_blocks=5, state=None):
        BlockWorld.__init__(self, num_blocks, state)

    def heuristic(self, goal):
        self_state = self.get_state()
        goal_state = goal.get_state()

        # ToDo. Implement the heuristic here.
        price = 0
        diff = 0
        start_idx = 0
        for i in goal_state:
            for j in self_state:
                if len(i) <= len(j):
                    diff = len(j) - len(i)
                    start_idx = len(j) - 1
                else:
                    diff = len(i) - len(j)
                    start_idx = len(i) - 1

                for a in range(start_idx, diff - 1, -1):
                    if len(i) <= len(j):
                        if j[a] != i[a - diff]:
                            break
                        else:
                            price += 1
                    else:
                        if i[a] != j[a - diff]:
                            break
                        else:
                            price += 1

        return price


class AStar():
    def search(self, start_orig, goal):
        # ToDo. Return a list of optimal actions that takes start to goal.

        # You can access all actions and neighbors like this:
        # for action, neighbor in state.get_neighbors():
        #   ...
        start = start_orig.clone()
        curr = start.clone()

        act = []
        result = []

        while True:
            price = -99
            if start == goal:
                break

            for action, neighbor in curr.get_neighbors():
                curr = start.clone()

                curr.apply(action)
                curr_price = curr.heuristic(goal)
                if curr_price > price:
                    price = curr_price
                    act = action

            start.apply(act)
            curr = start.clone()
            result.append(act)

        return result


if __name__ == '__main__':
    # Here you can test your algorithm. You can try different N values, e.g. 6, 7.
    N = 5

    start = BlockWorldHeuristic(N)
    goal = BlockWorldHeuristic(N)


    print("Searching for a path:")
    print(f"{start} -> {goal}")
    print()

    astar = AStar()
    path = astar.search(start, goal)

    if path is not None:
        print("Found a path:")
        print(path)

        print("\nHere's how it goes:")

        s = start.clone()
        print(s)

        for a in path:
            s.apply(a)
            print(s)

    else:
        print("No path exists.")

    print("Total expanded nodes:", BlockWorld.expanded)